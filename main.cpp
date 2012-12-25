#include <iostream>
#include <unistd.h>

#include <QDebug>
#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QQuickItem>

#include "contenttype.h"
#include "systemobject.h"
#include "systemconnection.h"
#include "objectflowmodel.h"
#include "objectflow.h"
#include "serverchooserquickview.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qRegisterMetaType<SystemObjectPtr>();
    qRegisterMetaType<SystemObjectSharedPtr>();
    qRegisterMetaType<SystemObject>();
    qRegisterMetaType<ContentTypePtr>();
    qRegisterMetaType<ContentType>();

    ObjectFlow *flow = new ObjectFlow;

    QSize minimumSize = QSize(200, 150);

    ServerChooserQuickView *serverChooser = new ServerChooserQuickView(QUrl("qrc:/qml/qtconnoisseur/server_chooser.qml"));
    serverChooser->setMinimumSize(minimumSize);
    serverChooser->setBaseSize(minimumSize);

    QQuickView objectFlow(QUrl("qrc:/qml/qtconnoisseur/main.qml"));
    objectFlow.setResizeMode(QQuickView::SizeRootObjectToView);
    objectFlow.setMinimumSize(minimumSize);
    objectFlow.rootContext()->setContextProperty(QLatin1String("objects"), flow->model());
    objectFlow.engine()->addImageProvider(QLatin1String("objects"), flow->model());
    objectFlow.create();

    QObject::connect(serverChooser, &ServerChooserQuickView::okButtonClicked, flow, &ObjectFlow::start);
    QObject::connect(serverChooser->rootObject(), SIGNAL(okButtonClicked(QString, QString)), serverChooser, SLOT(close()));
    QObject::connect(serverChooser, &ServerChooserQuickView::okButtonClicked, &objectFlow, &QQuickView::show);

    // QObject::connect(serverChooser, &ServerChooserQuickView::okButtonClicked,
    //                  []() {
    // });

    serverChooser->show();
    return app.exec();
}
