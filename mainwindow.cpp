#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , blockSize(0)
    , waitingForAuthResponse(false) // Инициализация флага ожидания авторизации
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
    QString clientMessage = ui->messageInput->text();
    clientMessage.prepend("Вы: ");
    ui->chatView->addItem(clientMessage);
    ui->messageInput->clear();
}

void MainWindow::receiveMessage()
{
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
        blockSize = 0;

        if (waitingForAuthResponse) {
            waitingForAuthResponse = false; // Сбрасываем флаг ожидания авторизации
            if (message == "SUCCESS") {
                QMessageBox::information(this, "Успех", "Авторизация прошла успешно!");
                ui->stackedWidget->setCurrentWidget(ui->chatpage);
            } else {
                QMessageBox::critical(this, "Ошибка", message);
                socket->disconnectFromHost();
            }
        } else {
            ui->chatView->addItem(message);
        }
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

    // Устанавливаем флаг ожидания авторизации
    waitingForAuthResponse = true;

    // Отправка данных авторизации
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);

    out << quint16(0);
    out << authData;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));

    socket->write(block);

    if (!socket->waitForBytesWritten(3000)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось отправить данные на сервер!");
        return;
    }
}
