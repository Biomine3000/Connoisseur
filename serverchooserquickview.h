#ifndef SERVERCHOOSERQUICKVIEW_H
#define SERVERCHOOSERQUICKVIEW_H

#include <QObject>
#include <QQuickView>
#include <QUrl>

class ServerChooserQuickView : public QQuickView
{
    Q_OBJECT
public:
    explicit ServerChooserQuickView(QWindow *parent = 0);
    ServerChooserQuickView(const QUrl &source, QWindow *parent = 0);

signals:
    void okButtonClicked(QString host, QString port);
    
public slots:
    void okButtonClickedSlot(QString host, QString port);
};

#endif // SERVERCHOOSERQUICKVIEW_H
