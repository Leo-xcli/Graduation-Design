#include "client.h"
#include "ui_client.h"
#include <QtNetwork>
#include <QLabel>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QStringList>

static int thread_temp=0;   //UDP服务器创建标志位 程序运行过程中只执行一次
static QDateTime date_time;//日期类型
//通信协议说明:
//帧头0x42 0x4d  数据长度(一字节) 帧类型(一字节)  命令值(一字节) 帧尾(一字节)

Client::Client(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Client)
{
   ui->setupUi(this);

   /*****************TCP服务器初始化******************/
     tcpServer =new QTcpServer(this);
     if(! tcpServer ->listen(QHostAddress::Any,1234)){
             qDebug()<<tcpServer->errorString();
             close();
        }
   /**********************END***********************/
     //获取当前时间
     date_time=QDateTime::currentDateTime();

     /******数据库的相关初始化*********************/
     my_sql("first.db");
     model=new QSqlTableModel(this);//使用QSqlTableModel类
     model->setTable("student");
     //OnManualSubmit 所有的改变都会在模型中进行缓存 直到调用submitALL()
     model->setEditStrategy(QSqlTableModel::OnManualSubmit);
     model->select();//选取整个表的所有行
     model->removeColumns(0,2);//从第0列开始 移除2列
     ui->tableView->setModel(model);
     model_update=1;//初始状态下允许定时对数据库列表进行刷新
    /*****************END************************/

    /*****TCP 相关信号槽***************************/
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
    connect(ui->up,SIGNAL(clicked()),this,SLOT(sendupMessage()));
    connect(ui->down,SIGNAL(clicked()),this,SLOT(senddownMessage()));
    connect(ui->left,SIGNAL(clicked()),this,SLOT(sendleftMessage()));
    connect(ui->right,SIGNAL(clicked()),this,SLOT(sendrightMessage()));
    connect(ui->car_stop,SIGNAL(clicked()),this,SLOT(sendstopMessage()));
    /*****************END************************/

    /***************监控开关的信号槽*************/
    connect(ui->Camera_switch->button(QDialogButtonBox::Cancel),
            SIGNAL(clicked()), this, SLOT(slotButtonCancel()));
    connect(ui->Camera_switch->button(QDialogButtonBox::Ok),
            SIGNAL(clicked()), this, SLOT(slotButtonOk()));

    connect(ui->env_switch->button(QDialogButtonBox::Cancel),
            SIGNAL(clicked()), this, SLOT(slotenvButtonCancel()));
    connect(ui->env_switch->button(QDialogButtonBox::Ok),
            SIGNAL(clicked()), this, SLOT(slotenvButtonOk()));

    connect(ui->camera_Slider,SIGNAL(valueChanged(int)), this, SLOT(slotcircleButtonCancel()));//滑块发生变化时

    //尚未有客户机链接服务器，监控开关控件不可用
    ui->Camera_switch->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->Camera_switch->button(QDialogButtonBox::Cancel)->setEnabled(false);

   // ui->camera_Slider->button(QDialogButtonBox::Ok)->setEnabled(false);
   // ui->camera_Slider->button(QDialogButtonBox::Cancel)->setEnabled(false);

    ui->env_switch->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->env_switch->button(QDialogButtonBox::Cancel)->setEnabled(false);  
    /*****************END**********************************/

    /***************曲线图相关初始化**************************/
    ui->widget->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->widget->legend->setFont(legendFont);
    ui->widget->legend->setSelectedFont(legendFont);
    ui->widget->legend->setBrush(QBrush(QColor(255,255,255,0)));//50透明度
    ui->widget->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items
    ui->widget->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);//标签位置
    //添加曲线
    //使用默认坐标轴
    graph_PM2_5=ui->widget->addGraph();
    graph_PM10 =ui->widget->addGraph();
    //规定坐标轴 x轴为xAxis，y轴为yAxis2(右边)
    graph_TEMP =ui->widget->addGraph(ui->widget->xAxis,ui->widget->yAxis2);
    graph_RH   =ui->widget->addGraph(ui->widget->xAxis,ui->widget->yAxis2);
    /****************曲线图的刷新频率************************/
    QTimer *timer = new QTimer(this);
    timer->start(500);
    connect(timer,SIGNAL(timeout()),this,SLOT(Graph_Show()));
    /*****************END**********************************/

}

