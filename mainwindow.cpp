#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QTimer>
#include <QLabel>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pnet = new networks(this);
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();

        // Example use QSerialPort
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
        {
            ui->comboBox->addItem(info.portName());
            serial.close();
        }
    }
    //QLabel lbl[64](this);
    f_num = 0;
    int i,j;
    //f1
    for(i = 0; i < 8; i++)
        for(j = 0; j < 8; j++)
    {
        lbl[i * 8 + j] = new QLabel(this);
        QFont lbl_font;
        lbl_font.setPointSize(8);      //设置字体大小
        //lbl[i * 8 + j].setParent(this);
        lbl[i * 8 + j]->setFont(lbl_font);
        //lbl[i * 8 + j]->setText("00");
        lbl[i * 8 + j]->setText(QString::number(i * 8 + j,16));
        lbl[i * 8 + j]->setGeometry(QRect(20 + 20 * i, 20 + 20 * j, 20, 20)); //设置大小和位置
        lbl[i * 8 + j]->setFrameStyle(QFrame::Panel | QFrame::Sunken); //设置外观

    }
    //f2
    for(i = 0; i < 8; i++)
        for(j = 0; j < 8; j++)
    {
        lbl[i * 8 + j + 64] = new QLabel(this);
        QFont lbl_font;
        lbl_font.setPointSize(8);      //设置字体大小

        lbl[i * 8 + j + 64]->setFont(lbl_font);
        //lbl[i * 8 + j + 64]->setText("00");
        lbl[i * 8 + j + 64]->setText(QString::number(i * 8 + j + 64,16));
        lbl[i * 8 + j + 64]->setGeometry(QRect(200 + 20 * i, 20 + 20 * j, 20, 20)); //设置大小和位置
        lbl[i * 8 + j + 64]->setFrameStyle(QFrame::Panel | QFrame::Sunken); //设置外观

    }

    //f3
    for(i = 0; i < 8; i++)
        for(j = 0; j < 8; j++)
    {
        lbl[i * 8 + j + 128] = new QLabel(this);
        QFont lbl_font;
        lbl_font.setPointSize(8);      //设置字体大小

        lbl[i * 8 + j + 128]->setFont(lbl_font);
        //lbl[i * 8 + j + 128]->setText("00");
        lbl[i * 8 + j + 128]->setText(QString::number(i * 8 + j + 128,16));
        lbl[i * 8 + j + 128]->setGeometry(QRect(380 + 20 * i, 20 + 20 * j, 20, 20)); //设置大小和位置
        lbl[i * 8 + j + 128]->setFrameStyle(QFrame::Panel | QFrame::Sunken); //设置外观

    }
/*

    QLabel *lbl = new QLabel(this);
    QFont lbl_font;
    lbl_font.setPointSize(8);      //设置字体大小
    lbl->setFont(lbl_font);
    lbl->setText("68");
    lbl->setGeometry(QRect(20, 20, 20, 20)); //设置大小和位置
    lbl->setFrameStyle(QFrame::Panel | QFrame::Sunken); //设置外观
*/
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::updateAA()
{
    int i,j;
    unsigned char f = (requestData.data()[6]) - 241; //get finger address
    //if(f == 3) f = 0;
    if(f > 2){
        qDebug()<<"bad finger number";
        return;
    }
    qDebug()<<"finger:"<<f;
    pnet->touch_data[f] = 0;

    for(i = 0; i < 64 ; i++){
        unsigned char value = requestData.data()[i + 7 ];
        lbl[i + f * 64 ]->setText(QString::number(value,16));
        QString str;
        str.sprintf("background-color: rgb(%d, %d, %d);",255,255-value,255-value);
        lbl[i + f * 64]->setStyleSheet(str);

        if(value > TOUCH_FORCE_THRESHOLD)
            pnet->touch_data[f]++;

    }

    ack =true;
    pnet->TStatus->writeDatagram((char*)pnet->touch_data,3,QHostAddress("127.0.0.1"),16274);

/*
    for(i = 0; i < 8; i++)
        for(j = 0; j < 8; j++)
    {
        unsigned char value = requestData.data()[i * 8 + j + 7 ];
        lbl[i * 8 + j + f * 64 ]->setText(QString::number(value,16));

        int tmp,r,g,b;
        if(value >= 0 && value <64){
            r = 255;
            g = value * 4;
            b = 0;

        }
        else if(value >= 64 && value <128){
            tmp = 255 - (value - 64) * 4;
            r = tmp;
            g = 255;
            b = 0;
        }
        else if(value >= 128 && value <192){
            tmp = (value - 128) * 4;
            r = 0;
            g = 255;
            b = tmp;
        }
        else if(value >= 192 && value <255){
            tmp = 255 - (value - 192) * 4;
            r = 0;
            g = tmp;
            b = 255;
        }
        //QString str;
        //str.sprintf("background-color: rgb(%d, %d, %d);",r,g,b);
        //lbl[i * 8 + j]->setStyleSheet(str);


    }*/
}

