#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include "controller.h"



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    int purpose = 0;//0: Server, 1: Client
    qDebug() << (int)(*argv[1]);
    controller *new_controller;
    if(argc != 2)
    {
        qDebug() << "Not enough info";
        return 0;
    }
    else
        if((int)(*argv[1]) == 49)
        {
            purpose = 1;
            qDebug() << "Being a Client";
            new_controller = new controller(purpose);
        }
        else
        {
            purpose = 0;
            qDebug() << "Being a server";
            new_controller = new controller(purpose);
        }
    if(purpose == 0)
    {
        qDebug() << "Waiting for input!";
        std::string a = "a";
        while(a != "x")
        {
            std::cin >> a;
            if(a == "a")
                new_controller->send_data();
            else if(a == "b")
                new_controller->send_spam();
        }
    }
    else
        qDebug() << "Waiting for data";
    delete new_controller;
    return a.exec();
}
