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
    // void open(QString host, int port, bool *ok);
    SystemObject* make_routing_subscription();

signals:
    void objectReceived(SystemObjectSharedPtr obj);
    
public slots:
    void receive_loop();
    
private:
    QDataStream *m_stream;
    QTcpSocket *m_socket;
    void subscribe();
};

#endif // SYSTEMCONNECTION_H
