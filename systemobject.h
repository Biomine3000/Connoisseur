#ifndef SYSTEMOBJECT_H
#define SYSTEMOBJECT_H

#include <QObject>
#include <QMetaType>
#include <QSharedPointer>
#include <QtNetwork/QAbstractSocket>
#include <QMap>
#include <QVariant>
#include <QString>
#include <QByteArray>

#include "contenttype.h"

class SystemObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QMap m_metadata READ metadata)
    Q_PROPERTY(QByteArray m_payload READ payload)
    Q_PROPERTY(ContentType* m_type READ type)
    Q_PROPERTY(QString m_id READ id)
public:
    SystemObject(QObject* parent=0);
    SystemObject(const SystemObject &other, QObject* parent=0);
    ~SystemObject();

    SystemObject(QMap<QString,QVariant> metadata, QByteArray payload,
                 QObject* parent=0);
    SystemObject(QAbstractSocket* socket, QObject* parent=0);
    SystemObject(QString payloadText, QObject* parent=0);

    // TODO: SystemObjectReadException with subclasses etc
    static const int invalid_object = 0;
    static const int cannot_read_payload = 0;
    static const int cannot_write_object = 0;

    QMap<QString,QVariant> metadata();
    QByteArray payload();
    ContentType *type();
    QString id();

    QString toString() const;
    QByteArray toByteArray() const;

private:
    QMap<QString,QVariant> m_metadata;
    QByteArray m_payload;

    ContentType *m_type;
    QString m_id;

    QByteArray read_until_nul(QAbstractSocket* socket);
};

QDebug operator<<(QDebug dbg, const SystemObject &obj);
QDataStream &operator<<(QDataStream &out, const SystemObject &obj);

// http://stackoverflow.com/a/5356733
typedef SystemObject * SystemObjectPtr;
Q_DECLARE_METATYPE(SystemObjectPtr)

typedef QSharedPointer<SystemObject> SystemObjectSharedPtr;
Q_DECLARE_METATYPE(SystemObjectSharedPtr)

Q_DECLARE_METATYPE(SystemObject)

#endif // SYSTEMOBJECT_H
