#include "my_thread.h"
#include <QLabel>
#include <QDebug>
#include <QMessageBox>
#include <QtCore>
static int fps;
my_thread::my_thread(int num)
{
  port = num;
  threadSotp= false;
  receiver = new QUdpSocket(this);
  receiver->bind(QHostAddress("192.168.43.144"),port,QUdpSocket::ShareAddress);
  fps_time = new QTimer(this);
  fps_time->start(1000);
  connect(fps_time,SIGNAL(timeout()),this,SLOT(fps_Show()));

 // connect(receiver,SIGNAL(readyRead()),this,SLOT(dataReceived()));

  //tcpSocket=new QTcpSocket(this);
 // tcpSocket->connectToHost("192.168.1.10",tcp_port,QTcpSocket::ReadWrite);
}


void my_thread::stop()
{
  threadSotp=true;
  qDebug()<<"my_thread stop";
}
void my_thread::run()//线程运行槽函数
{

  QByteArray image;
  threadSotp=false;
  while(!threadSotp)
    {
      while(receiver->hasPendingDatagrams())   //如果有数据报的话
        {

          QByteArray datagram;    //字节流
          datagram.resize(receiver->pendingDatagramSize());  //里面那个获得第一个数据报的长度，设定字节流的长度
          receiver->readDatagram(datagram.data(), datagram.size());    //读取相应长度的数据报到datagram
          QString msg = datagram.data();    //得到字节流的数据
          emit   Log(datagram);//发射一个Log信号，传递datagram数据
          fps++;
          //qDebug()<<msg;
        }
    }
}
void my_thread::dataReceived()//数据接槽函数
{
  while(receiver->hasPendingDatagrams())   //如果有数据报的话
    {
      QByteArray datagram;    //字节流
      datagram.resize(receiver->pendingDatagramSize());  //里面那个获得第一个数据报的长度，设定字节流的长度
      receiver->readDatagram(datagram.data(), datagram.size());    //读取相应长度的数据报到datagram
      QString msg = datagram.data();    //得到字节流的数据
      emit   Log(datagram);
      //qDebug()<<msg;
    }
}
void my_thread::fps_Show()//数据接槽函数
{
      qDebug()<<fps<<"fps";
      fps=0;
}
