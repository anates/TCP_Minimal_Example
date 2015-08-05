#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <iostream>
//#include <QtTest/QTest>
#include <QSignalSpy>
#include <QTcpSocket>
#include <QDebug>
//#include <QMessageBox>
#include <QNetworkInterface>
#include <typeinfo>
#include <QStringList>
//#include <QSignalSpy>

QT_BEGIN_NAMESPACE
class QTcpServer;
class QNetworkSession;
QT_END_NAMESPACE

class Server : public QObject
{
    Q_OBJECT
public slots:
    void sessionOpened();
    void sendData(QPair<QString, QPair<QString, QVariant> > data);
    void disconnectServer(void);
    void openNewConnection(void);
signals:
    void gotNewConnection(QVariant);
private:
    QString purpose;
    QTcpServer *tcpServer;
    QString ipAddr;
    quint32 port;
    QTcpSocket *clientConnection;
    quint32 BlockSize;
    bool firstTime;
    QSignalSpy * m_pSignalSpy;
    QStringList fortunes;
    QNetworkSession *networkSession;
    //QNetworkConfiguration config;
public:
    Server(QString ipAddr, quint32 port, QString purpose = "");
};

#endif // SERVER_H
