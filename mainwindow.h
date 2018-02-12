#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QWidget>
#include <QTimer>
#include <QLabel>
#include "networks.h"

#define TOUCH_FORCE_THRESHOLD 50
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void updateAA();

    void getData();

    void on_pushButton_send_clicked();

    void on_pushButton_cmopen_clicked();
    void readyReadSlot();

    void on_pb_start_clicked();

    void on_pb_stop_clicked();

    void on_pb_setz_clicked();

    void on_pb_reset_clicked();

    void on_pb_get_1_clicked();

    void on_pb_get_2_clicked();

    void on_pb_get_3_clicked();

private:
    Ui::MainWindow *ui;
    QLabel *lbl[192];
public:
    QSerialPort *my_serialport;
    QByteArray requestData;
    QTimer *timer;
    int f_num,f_num_last;
    bool ack;
    networks *pnet;
};

#endif // MAINWINDOW_H
