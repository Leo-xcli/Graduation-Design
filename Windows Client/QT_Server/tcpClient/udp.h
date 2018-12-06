#ifndef UDP_H
#define UDP_H
#include <QDialog>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QUdpSocket>

class udp
{
public:
    udp(int udp_port);
public slots:
   void dataReceived();
private:
   int port;   //端口号
   QUdpSocket *udpSocket;    //套接字
};

#endif // UDP_H
