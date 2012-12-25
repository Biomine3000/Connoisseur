#ifndef OBJECTFLOWMODEL_H
#define OBJECTFLOWMODEL_H

#include <QObject>
#include <QVariant>
#include <QAbstractListModel>
#include <QQuickImageProvider>
#include <QList>
#include <QMutex>
#include <QSharedPointer>

class SystemObject;
typedef QSharedPointer<SystemObject> SystemObjectSharedPtr;

class ObjectFlowModel : public QAbstractListModel,
        public QQuickImageProvider
{
    Q_OBJECT

    enum Roles {
        IdRole = Qt::UserRole + 1,
        TypeRole,
        MetadataRole,
        PayloadRole
    };

public:
    explicit ObjectFlowModel(QObject *parent = 0);
    ~ObjectFlowModel();

    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    QImage requestImage(const QString &IdRole, QSize *size, const QSize &requestedSize);

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
