#include <QObject>
#include <QSharedPointer>
#include <QtCore/QDataStream>

#include "systemconnection.h"
#include "systemobject.h"

SystemConnection::SystemConnection(QObject *parent) :
    QObject(parent)
{
}

SystemConnection::SystemConnection(QString host, int port, QObject *parent) :
    QObject(parent), m_host(host), m_port(port)
{
}

void SystemConnection::receive_loop()
{
    m_socket = new QTcpSocket;
    m_socket->connectToHost(m_host, m_port, QTcpSocket::ReadWrite);

    if (!m_socket->waitForConnected(15000)) {
        qDebug() << m_socket->errorString();
        return;
    }
    qDebug() << "Connected!";

    m_stream = new QDataStream(m_socket);
    this->subscribe();

    qDebug() << "Entering receive loop!";
    while(true) {
        if (m_socket->state() != QAbstractSocket::ConnectedState) {
            qDebug() << "Stream status:" << m_stream->status();
            qDebug() << "Socket state:" << m_socket->state();
            return;
        }

        QSharedPointer<SystemObject> obj =
                   QSharedPointer<SystemObject>(new SystemObject(m_socket));
        //                                     &QObject::deleteLater);

        // qDebug() << "Current thread of SystemConnection:" << QThread::currentThread();
        qDebug() << "receive_loop() received:" << *obj << "emitting objectReceived(obj)";
        this->dumpObjectInfo();

        emit objectReceived(obj);
        obj.clear();
    }
}

void SystemConnection::subscribe()
{
    SystemObject *subscription = make_routing_subscription();
    *m_stream << *subscription;
    m_socket->flush();
    subscription->deleteLater();

    qDebug() << "Sent subscription object!";
}

SystemObject* SystemConnection::make_routing_subscription()
{
    QMap<QString,QVariant> metadata;
    metadata["event"] = "routing/subscribe";
    metadata["receive_mode"] = "all";
    metadata["types"] = "all";
    SystemObject *ret = new SystemObject(metadata, QByteArray());
    return ret;
}
