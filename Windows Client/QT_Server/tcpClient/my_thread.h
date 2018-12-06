#ifndef MY_THREAD_H
#define MY_THREAD_H
#include <QThread>
#include <QDialog>

#include <QUdpSocket>

#include<QTcpSocket>
#include<QTcpServer>
class my_thread :public QThread
{
  Q_OBJECT
public:
    my_thread(int num);
    void stop();
    QTimer *fps_time;
public slots:
   void dataReceived();
   void fps_Show();
protected:
   void run();
signals:     //这里制造一个名为Log的信号
    //在打开视频监控开关的槽函数中该信号关联Draw_pic(QByteArray image);函数
    void    Log(QByteArray   sMessage);
private:
    bool threadSotp;
    int port;     //端口号
    QUdpSocket *receiver;

};

#endif // MY_THREAD_H
