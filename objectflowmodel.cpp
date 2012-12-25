#include <QThread>
#include <QMutex>
#include <QList>
#include <QImage>
#include <QUrl>

#include "systemobject.h"
#include "objectflowmodel.h"


ObjectFlowModel::ObjectFlowModel(QObject *parent) :
    QAbstractListModel(parent),
    QQuickImageProvider(QQuickImageProvider::Image)
{
}

ObjectFlowModel::~ObjectFlowModel()
{
}

QHash<int, QByteArray> ObjectFlowModel::roleNames() const
{
    QHash<int, QByteArray> roles; // = roleNames();
    roles[IdRole] = "id";
    roles[TypeRole] = "type";
    roles[MetadataRole] = "metadata";
    roles[PayloadRole] = "payload";
    return roles;
}

void ObjectFlowModel::add(SystemObjectSharedPtr obj)
{
    // qDebug() << "Current thread of ObjectFlowModel:" << QThread::currentThread();
    // qDebug() << "Add" << *obj;
    m_mutex.lock();
    beginInsertRows(QModelIndex(), 0, 0);
    m_objects.prepend(obj);
    endInsertRows();
    removeExtras();
    m_mutex.unlock();
}

int ObjectFlowModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_objects.length();
}

QVariant ObjectFlowModel::data(const QModelIndex &index, int role) const
{
    // qDebug() << roleNames();
    // qDebug() << "Role requested:" << role;
    // qDebug() << "Index:" << index << "row:" << index.row();

    QSharedPointer<SystemObject> obj = m_objects.at(index.row());
    // qDebug() << "Object:" << *obj;

    return dataForRole(obj, role);
}

void ObjectFlowModel::removeExtras()
{
    // THIS METHOD SHOULD NOT BE CALLED BY ANYONE ELSE THAN add
    if (m_objects.length() > 25) {
        beginRemoveRows(QModelIndex(), m_objects.length() - 1, m_objects.length() - 1);
        QSharedPointer<SystemObject> obj = m_objects.at(0);
        m_objects.removeLast();
        endRemoveRows();
        // obj.clear();
        // qDebug() << "Remove" << *obj;
    }
}

QVariant ObjectFlowModel::dataForRole(QSharedPointer<SystemObject> obj, int role) const
{
    switch(role) {
    case IdRole:
        return obj->id();
    case TypeRole:
        return QVariant::fromValue<QObject *>(obj->type());
    case MetadataRole:
        return obj->metadata();
    case PayloadRole:
        return obj->payload();
    default:
        return QVariant(obj->toString());
    }
}

QImage ObjectFlowModel::requestImage(const QString &id, QSize *size,
                                     const QSize &requestedSize)
{
    // image://objects/<id_of_object>

    // m_mutex.lock();

    QString decodedId = QUrl::fromPercentEncoding(id.toUtf8());

    QSharedPointer<SystemObject> obj;
    bool found = false;
    for (int i=0; i<m_objects.length(); i++) {
        if (decodedId == m_objects.at(i)->id()) {
            obj = m_objects.at(i);
            found = true;
            break;
        }
    }

    if (!found) {
        qDebug() << "Couldn't find object by id" << decodedId;
        // m_mutex.unlock();
        return QImage();
    }

    if (!obj->type()->ofContentType("image")) {
        qDebug() << "No picture for object:" << *obj;
        // m_mutex.unlock();
        return QImage();
    }

    QImage ret;
    qDebug() << "Trying to load" << obj->type()->subType()
             << "from length" << obj->payload().length() << "buffer";

    bool success = ret.loadFromData(obj->payload(), obj->type()->subType().toUtf8());
    qDebug() << "Succeeded:" << success;

    if (success && requestedSize.isValid()) {
        size->setHeight(ret.height());
        size->setWidth(ret.width());

        ret.scaledToHeight(requestedSize.height());
        ret.scaledToWidth(requestedSize.width());
    }

    // m_mutex.unlock();

    return ret;
}
