#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QCryptographicHash>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->loginpage);
    socket = new QTcpSocket(this);
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::sendMessage);
    connect(ui->messageInput, &QLineEdit::returnPressed, this, &MainWindow::sendMessage);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::receiveMessage);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (socket) {
        socket->disconnectFromHost();
        socket->deleteLater();
    }
}

void MainWindow::sendMessage()
{
    QString message = ui->messageInput->text().trimmed();
    if (message.isEmpty()) return;

    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_4);

        out << quint16(0);
        out << message;
        out.device()->seek(0);
        out << quint16(block.size() - sizeof(quint16));

        socket->write(block);
    }

    ui->messageInput->clear();
}

void MainWindow::receiveMessage()
{
    static quint16 blockSize = 0;
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_4);

    while (true) {
        if (blockSize == 0) {
            if (socket->bytesAvailable() < sizeof(quint16)) break;
            in >> blockSize;
        }

        if (socket->bytesAvailable() < blockSize) break;

        QString message;
        in >> message;
        ui->chatView->addItem(message);

        blockSize = 0;
    }
}

void MainWindow::on_connectbtn_clicked()
{
    QString loginData = ui->login->text();
    QString passwordData = ui->password->text();

    if (loginData.isEmpty() || passwordData.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Логин и пароль не могут быть пустыми!");
        return;
    }

    QByteArray hashedPassword = QCryptographicHash::hash(passwordData.toUtf8(), QCryptographicHash::Sha256);
    QString authData = loginData + ":" + hashedPassword.toHex();

    qDebug() << "Отправка данных авторизации:" << authData;

    socket->connectToHost("127.0.0.1", 5555);

    if (!socket->waitForConnected(3000)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к серверу!");
        return;
    }

    // Отправка данных с длиной пакета
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);

    out << quint16(0);
    out << authData;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));

    qDebug() << "Размер блока:" << block.size() << "Содержимое:" << block.toHex();

    socket->write(block);

    if (!socket->waitForBytesWritten(3000)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось отправить данные на сервер!");
        return;
    }

    if (socket->waitForReadyRead(5000)) {
        // Чтение ответа по тому же протоколу, что и обычные сообщения
        static quint16 blockSize = 0;
        QDataStream in(socket);
        in.setVersion(QDataStream::Qt_6_4);

        if (blockSize == 0) {
            if (socket->bytesAvailable() < sizeof(quint16)) {
                QMessageBox::critical(this, "Ошибка", "Неполный заголовок пакета");
                return;
            }
            in >> blockSize;
        }

        if (socket->bytesAvailable() < blockSize) {
            QMessageBox::critical(this, "Ошибка", "Неполный пакет данных");
            return;
        }

        QString response;
        in >> response;
        blockSize = 0; // Сбрасываем для следующего сообщения

        qDebug() << "Получен ответ от сервера:" << response;

        if (response == "SUCCESS") {
            QMessageBox::information(this, "Успех", "Авторизация прошла успешно!");
            ui->stackedWidget->setCurrentWidget(ui->chatpage);
        } else {
            QMessageBox::critical(this, "Ошибка", response);
        }
    } else {
        QMessageBox::critical(this, "Ошибка", "Нет ответа от сервера!");
        qDebug() << "Доступно байт:" << socket->bytesAvailable();
    }
}

