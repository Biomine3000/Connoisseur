#include <QQuickItem>
#include "serverchooserquickview.h"

ServerChooserQuickView::ServerChooserQuickView(QWindow *parent) :
    QQuickView(parent)
{
}

ServerChooserQuickView::ServerChooserQuickView(const QUrl &source, QWindow *parent) :
    QQuickView(source, parent)
{
    QObject::connect(this->rootObject(), SIGNAL(okButtonClicked(QString, QString)),
                     this, SLOT(okButtonClickedSlot(QString,QString)));
    this->setResizeMode(QQuickView::SizeRootObjectToView);
}

void ServerChooserQuickView::okButtonClickedSlot(QString host, QString port)
{
    emit okButtonClicked(host, port);
}
