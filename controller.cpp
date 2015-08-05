#include "controller.h"

controller::controller(int purpose)
{
    this->purpose = purpose;
    if(purpose == 1)
    {
        std::cout << "Starting Client";
        this->myClient = new Client(QString(QString("test")));
        myClient->connectToServer(QString("127.0.0.1"), 60000);
        qDebug() << connect(myClient, &Client::gotData, this, &controller::dataReceived);
    }
    else
    {
        std::cout << "Starting Server";
        this->myServer = new Server(QString("127.0.0.1"), 60000, QString("test"));
    }
}

controller::~controller()
{
    if(this->myServer != NULL)
        delete myServer;
    if(this->myClient != NULL)
        delete myClient;
}

void controller::dataReceived(QPair<QString, QPair<QString, QVariant> > data)
{
    qDebug() << data.first << data.second.first << data.second.second.toString();
}

void controller::send_data()
{
    if(this->purpose == 0)
        this->myServer->sendData(qMakePair(QString("Hello"), qMakePair(QString("World"), QVariant("this"))));
}

void controller::send_spam()
{
    if(this->purpose == 0)
        for(int i = 0; i < 1000; i++)
        {
            this->myServer->sendData(qMakePair(QString("Hello"), qMakePair(QString("World"), QVariant(i))));
        }
}