Client::~Client()
{
    delete ui;
}

void Client:: acceptConnection()
{
    tcpServerConnection = tcpServer ->nextPendingConnection();
    connect(tcpServerConnection,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(tcpServerConnection ,SIGNAL(disconnected()),tcpServerConnection,SLOT(deleteLater()));
    connect(tcpServerConnection,SIGNAL(error(QAbsractSocket::SocketError)),this,SLOT(displayError(QAbsractSocket::SocketError)));
    qDebug()<<tr("connected");
    /*已经有客户连接  监控开关生效*/
    ui->Camera_switch->button(QDialogButtonBox::Ok)->setEnabled(true);
    ui->Camera_switch->button(QDialogButtonBox::Cancel)->setEnabled(true);

    ui->env_switch->button(QDialogButtonBox::Ok)->setEnabled(true);
    ui->env_switch->button(QDialogButtonBox::Cancel)->setEnabled(true);

}
void Client:: readMessage()
{
    QByteArray arr=tcpServerConnection->readAll();
    QString s=arr;
    QString PM2_5 =s.mid(6,4);
    QString PM10  =s.mid(16,4);
    QString TEMP  =s.mid(26,4);
    QString RH    =s.mid(34,4);
    //将字符串转化为double变量
    double p2_5,p10,tem,R_h;
    p2_5 = PM2_5.toDouble();
    p10  = PM10.toDouble();
    tem  = TEMP.toDouble()/10;
    R_h  = RH.toDouble()/10;
    insert(p2_5,p10,tem,R_h);//插入数据库
}
void Client::showenv(double p2_5,double p10,double tem,double R_h)
{
  ui->temp_label->setText(tr("%1 ℃").arg(tem));
  ui->PM25_label->setText(tr("%1 ug/m3").arg(p2_5));
  ui->PM10_label->setText(tr("%1 ug/m3").arg(p10));
  ui->RH_label->setText(tr("%1 %").arg(R_h));
}
void Client::displayError(QAbstractSocket ::SocketError)
{
    qDebug()<<tcpSocket ->errorString();
}
void Client:: sendupMessage()
{
  const char sendbuf[]={0x42,0x4d,0x06,0x01,0x01,0x55};//帧类型 小车控制命令 前进
  tcpServerConnection->write(sendbuf,sizeof(sendbuf));
}
void Client:: senddownMessage()
{
  const char sendbuf[]={0x42,0x4d,0x06,0x01,0x02,0x55};//帧类型 小车控制命令 后退
  tcpServerConnection->write(sendbuf,sizeof(sendbuf));
}
void Client:: sendleftMessage()
{
  const char sendbuf[]={0x42,0x4d,0x06,0x01,0x03,0x55};//帧类型 小车控制命令 左转
  tcpServerConnection->write(sendbuf,sizeof(sendbuf));

}
void Client:: sendrightMessage()
{
  const char sendbuf[]={0x42,0x4d,0x06,0x01,0x04,0x55};//帧类型 小车控制命令 右转

   tcpServerConnection->write(sendbuf,sizeof(sendbuf));
}
void Client:: sendstopMessage()
{
  const char sendbuf[]={0x42,0x4d,0x06,0x01,0x05,0x55};//帧类型 小车控制命令 停止

   tcpServerConnection->write(sendbuf,sizeof(sendbuf));
}
void Client:: my_sql(const QString &dbName)//创建一个数据库
{
  QSqlDatabase database;
  if (QSqlDatabase::contains("qt_sql_default_connection"))
  {
      database = QSqlDatabase::database("qt_sql_default_connection");

  }
  else
  {
      QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
      database.setDatabaseName(dbName);
      database.setUserName("root");
      database.setPassword("123456");
      if (!database.open()) {
            qDebug() << "Database Error!";

        }else{
         QSqlQuery sql_query;

         /*
          * id:用于主键排序时使用
          * Date：具体插入时间点(yyyy-MM-dd hh:mm:ss)
          * Day：月日（MM-dd）
          * Hour:小时（hh）
          * PM2_5,PM10...
          */
         QString create_sql = "create table student (id int primary key,Date vchar,Day vchar,Hour int,PM2_5 double, PM10 double,TEMP double, RH double)";
        //创建 已经存在该数据库文件，就不需要再创建一次
         sql_query.prepare(create_sql);
         if(!sql_query.exec())
         {
           qDebug() << "int here";
           qDebug()<<sql_query.lastError();
         }
         else
         {
           qDebug()<<"table created!";
         }
          }
  }
}
void Client:: insert(double pm2_5, double pm10,double temp, double rh)
{
  QSqlQuery sql_query;
  //查询数据库中的最大id
  QString select_max_sql  = "select max(id) from student";
  //查询按照ID降序排列最后一个数据  student表格中
  QString select_last_sql = "select * from student order by id desc LIMIT 0,1";
  //插入数据
  QString insert_sql = "insert into student values (?,?,?,?,?,?,?,?)";

  /************查询最大id**********/
  int max_id = 0;
  sql_query.prepare(select_max_sql);
  if(!sql_query.exec())
  {
     qDebug()<<sql_query.lastError();
  }else{
    while(sql_query.next())
    {
      max_id = sql_query.value(0).toInt();
      qDebug()<<QString("max id:%1").arg(max_id);
    }
  }
  /************查询上一个数据的插入日期**********/
  sql_query.prepare(select_last_sql);
  if(!sql_query.exec())
  {
     qDebug()<<sql_query.lastError();
  }else{
    while(sql_query.next())
    {
      date_time = sql_query.value(1).toDateTime();//获取上一条数据存入时间 重新赋值给date_time
      qDebug()<<date_time.toString("yyyy-MM-dd hh");
    }
  }

  QString dataTime;//储存时间字符串(yyyy-MM-dd hh:mm:ss)
  QString dataDay;//日期字符串（MM-dd）
  date_time=date_time.addSecs(60*60);//时间加一个小时
  dataTime = date_time.toString("yyyy-MM-dd hh:mm:ss");//转换为特定的字符格式
  //qDebug()<<dataTime;
  dataDay  = date_time.toString("MM-dd");//获得日期
  //qDebug()<<dataDay;
  bool OK;
  int time ;
  time = date_time.toString("hh").toInt(&OK,10);//获得小时数
  //插入数据
  sql_query.prepare(insert_sql);
  sql_query.addBindValue(max_id+1);
  sql_query.addBindValue(dataTime);
  sql_query.addBindValue(dataDay);
  sql_query.addBindValue(time);
  sql_query.addBindValue(pm2_5);
  sql_query.addBindValue(pm10);
  sql_query.addBindValue(temp);
  sql_query.addBindValue(rh);
  if(!sql_query.exec())
  {
  qDebug()<<sql_query.lastError();
  }
  else
  {
  qDebug()<<"inserted!";
  }
}

void Client::on_pushButton_clicked()//数据库刷新操作
{
      model->database().transaction(); //开始事务操作
      if (model->submitAll()) {
          model->database().commit(); //提交
      } else {
          model->database().rollback(); //回滚
          QMessageBox::warning(this, tr("tableModel"),
                               tr("数据库错误: %1")
                               .arg(model->lastError().text()));
      }
}

void Client::on_pushButton_2_clicked()
{
  QString name = ui->lineEdit->text();
  model_update=0;//进行查询时禁止数据库列表进行刷新
  //根据姓名进行筛选
  model->setFilter(QString("Day = '%1'").arg(name));
 //显示结果
  model->select();
}

void Client::on_pushButton_3_clicked()
{
    model_update=1;//点击显示全表后，表示允许数据库定时刷新
    model->setTable("student");
    model->select();
}
void Client::Graph_Show()
{

    Graph_Show(ui->widget);
   // if(model_update==1)
   //   {
    //    model->setTable("student");
    //    model->removeColumns(0,2);//从第0列开始 移除2列
    //    model->select();
    //  }
}

void Client::Graph_Show(QCustomPlot *CustomPlot)
{
    QVector<double> time(24);//横坐标
    QVector<double> PM25(24);//纵坐标
    QVector<double> PM10(24);//纵坐标
    QVector<double> TEMP(24);//纵坐标
    QVector<double> RH(24);//纵坐标
    QSqlQuery sql_query;//进行数据库查询操作
    //查询student表格中的最近24条的数据 使用ID降序
    QString select_all_sql = "select * from student order by id desc limit 0,24";
    //查询最近24小时的数据
    int i=23;
    sql_query.prepare(select_all_sql);
    if(!sql_query.exec())
    {
      qDebug()<<sql_query.lastError();
    }
    else
    {
      while(sql_query.next())
      {
        QDateTime sql_dateTime = sql_query.value(1).toDateTime();
        //qDebug()<<sql_query.value(1).toDateTime();
        //qDebug()<<sql_dateTime.toString("yyyy-MM-dd hh");
        //double sql_time = sql_datelist
        double sql_PM2_5 = sql_query.value(4).toDouble();
        double sql_PM10 = sql_query.value(5).toDouble();
        double sql_TEMP = sql_query.value(6).toDouble();
        double sql_RH = sql_query.value(7).toDouble();
        //将QDateTime类型的sql_dateTime转换为特定格式的字符串在转换为和1970-01-01T00:00:00之间的时间差（秒数）
        time[i]=QDateTime::fromString(sql_dateTime.toString("yyyy-MM-dd hh"),"yyyy-MM-dd hh").toTime_t();
        PM25[i]=sql_PM2_5;
        PM10[i]=sql_PM10;
        TEMP[i]=sql_TEMP;
        RH[i]  =sql_RH;
        i--;
       // qDebug()<<QString("time:%1 PM2_5:%2 PM10:%3").arg(sql_time).arg(sql_PM2_5).arg(sql_PM10);
       }
    }
     showenv(PM25[23],PM10[23],TEMP[23],RH[23]);//实时显示当前环境数据(第23个数据表示最新的环境数据)
/*********************数据库中的数据读取完毕*************/

    //下标的格式：数字或者时钟
    CustomPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    CustomPlot->xAxis->setDateTimeFormat("dd日hh时");//设置显示格式

    CustomPlot->xAxis->setAutoTickStep(false); //是否允许自动下标
    CustomPlot->xAxis->setTickStep(3600*4); //设置间隔6小时显示一个下标

    //设置steps之间的间隔杠
    CustomPlot->xAxis->setAutoSubTicks(false);
    CustomPlot->xAxis->setSubTickCount(3);//一个大刻度包含5个小刻度

    //设置横轴标签
    CustomPlot->xAxis->setLabel("时间(单位h)");
   // time[0]=QDateTime::fromString("2018-02-11 00","yyyy-MM-dd hh").toTime_t();
    CustomPlot->xAxis->setRange(time[0],time[0]+24*3600);//设置横轴范围，一共24小时

    /*****************设置左边纵轴*****************/
    //设置纵轴范围
    CustomPlot->yAxis->setRange(0,300);
    //设置纵轴标签
    CustomPlot->yAxis->setLabel("ug/m3");
    CustomPlot->yAxis->setAutoTickStep(false);
    CustomPlot->yAxis->setTickStep(30);
    CustomPlot->yAxis->setAutoSubTicks(false);
    CustomPlot->yAxis->setSubTickCount(1);//1+1

    /*****************设置右边纵轴*****************/
    CustomPlot->yAxis2->setVisible(true);
    //设置纵轴范围
    CustomPlot->yAxis2->setRange(-20,80);
    //设置纵轴标签
    CustomPlot->yAxis2->setLabel(" ℃ 或 %");
    CustomPlot->yAxis2->setAutoTickStep(false);
    CustomPlot->yAxis2->setTickStep(10);
    CustomPlot->yAxis2->setAutoSubTicks(false);
    CustomPlot->yAxis2->setSubTickCount(1);//1+1


   // CustomPlot->addGraph();
   // CustomPlot->graph(0)->setName("PM2.5");
   // CustomPlot->graph(0)->setPen(QPen(Qt::red));
   // CustomPlot->graph(0)->setData(time,PM25);
        graph_PM2_5->setName("PM2.5");//设置曲线名字
        graph_PM2_5->setPen(QPen(Qt::green));//颜色
        graph_PM2_5->setData(time,PM25);//添加数据 x=time，y=PM25

        graph_PM10->setName("PM10");
        graph_PM10->setPen(QPen(Qt::blue));
        graph_PM10->setData(time,PM10);

        graph_TEMP->setName("温度");
        graph_TEMP->setPen(QPen(Qt::red));
        graph_TEMP->setData(time,TEMP);

        graph_RH->setName("湿度");
        graph_RH->setPen(QPen(Qt::black));
        graph_RH->setData(time,RH);

   // ui->widget->legend->setVisible(true);
   // CustomPlot->legend->setFont(QFont("Helvetica",9));
   // CustomPlot->legend->setBrush(QBrush(QColor(255,255,255,0)));//210透明度
   // CustomPlot->legend->setSelectableParts(QCPLegend::spItems);
   // CustomPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);//标签位置
    CustomPlot->replot();//重新绘制
}
void Client::slotButtonCancel()//关闭视频监控  关闭接受视频数据的线程
{
  const char sendbuf[]={0x42,0x4d,0x06,0x02,0x02,0x55};//帧类型02 命令值02  关视频监控
  tcpServerConnection->write(sendbuf,sizeof(sendbuf));//发送关闭命令给下位机
  qDebug()<<tr("you are in close video");
  camera->stop();//关闭线程
  ui->Camera_switch->button(QDialogButtonBox::Cancel)->setEnabled(false);
  ui->Camera_switch->button(QDialogButtonBox::Ok)->setEnabled(true);
}
void Client::slotButtonOk()//打开视频监控开关 开启一个线程接受视频数据
{
  const char sendbuf[]={0x42,0x4d,0x06,0x02,0x01,0x55};//帧类型02 命令值01  开视频监控
  tcpServerConnection->write(sendbuf,sizeof(sendbuf));
  qDebug()<<tr("you are in open video");
  thread_temp++;
  if(thread_temp==1)//这里保证创建该线程在程序中只能允许一次
    {
     camera = new my_thread(6789);
     connect(camera, SIGNAL(Log(QByteArray)),
             this, SLOT(Draw_pic(QByteArray)));//将槽函数和信号连接起来
    }
  camera->start();//开启线程
  ui->Camera_switch->button(QDialogButtonBox::Cancel)->setEnabled(true);
  ui->Camera_switch->button(QDialogButtonBox::Ok)->setEnabled(false);
}
void Client::slotcircleButtonCancel()//摄像头旋动
{
  unsigned char sendbuf[]={0x42,0x4d,0x06,0x02,0x05,0x55};//帧类型02 命令值sendbuf[4]
  sendbuf[4]=ui->camera_Slider->value();
  tcpServerConnection->write((const char *)sendbuf,sizeof(sendbuf));

 // qDebug()<<tr("hello world")<<sendbuf[4];

}


void Client::slotenvButtonCancel()//环境监控 关
{
 // qDebug()<<tr("you are in envCanel");
  const char sendbuf[]={0x42,0x4d,0x06,0x03,0x02,0x55};//帧类型03 命令值02  关环境监控
  qDebug()<<sizeof(sendbuf);
  tcpServerConnection->write(sendbuf,sizeof(sendbuf));

}
void Client::slotenvButtonOk()//环境监控 开
{
  //qDebug()<<tr("you are in envOk");
  const char sendbuf[]={0x42,0x4d,0x06,0x03,0x01,0x55};//帧类型03 命令值01  开环境监控
  tcpServerConnection->write(sendbuf,sizeof(sendbuf));
}

void Client::Draw_pic(QByteArray image)//绘制图像
{
//  qDebug()<<image<<tr("int slot");
  QImage* img=new QImage;
   if(! ( img->loadFromData(image) ) ) //加载图像loadFromData
  {
      delete img;
      return;
  }
  Client::ui->pic_label->setPixmap(QPixmap::fromImage(*img));
}

