#include <iostream>
#include <unistd.h>

#include <QThread>
#include <QSharedPointer>
#include <QDebug>
#include <QtGui/QApplication>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include "qmlapplicationviewer.h"
#include <QtCore>

#include "contenttype.h"
#include "systemobject.h"
#include "systemconnection.h"
#include "objectflowmodel.h"


Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    qRegisterMetaType<SystemObjectPtr>();
    qRegisterMetaType<SystemObjectSharedPtr>();
    qRegisterMetaType<SystemObject>();
    qRegisterMetaType<ContentTypePtr>();
    qRegisterMetaType<ContentType>();

    ObjectFlowModel *model = new ObjectFlowModel;
    SystemConnection *conn = new SystemConnection();

    QObject::connect(conn, SIGNAL(objectReceived(SystemObjectSharedPtr)),
                     model, SLOT(add(SystemObjectSharedPtr)));

    QThread *connection_thread = new QThread;
    conn->moveToThread(connection_thread);
    QObject::connect(connection_thread, SIGNAL(started()),
                     conn, SLOT(receive_loop()));
    connection_thread->start();
    // conn->receive_loop();

    QmlApplicationViewer viewer;
    viewer.rootContext()->setContextProperty("objects", model);
    viewer.engine()->addImageProvider(QLatin1String("objects"), model);
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setResizeMode(QDeclarativeView::SizeRootObjectToView);
    viewer.setMainQmlFile(QLatin1String("qml/qtconnoisseur/main.qml"));
    viewer.setWindowTitle(QString("Object Connoisseur™"));
    viewer.showExpanded();

    qDebug() << "Current thread of main execution: " << QThread::currentThread();

    return app->exec();

    /*
    delete conn;
    delete model;
    */
}
