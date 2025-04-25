#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectbtn_clicked()
{
    QString loginData = ui->login->text();
    QString passswordData = ui->password->text();
    //qDebug() << loginData << passswordData;
    if (loginData.isEmpty() && passswordData.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Поля логин и пароль не могут быть пустыми!");
    }
    else if(loginData.isEmpty()){
        QMessageBox::warning(this, "Ошибка", "Вы забыли указать логин");
    }
    else if(passswordData.isEmpty()){
        QMessageBox::warning(this, "Ошибка", "Вы забыли указать пароль");
    }
    else
    {
        qDebug() << "получилось" << loginData << passswordData;
        QByteArray hashedPassword = QCryptographicHash::hash(passswordData.toUtf8(), QCryptographicHash::Sha256);
        qDebug() << "Логин:" << loginData;
        qDebug() << "Хэш пароля:" << hashedPassword.toHex();
    }
}

