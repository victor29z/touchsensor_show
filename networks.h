#ifndef NETWORKS_H
#define NETWORKS_H

#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>
#include <QDebug>
#include <QByteArray>

class networks : public QObject
{
    Q_OBJECT
public:
    explicit networks(QObject *parent = 0);
    QUdpSocket *TStatus;
    unsigned char touch_data[3];
signals:

public slots:
};

#endif // NETWORKS_H
