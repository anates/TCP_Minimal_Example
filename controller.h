#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "client.h"
#include "server.h"
#include <QObject>
#include <iostream>

class controller : public QObject
{
    Q_OBJECT
private:
    int purpose;
    Server *myServer = NULL;
    Client *myClient = NULL;
public slots:
    void dataReceived(QPair<QString, QPair<QString, QVariant> > data);
public:
    controller(int purpose);
    ~controller();
    void send_data();
    void send_spam();

};

#endif // CONTROLLER_H
