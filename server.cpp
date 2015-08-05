#include <QtNetwork>

#include <stdlib.h>

#include "server.h"

Server::Server(QString ipAddr, quint32 port, QString purpose)
:  tcpServer(0), networkSession(0)
{
    Server::clientConnection = NULL;
    Server::purpose = purpose;
    Server::port = port;
    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        qDebug() << connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));

        //statusLabel->setText(tr("Opening network session."));
        networkSession->open();
    } else {
        sessionOpened();
    }

        fortunes << tr("You've been leading a dog's life. Stay off the furniture.")
                 << tr("You've got to think about tomorrow.")
                 << tr("You will be surprised by a loud noise.")
                 << tr("You will feel hungry again in another hour.")
                 << tr("You might have mail.")
                 << tr("You cannot kill time without injuring eternity.")
                 << tr("Computers are not intelligent. They only think they are.");
        //connect(tcpServer, SIGNAL(newConnection()), this, SLOT(sendFortune()));
        qDebug() << connect(tcpServer, &QTcpServer::newConnection, this, &Server::openNewConnection);
        //connect(tcpServer, &QTcpServer::newConnection, this, &Server::openNewConnection);
}

void Server::sessionOpened()
{
    // Save the used configuration
    if (networkSession) {
        QNetworkConfiguration config = networkSession->configuration();
        QString id;
        if (config.type() == QNetworkConfiguration::UserChoice)
            id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
        else
            id = config.identifier();

        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }

    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, Server::port)) {
        return;
    }
    qDebug() << "Server listening on: " << tcpServer->serverPort();
//! [0]
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

//! [1]
}

void Server::openNewConnection(void)
{
    qDebug() << "New conn incoming!";
    Server::clientConnection = tcpServer->nextPendingConnection();
    QVariant ipAddr_QVar(clientConnection->peerAddress().toString());
    qDebug() << "Got new connection!";
    emit gotNewConnection(ipAddr_QVar);
}

void Server::disconnectServer(void)
{
    Server::clientConnection->disconnectFromHost();
}


void Server::sendData(QPair<QString, QPair<QString, QVariant> > data)
{
//! [5]
    //QString data = "";
    qDebug() << "Transmitted data in send-data-function is: " + data.first + " " + data.second.first + " " + QString::number(data.second.second.toDouble());
    QPair<QString, QPair<QString, QVariant> > send_data = data;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << (quint16)0;
    out << send_data;
    qDebug() << "Out-status after read: " << out.status();
    Q_ASSERT(out.status() == QDataStream::Ok);
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    qDebug() << "Out-status after read: " << out.status();
    Q_ASSERT(out.status() == QDataStream::Ok);
    if(Server::clientConnection != NULL)
    {
        //qDebug() << "New Connection: " << Server::clientConnection->peerAddress().toString() << " on Port: " << Server::clientConnection->peerPort();
        QVariant ipAddr_QVar(Server::clientConnection->peerAddress().toString());
        //qDebug() << "New qvariant: " << ipAddr_QVar;

        clientConnection->write(block);
        qDebug() << "Block written";
    }
    else
        qDebug() << "No connection!";
}
