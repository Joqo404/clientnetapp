#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket> // Для работы с сетью
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr); // Конструктор
    ~MainWindow();                         // Деструктор

private slots:
    void on_connectbtn_clicked();         // Слот для обработки нажатия кнопки "Подключиться"

private:
    Ui::MainWindow *ui;                   // Указатель на UI-объект, созданный Qt Designer
    QTcpSocket *socket;
    void sendMessage();        // Сокет для взаимодействия с сервером
};

#endif // MAINWINDOW_H
