#include "mainwindow.h"
#include "./ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , blockSize(0)
    , waitingForAuthResponse(false)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);
    udpSocket = new QUdpSocket(this);

    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::sendMessage);
    connect(ui->messageInput, &QLineEdit::returnPressed, this, &MainWindow::sendMessage);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::receiveMessage);

    connect(ui->voiceBtn, &QPushButton::pressed, this, &MainWindow::startVoiceTransmission);
    connect(ui->voiceBtn, &QPushButton::released, this, &MainWindow::stopVoiceTransmission);

    udpSocket->bind(QHostAddress::LocalHost, 5557);
    connect(udpSocket, &QUdpSocket::readyRead, this, &MainWindow::receiveVoice);

    ui->stackedWidget->setCurrentWidget(ui->loginpage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendMessage()
{
    QString message = ui->messageInput->text().trimmed();
    if (message.isEmpty()) return;

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);

    out << quint16(0);
    out << message;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));

    socket->write(block);

    ui->chatView->addItem("Вы: " + message);
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
            waitingForAuthResponse = false;
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

    socket->connectToHost("127.0.0.1", 5555);

    if (!socket->waitForConnected(3000)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к серверу!");
        return;
    }

    waitingForAuthResponse = true;

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);

    out << quint16(0);
    out << authData;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));

    socket->write(block);
}

void MainWindow::startVoiceTransmission()
{
    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    QAudioDevice inputDev = QMediaDevices::defaultAudioInput();
    if (!inputDev.isFormatSupported(format)) {
        qWarning() << "Формат записи не поддерживается!";
        return;
    }

    audioSource = new QAudioSource(inputDev, format, this);
    inputDevice = audioSource->start();  // Запускаем запись

    connect(inputDevice, &QIODevice::readyRead, this, [this]() {
        QByteArray data = inputDevice->readAll();
        udpSocket->writeDatagram(data, QHostAddress::LocalHost, 5556);
    });
}

void MainWindow::stopVoiceTransmission()
{
    if (audioSource) {
        audioSource->stop();
        audioSource->deleteLater();
        audioSource = nullptr;
        inputDevice = nullptr;
    }
}

void MainWindow::receiveVoice()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray data;
        data.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(data.data(), data.size());

        QBuffer *buffer = new QBuffer(this);
        buffer->setData(data);
        buffer->open(QIODevice::ReadOnly);

        if (!audioSink) {
            QAudioFormat format;
            format.setSampleRate(8000);
            format.setChannelCount(1);
            format.setSampleFormat(QAudioFormat::Int16);

            QAudioDevice outputDev = QMediaDevices::defaultAudioOutput();
            if (!outputDev.isFormatSupported(format)) {
                qWarning() << "Формат воспроизведения не поддерживается!";
                delete buffer;
                return;
            }

            audioSink = new QAudioSink(outputDev, format, this);
        }

        audioSink->start(buffer);  // Воспроизводим данные
    }
}
