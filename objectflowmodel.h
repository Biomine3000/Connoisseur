#ifndef OBJECTFLOWMODEL_H
#define OBJECTFLOWMODEL_H

#include <QObject>
#include <QVariant>
#include <QAbstractListModel>
#include <QDeclarativeImageProvider>
#include <QList>
#include <QMutex>
#include <QSharedPointer>

class SystemObject;
typedef QSharedPointer<SystemObject> SystemObjectSharedPtr;

class ObjectFlowModel : public QAbstractListModel,
        public QDeclarativeImageProvider
{
    Q_OBJECT

    enum Roles {
        id = Qt::UserRole + 1,
        type,
        metadata,
        payload
    };

public:
    explicit ObjectFlowModel(QObject *parent = 0);
    ~ObjectFlowModel();

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

signals:

public slots:
    void add(SystemObjectSharedPtr obj);

private:
    QMutex m_mutex;
    QList<SystemObjectSharedPtr> m_objects;
    void removeExtras();
    QVariant dataForRole(QSharedPointer<SystemObject> obj, int role) const;
};

#endif // OBJECTFLOWMODEL_H
