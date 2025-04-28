#pragma once

#include <QMainWindow>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QAudioInput>
#include <QAudioOutput>
#include <QBuffer>
#include <QAudioFormat>
#include <QMediaDevices>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QBuffer>
#include <QAudioSource>
#include <QAudioSink>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void sendMessage();
    void receiveMessage();
    void on_connectbtn_clicked();
    void startVoiceTransmission();
    void stopVoiceTransmission();
    void receiveVoice();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QUdpSocket *udpSocket;
    quint16 blockSize;
    bool waitingForAuthResponse;

    QAudioSource *audioSource = nullptr;  // Было QAudioInput
    QAudioSink *audioSink = nullptr;      // Было QAudioOutput
    QIODevice *inputDevice = nullptr;
};
