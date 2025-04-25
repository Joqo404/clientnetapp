#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QCryptographicHash>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->loginpage);
    // Инициализация сокета
    socket = new QTcpSocket(this);
}

MainWindow::~MainWindow()
{
    delete ui;

    // Закрываем соединение и удаляем сокет
    if (socket) {
        socket->disconnectFromHost();
        socket->deleteLater();
    }
}

void MainWindow::on_connectbtn_clicked()
{
    QString loginData = ui->login->text();
    QString passwordData = ui->password->text();

    if (loginData.isEmpty() && passwordData.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Поля логин и пароль не могут быть пустыми!");
        return;
    }
    else if (loginData.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Вы забыли указать логин");
        return;
    }
    else if (passwordData.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Вы забыли указать пароль");
        return;
    }

    // Хэшируем пароль
    QByteArray hashedPassword = QCryptographicHash::hash(passwordData.toUtf8(), QCryptographicHash::Sha256);

    qDebug() << "Логин:" << loginData;
    qDebug() << "Хэш пароля:" << hashedPassword.toHex();

    // Подключаемся к серверу
    socket->connectToHost("127.0.0.1", 5555); // Подключаемся к серверу (IP и порт)

    if (!socket->waitForConnected(3000)) // Ожидаем подключения в течение 3 секунд
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к серверу!");
        return;
    }

    // Формируем данные для отправки
    QString sendData = loginData + ":" + hashedPassword.toHex();
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    out << sendData;

    // Отправляем данные на сервер
    socket->write(data);
    if (!socket->waitForBytesWritten(3000)) // Ожидаем, пока данные будут отправлены
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось отправить данные на сервер!");
        return;
    }

    // Читаем ответ от сервера
    if (socket->waitForReadyRead(3000)) // Ожидаем ответа от сервера
    {
        QDataStream in(socket);
        in.setVersion(QDataStream::Qt_6_4);

        QString response;
        in >> response;

        // Выводим ответ сервера
        if (response.startsWith("SUCCESS")) {
            QMessageBox::information(this, "Успех", "Авторизация прошла успешно!");
            ui->stackedWidget->setCurrentWidget(ui->chatpage);
        } else {
            QMessageBox::critical(this, "Ошибка", "Неверный логин или пароль!");
        }
    }
    else
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось получить ответ от сервера!");
    }

    // Закрываем соединение
    socket->disconnectFromHost();
}
