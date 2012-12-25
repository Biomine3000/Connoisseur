#include "objectflow.h"
#include "objectflowmodel.h"

#include "systemconnection.h"

ObjectFlow::ObjectFlow(QObject *parent) :
    QObject(parent)
{
    m_model = new ObjectFlowModel;
    m_connection = new SystemConnection;
}

ObjectFlow::~ObjectFlow()
{
    if (m_connection_thread.isRunning())
        m_connection_thread.quit();

    delete m_connection;
    delete m_model;
}

ObjectFlowModel* ObjectFlow::model()
{
    return m_model;
}

SystemConnection* ObjectFlow::connection()
{
    return m_connection;
}

void ObjectFlow::start(QString host, QString port)
{
    bool ok = false;
    int portNumber = port.toInt(&ok);

    m_connection->host(host);
    m_connection->port(portNumber);

    QObject::connect(m_connection, SIGNAL(objectReceived(SystemObjectSharedPtr)),
                     m_model, SLOT(add(SystemObjectSharedPtr)));

    m_connection->moveToThread(&m_connection_thread);
    QObject::connect(&m_connection_thread, SIGNAL(started()),
                     m_connection, SLOT(run()));
    m_connection_thread.start();

    /*
    this->rootContext()->setContextProperty("objects", m_model);
    this->engine()->addImageProvider(QLatin1String("objects"), m_model);
    // this->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    this->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    // this->setMainQmlFile();
    this->setSource(QUrl("qml/qtconnoisseur/main.qml"));
    this->setWindowTitle(QString("Object Connoisseur™"));
    // this->showExpanded();
    */
}
