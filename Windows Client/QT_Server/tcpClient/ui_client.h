/********************************************************************************
** Form generated from reading UI file 'client.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENT_H
#define UI_CLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTableView>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE

class Ui_Client
{
public:
    QPushButton *up;
    QPushButton *down;
    QPushButton *left;
    QPushButton *right;
    QPushButton *car_stop;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QDialogButtonBox *Camera_switch;
    QDialogButtonBox *env_switch;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QTableView *tableView;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLineEdit *lineEdit;
    QLabel *label_12;
    QPushButton *pushButton_3;
    QLabel *label_13;
    QLabel *label_14;
    QCustomPlot *widget;
    QLabel *pic_label;
    QLabel *temp_label;
    QLabel *PM25_label;
    QLabel *PM10_label;
    QLabel *RH_label;
    QSlider *camera_Slider;

    void setupUi(QDialog *Client)
    {
        if (Client->objectName().isEmpty())
            Client->setObjectName(QStringLiteral("Client"));
        Client->resize(1331, 788);
        up = new QPushButton(Client);
        up->setObjectName(QStringLiteral("up"));
        up->setGeometry(QRect(1110, 550, 60, 30));
        down = new QPushButton(Client);
        down->setObjectName(QStringLiteral("down"));
        down->setGeometry(QRect(1110, 700, 60, 30));
        left = new QPushButton(Client);
        left->setObjectName(QStringLiteral("left"));
        left->setGeometry(QRect(1022, 630, 60, 30));
        right = new QPushButton(Client);
        right->setObjectName(QStringLiteral("right"));
        right->setGeometry(QRect(1200, 630, 60, 30));
        car_stop = new QPushButton(Client);
        car_stop->setObjectName(QStringLiteral("car_stop"));
        car_stop->setGeometry(QRect(1110, 620, 61, 41));
        label = new QLabel(Client);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(750, 560, 101, 41));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(14);
        label->setFont(font);
        label->setTextFormat(Qt::RichText);
        label_2 = new QLabel(Client);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(1100, 480, 161, 31));
        label_2->setFont(font);
        label_3 = new QLabel(Client);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(650, 620, 131, 41));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(12);
        label_3->setFont(font1);
        label_4 = new QLabel(Client);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(650, 700, 131, 31));
        label_4->setFont(font1);
        Camera_switch = new QDialogButtonBox(Client);
        Camera_switch->setObjectName(QStringLiteral("Camera_switch"));
        Camera_switch->setGeometry(QRect(780, 630, 193, 28));
        Camera_switch->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        Camera_switch->setCenterButtons(false);
        env_switch = new QDialogButtonBox(Client);
        env_switch->setObjectName(QStringLiteral("env_switch"));
        env_switch->setGeometry(QRect(780, 700, 193, 28));
        env_switch->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label_5 = new QLabel(Client);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(650, 660, 121, 31));
        label_5->setFont(font1);
        label_6 = new QLabel(Client);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(500, 180, 161, 41));
        label_6->setFont(font);
        label_7 = new QLabel(Client);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(510, 220, 61, 31));
        label_7->setFont(font1);
        label_8 = new QLabel(Client);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(650, 220, 51, 31));
        label_8->setFont(font1);
        label_9 = new QLabel(Client);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(510, 265, 71, 21));
        QFont font2;
        font2.setFamily(QStringLiteral("Arial"));
        font2.setPointSize(11);
        label_9->setFont(font2);
        label_10 = new QLabel(Client);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(510, 310, 51, 16));
        label_10->setFont(font2);
        label_11 = new QLabel(Client);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(10, 420, 171, 41));
        label_11->setFont(font);
        tableView = new QTableView(Client);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setGeometry(QRect(0, 30, 461, 391));
        pushButton = new QPushButton(Client);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(490, 30, 71, 31));
        pushButton_2 = new QPushButton(Client);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(580, 90, 71, 31));
        lineEdit = new QLineEdit(Client);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(490, 90, 81, 31));
        label_12 = new QLabel(Client);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(490, 60, 211, 21));
        pushButton_3 = new QPushButton(Client);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(490, 130, 81, 41));
        label_13 = new QLabel(Client);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(10, 0, 111, 31));
        QFont font3;
        font3.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font3.setPointSize(14);
        font3.setBold(false);
        font3.setWeight(50);
        label_13->setFont(font3);
        label_14 = new QLabel(Client);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(1060, 10, 121, 21));
        QFont font4;
        font4.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font4.setPointSize(14);
        label_14->setFont(font4);
        widget = new QCustomPlot(Client);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(0, 460, 631, 321));
        pic_label = new QLabel(Client);
        pic_label->setObjectName(QStringLiteral("pic_label"));
        pic_label->setGeometry(QRect(940, 70, 361, 351));
        temp_label = new QLabel(Client);
        temp_label->setObjectName(QStringLiteral("temp_label"));
        temp_label->setGeometry(QRect(560, 220, 61, 31));
        temp_label->setFont(font1);
        PM25_label = new QLabel(Client);
        PM25_label->setObjectName(QStringLiteral("PM25_label"));
        PM25_label->setGeometry(QRect(570, 260, 101, 31));
        PM25_label->setFont(font1);
        PM10_label = new QLabel(Client);
        PM10_label->setObjectName(QStringLiteral("PM10_label"));
        PM10_label->setGeometry(QRect(570, 300, 121, 31));
        PM10_label->setFont(font1);
        RH_label = new QLabel(Client);
        RH_label->setObjectName(QStringLiteral("RH_label"));
        RH_label->setGeometry(QRect(700, 220, 81, 31));
        RH_label->setFont(font1);
        camera_Slider = new QSlider(Client);
        camera_Slider->setObjectName(QStringLiteral("camera_Slider"));
        camera_Slider->setGeometry(QRect(790, 670, 160, 22));
        camera_Slider->setLayoutDirection(Qt::LeftToRight);
        camera_Slider->setMinimum(5);
        camera_Slider->setMaximum(95);
        camera_Slider->setValue(50);
        camera_Slider->setOrientation(Qt::Horizontal);

        retranslateUi(Client);

        QMetaObject::connectSlotsByName(Client);
    } // setupUi

    void retranslateUi(QDialog *Client)
    {
        Client->setWindowTitle(QApplication::translate("Client", "Client", 0));
        up->setText(QApplication::translate("Client", "\344\270\212", 0));
        down->setText(QApplication::translate("Client", "\344\270\213", 0));
        left->setText(QApplication::translate("Client", "\345\267\246", 0));
        right->setText(QApplication::translate("Client", "\345\217\263", 0));
        car_stop->setText(QApplication::translate("Client", "\345\201\234", 0));
        label->setText(QApplication::translate("Client", "\347\233\221\346\216\247\350\256\276\347\275\256\357\274\232", 0));
        label_2->setText(QApplication::translate("Client", "\345\260\217\350\275\246\346\216\247\345\210\266\351\200\211\351\241\271:", 0));
        label_3->setText(QApplication::translate("Client", "\346\221\204\345\203\217\345\244\264\345\274\200\345\205\263:", 0));
        label_4->setText(QApplication::translate("Client", "\347\216\257\345\242\203\347\233\221\346\216\247\345\274\200\345\205\263:", 0));
        label_5->setText(QApplication::translate("Client", "\346\221\204\345\203\217\345\244\264\346\227\213\350\275\254:", 0));
        label_6->setText(QApplication::translate("Client", "\345\275\223\345\211\215\347\216\257\345\242\203\346\225\260\346\215\256:", 0));
        label_7->setText(QApplication::translate("Client", "\346\270\251\345\272\246:", 0));
        label_8->setText(QApplication::translate("Client", "\346\271\277\345\272\246:", 0));
        label_9->setText(QApplication::translate("Client", "PM2.5:", 0));
        label_10->setText(QApplication::translate("Client", "PM10:", 0));
        label_11->setText(QApplication::translate("Client", "24\345\260\217\346\227\266\347\216\257\345\242\203\346\233\262\347\272\277:", 0));
        pushButton->setText(QApplication::translate("Client", "\345\210\267\346\226\260", 0));
        pushButton_2->setText(QApplication::translate("Client", "\346\237\245\350\257\242", 0));
        label_12->setText(QApplication::translate("Client", "\350\276\223\345\205\245\350\246\201\346\237\245\350\257\242\347\232\204\346\227\245\346\234\237(\344\276\213\357\274\23202-13)", 0));
        pushButton_3->setText(QApplication::translate("Client", "\346\230\276\347\244\272\345\205\250\350\241\250", 0));
        label_13->setText(QApplication::translate("Client", "\345\216\206\345\217\262\346\225\260\346\215\256:", 0));
        label_14->setText(QApplication::translate("Client", "\345\256\236\346\227\266\347\233\221\346\216\247\357\274\232", 0));
        pic_label->setText(QString());
        temp_label->setText(QString());
        PM25_label->setText(QString());
        PM10_label->setText(QString());
        RH_label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Client: public Ui_Client {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENT_H
