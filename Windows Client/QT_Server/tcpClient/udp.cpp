#include "udp.h"
#include <QUdpSocket>
#include <QMessageBox>
#include <QHostAddress>
udp::udp(int udp_port)
{
  port = udp_port;
  udpSocket=new QUdpSocket;
  bool result = udpSocket->bind(port);    //把udpSocket绑定到相应的端口
   if(!result)   //result为false的话，那么就进行下面的，也就是绑定失败
     {
   //    QMessageBox::information(this, tr("error"), tr("udp socket create error!"));    //udp协议套接字创建失败
       return;
     }
  //connect(udpSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));    //收集数据，电报
}

void udp::dataReceived()
{
  while(udpSocket->hasPendingDatagrams())   //如果有数据报的话
    {
      QByteArray datagram;    //字节流
      datagram.resize(udpSocket->pendingDatagramSize());  //里面那个获得第一个数据报的长度，设定字节流的长度
      udpSocket->readDatagram(datagram.data(), datagram.size());    //读取相应长度的数据报到datagram
      QString msg = datagram.data();    //得到字节流的数据
      qDebug()<<msg;
    }


}
