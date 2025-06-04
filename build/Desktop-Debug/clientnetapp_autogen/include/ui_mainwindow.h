/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QStackedWidget *stackedWidget;
    QWidget *loginpage;
    QWidget *gridLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QLineEdit *login;
    QLabel *label_3;
    QLineEdit *password;
    QPushButton *connectbtn;
    QWidget *chatpage;
    QListWidget *chatView;
    QLineEdit *messageInput;
    QPushButton *sendButton;
    QPushButton *voiceBtn;
    QWidget *listpage;
    QLabel *label;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        loginpage = new QWidget();
        loginpage->setObjectName("loginpage");
        gridLayoutWidget = new QWidget(loginpage);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(280, 170, 161, 149));
        verticalLayout = new QVBoxLayout(gridLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName("label_2");

        verticalLayout->addWidget(label_2);

        login = new QLineEdit(gridLayoutWidget);
        login->setObjectName("login");

        verticalLayout->addWidget(login);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName("label_3");

        verticalLayout->addWidget(label_3);

        password = new QLineEdit(gridLayoutWidget);
        password->setObjectName("password");
        password->setEchoMode(QLineEdit::Password);

        verticalLayout->addWidget(password);

        connectbtn = new QPushButton(gridLayoutWidget);
        connectbtn->setObjectName("connectbtn");

        verticalLayout->addWidget(connectbtn);

        stackedWidget->addWidget(loginpage);
        chatpage = new QWidget();
        chatpage->setObjectName("chatpage");
        chatView = new QListWidget(chatpage);
        chatView->setObjectName("chatView");
        chatView->setGeometry(QRect(270, 30, 431, 381));
        messageInput = new QLineEdit(chatpage);
        messageInput->setObjectName("messageInput");
        messageInput->setGeometry(QRect(280, 430, 411, 31));
        sendButton = new QPushButton(chatpage);
        sendButton->setObjectName("sendButton");
        sendButton->setGeometry(QRect(710, 430, 61, 31));
        QIcon icon;
        QString iconThemeName = QString::fromUtf8("mail-send");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        sendButton->setIcon(icon);
        voiceBtn = new QPushButton(chatpage);
        voiceBtn->setObjectName("voiceBtn");
        voiceBtn->setGeometry(QRect(130, 440, 80, 23));
        stackedWidget->addWidget(chatpage);
        listpage = new QWidget();
        listpage->setObjectName("listpage");
        label = new QLabel(listpage);
        label->setObjectName("label");
        label->setGeometry(QRect(150, 110, 231, 61));
        stackedWidget->addWidget(listpage);

        gridLayout->addWidget(stackedWidget, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 20));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Login", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Password", nullptr));
        connectbtn->setText(QCoreApplication::translate("MainWindow", "connect", nullptr));
        sendButton->setText(QString());
        voiceBtn->setText(QCoreApplication::translate("MainWindow", "voiceBtn", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\320\242\321\203\321\202 \320\261\321\203\320\264\320\265\321\202 \320\273\320\270\321\201\321\202 \320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\320\265\320\271", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