void MainWindow::getData(){
    int i;
    f_num_last = f_num ;
    f_num = (f_num + 1) % 3;
    if(ack == false){
        for(i = 0; i < 64 ; i++){
            lbl[i + f_num_last * 64 ]->setText(QString::number(0,16));
        }

    }
    // use No4 board for finger 1
    unsigned char addr;
    addr = f_num + 1;
    addr |= 0xf0;
    char data[] = {0x04,0x00,0x03,0x00,addr};
    QByteArray TxData;
    TxData.setRawData(data,5);

    my_serialport->write(TxData);
    ack = false;


}


void MainWindow::on_pushButton_send_clicked()
{
    QByteArray TxData = "aaaa";
    my_serialport->write(TxData);
}

void MainWindow::on_pushButton_cmopen_clicked()
{
    my_serialport= new QSerialPort();
    qDebug()<<ui->comboBox->currentText();
    my_serialport->setPortName("/dev/"+ui->comboBox->currentText());
    my_serialport->open(QIODevice::ReadWrite);
    qDebug()<<ui->lineEdit->text().toInt();
    my_serialport->setBaudRate(ui->lineEdit->text().toInt());
    my_serialport->setDataBits(QSerialPort::Data8);
    my_serialport->setParity(QSerialPort::NoParity);
    my_serialport->setStopBits(QSerialPort::OneStop);
    my_serialport->setFlowControl(QSerialPort::NoFlowControl);
    my_serialport->setReadBufferSize(100);
    timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(updateAA()));
    connect(timer, SIGNAL(timeout()), this, SLOT(getData()));
    connect(my_serialport, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));


    timer->start(33);
}

void MainWindow::readyReadSlot()
{
    unsigned char checksum;


    if(my_serialport->bytesAvailable() < 73){
        qDebug()<<"uncompleted pack:"<<my_serialport->bytesAvailable();
        //if(my_serialport->size() > 99)my_serialport->clear();
        return;
    }

    qDebug()<<"pack size:"<<my_serialport->bytesAvailable();
    //requestData = my_serialport->readAll();
    if(my_serialport->bytesAvailable() > 99){
        qDebug()<<"buffer is full,reset port";
        my_serialport->close();
        my_serialport->open(QIODevice::ReadWrite);


        return;
    }
    requestData = my_serialport->read(73);
    checksum =  (requestData[0] - 0x55) +
                (requestData[1] - 0xaa) +
                (requestData[71] - 0xaa) +
                (requestData[72] - 0x55);
    if(checksum != 0){
        qDebug()<<"bad pack, discard!";
        my_serialport->close();
        my_serialport->open(QIODevice::ReadWrite);


        return;
    }


    //qDebug()<<"size:"<<requestData.size();
    qDebug()<<requestData.toHex();
    /*
    if(requestData!= NULL)
    {
        ui->textEdit->append(requestData);
    }*/


    updateAA();
    requestData.clear();
}

void MainWindow::on_pb_start_clicked()
{
    char data[] = {0x04,0x00,0x03,0x00};
    QByteArray TxData;
    TxData.setRawData(data,4);
    my_serialport->write(TxData);
}

void MainWindow::on_pb_stop_clicked()
{
    char data[] = {0x04,0x00,0x04,0x00};
    QByteArray TxData;
    TxData.setRawData(data,4);
    my_serialport->write(TxData);
}

void MainWindow::on_pb_setz_clicked()
{
    char data[] = {0x04,0x00,0x02,0x00};
    QByteArray TxData;
    TxData.setRawData(data,4);
    my_serialport->write(TxData);
}

void MainWindow::on_pb_reset_clicked()
{
    char data[] = {16,0x00};
    QByteArray TxData;
    TxData.setRawData(data,2);
    my_serialport->write(TxData);
}

void MainWindow::on_pb_get_1_clicked()
{
    char data[] = {0x04,0x00,0x03,0x00,0x01};
    QByteArray TxData;
    TxData.setRawData(data,5);
    f_num = 0;
    my_serialport->write(TxData);
}

void MainWindow::on_pb_get_2_clicked()
{
    char data[] = {0x04,0x00,0x03,0x00,0x02};
    QByteArray TxData;
    TxData.setRawData(data,5);
    f_num = 1;
    my_serialport->write(TxData);
}

void MainWindow::on_pb_get_3_clicked()
{
    char data[] = {0x04,0x00,0x03,0x00,0x03};
    QByteArray TxData;
    TxData.setRawData(data,5);
    f_num = 2;
    my_serialport->write(TxData);
}
