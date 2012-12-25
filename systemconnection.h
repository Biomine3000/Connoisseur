#ifndef SYSTEMCONNECTION_H
#define SYSTEMCONNECTION_H

#include <QTcpSocket>
#include <QThread>
#include <QSharedPointer>

class SystemObject;
typedef QSharedPointer<SystemObject> SystemObjectSharedPtr;

class SystemConnection : public QObject
{
    Q_OBJECT
public:
    SystemConnection(QObject *parent = 0);
    SystemConnection(QString host, int port, QObject *parent = 0);
    SystemObject* make_routing_subscription();

    void host(QString host);
    void port(int port);
signals:
    void objectReceived(SystemObjectSharedPtr obj);

public slots:
    void run();

private:
    QString m_host;
    int m_port;
    QDataStream *m_stream;
    QTcpSocket *m_socket;
    void receive_loop();
    bool m_should_keep_running;
    void subscribe();
};

#endif // SYSTEMCONNECTION_H
