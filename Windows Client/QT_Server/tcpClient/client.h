#ifndef CLIENT_H
#define CLIENT_H
#include <QAbstractSocket>
#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlTableModel>
#include "qcustomplot.h" //曲线图表第三方库
#include "my_thread.h"
class QTcpSocket;
class QTcpServer ;
namespace Ui {
class Client;
}

class Client : public QDialog
{
    Q_OBJECT
    
public:
    explicit Client(QWidget *parent = 0);
    ~Client();
    void my_sql(const QString &dbName);//创建数据库
    void insert(double pm2_5, double pm10,double temp, double rh);//插入数据
    void Graph_Show(QCustomPlot *parents);//更新曲线
private:
    Ui::Client *ui;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QTcpSocket *tcpServerConnection;


    QSqlTableModel *model;//table 类型的私有对象
    my_thread *camera;
    QCPGraph *graph_PM2_5;//曲线1 PM2.5
    QCPGraph *graph_PM10;//曲线2 PM10
    QCPGraph *graph_TEMP;//曲线2 PM10
    QCPGraph *graph_RH;//曲线2 PM10
    int   model_update;
    void  sendMessage(const char * str);
    void  showenv(double p2_5,double p10,double tem,double R_h);//现实环境数据
private slots:
    void acceptConnection();//TCP服务器有连接槽函数
    void readMessage();
    void displayError(QAbstractSocket::SocketError);
    void sendupMessage();
    void senddownMessage();
    void sendleftMessage();
    void sendrightMessage();
    void sendstopMessage();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void Graph_Show();//定时刷新图表槽函数
    /*监控开关槽函数*/
    void slotButtonCancel();
    void slotButtonOk();
    void slotenvButtonOk();
    void slotenvButtonCancel();
    void slotcircleButtonCancel();

public slots:
    void  Draw_pic(QByteArray image);//绘制图像槽函数
};

#endif // CLIENT_H
