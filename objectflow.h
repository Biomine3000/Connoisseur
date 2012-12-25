#ifndef OBJECTFLOW_H
#define OBJECTFLOW_H

#include <QObject>
#include <QString>
#include <QThread>

#include "objectflowmodel.h"
#include "systemconnection.h"

class ObjectFlow : public QObject
{
    Q_OBJECT

    Q_PROPERTY(ObjectFlowModel* m_model READ model)
    Q_PROPERTY(SystemConnection* m_connection READ connection)
public:
    explicit ObjectFlow(QObject *parent = 0);
    ~ObjectFlow();

    ObjectFlowModel* model();
    SystemConnection* connection();
private:
    ObjectFlowModel* m_model;
    SystemConnection* m_connection;
    QThread m_connection_thread;
signals:
    
public slots:
    void start(QString host, QString port);
};

#endif // OBJECTFLOW_H
