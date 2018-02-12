#include "networks.h"

networks::networks(QObject *parent) : QObject(parent)
{
    TStatus = new QUdpSocket(this);
}

