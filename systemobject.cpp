#include <assert.h>
#include <algorithm>

#include <QObject>
#include <QDebug>
#include <QUuid>

#include <QJson/Parser>
#include <QJson/Serializer>

#include "systemobject.h"
#include "contenttype.h"

SystemObject::SystemObject(QObject *parent) :
    QObject(parent)
{
}

SystemObject::SystemObject(QMap<QString,QVariant> metadata, QByteArray payload,
                           QObject *parent) :
    QObject(parent),
    m_metadata(metadata), m_payload(payload)
{
    if (m_metadata.contains(QString("id")))
        m_id = m_metadata["id"].toString();
    else {
        m_id = QUuid::createUuid().toString();
        m_metadata["id"] = m_id;
    }

    if (m_metadata.contains(QString("type")))
        m_type = new ContentType(m_metadata["type"].toString(), this);
    else
        m_type = new ContentType("", this);
}

SystemObject::SystemObject(QAbstractSocket* socket, QObject *parent) :
    QObject(parent)
{
    // qDebug() << "Reading from socket...";
    const QByteArray metadata_array = SystemObject::read_until_nul(socket);

    QJson::Parser parser;

    bool ok;
    QVariant result = parser.parse(metadata_array, &ok);

    if (!ok) {
        QString s = QString::fromUtf8(metadata_array);
        qDebug() << "Tried to parse JSON from:" << s;
        throw SystemObject::invalid_object;
    }

    QByteArray payload;
    QMap<QString, QVariant> metadata = result.toMap();
    // qDebug() << "Keys in metadata:" << metadata.count();

    if (metadata.contains(QString("size")) &&
            metadata["size"].canConvert(QVariant::Int) &&
            metadata["size"].toInt() > 0) {

        qint64 size = metadata["size"].toInt();
        // qDebug() << "Attempting to read payload of size" << size;

        qint64 read_total = 0;
        // qint64 int_max = (qint64)std::numeric_limits<int>::max();
        char buf[size];
        while (read_total < size) {
            int read_bytes = socket->read(buf, std::min((qint64)1024, size - read_total));
            // qDebug() << "read_bytes" << read_bytes;
            if (read_bytes == -1) {
                qDebug() << "Got -1 while reading payload of size" << size;
                throw SystemObject::cannot_read_payload;
            } else if (read_bytes == 0) {
                socket->waitForReadyRead(500);
            } else if (read_bytes > 0) {
                payload.append(buf, read_bytes);
                read_total += read_bytes;
            }
        }

        // qDebug("Done reading payload!");
    } else {
        // qDebug("Not reading payload!");
    }

    m_metadata = metadata;
    m_payload = payload;

    if (m_metadata.contains(QString("id")))
        m_id = m_metadata["id"].toString();
    else {
        m_id = QUuid::createUuid().toString();
        m_metadata["id"] = m_id;
        qDebug() << "Assigned id" << m_metadata["id"];
    }
    if (m_metadata.contains(QString("type")))
        m_type = new ContentType(m_metadata["type"].toString(), this);
    else
        m_type = new ContentType("");
}

SystemObject::SystemObject(QString payloadText, QObject *parent) :
    QObject(parent)
{
    m_payload = payloadText.toUtf8();
    m_id = QUuid::createUuid().toString();
    m_type = new ContentType("text/plain", this);

    m_metadata["type"] = m_type->toString();
    m_metadata["size"] = m_payload.length();
    m_metadata["id"] = m_id;
}

SystemObject::SystemObject(const SystemObject &other, QObject *parent) :
    QObject(parent),
    m_metadata(other.m_metadata), m_payload(other.m_payload),
    m_type(other.m_type), m_id(other.m_id)
{
}

SystemObject::~SystemObject()
{
    // qDebug() << "~SystemObject called!";
    // m_type->deleteLater(); // deleted by Qt (this is the parent object)
}

QByteArray SystemObject::read_until_nul(QAbstractSocket* socket)
{
    // qDebug() << "Reading until nul...";
    QByteArray ret;
    ret.reserve(500);
    char read_char = ' ';
    while (true) {

        bool read_success = socket->getChar(&read_char);

        // qDebug() << "Read char" << read_char << " (code:" << (int)read_char << ")";

        if (read_success) {
            if (read_char == '\0')
                break;

            ret += read_char;
        } else {
            // qDebug() << "waitForReadyRead";
            socket->waitForReadyRead(500);
        }
    }

    // qDebug() << "Metadata buffer length: " << ret.length();

    return ret;
}

QMap<QString,QVariant> SystemObject::metadata()
{
    return m_metadata;
}

QByteArray SystemObject::payload()
{
    return m_payload;
}

ContentType* SystemObject::type()
{
    return m_type;
}

QString SystemObject::id()
{
    return m_id;
}

QString SystemObject::toString() const
{
    if (m_metadata.contains(QString("event"))) {
        QString format("SystemObject event: '%1', content-type: '%2', id: '%3'");
        return format.arg(m_metadata["event"].toString(), m_type->toString(), m_id);
    } else {
        QString format("SystemObject content-type: '%2', id: '%3'");
        return format.arg(m_type->toString(), m_id);
    }
}

QByteArray SystemObject::toByteArray() const
{
    QJson::Serializer serializer;
    QByteArray serialized = serializer.serialize(m_metadata);
    serialized += '\0';

    if (m_metadata.contains(QString("size")))
        assert(m_payload.length() == m_metadata["size"]);

    if (m_payload.length() > 0)
        serialized += serialized;

    return serialized;
}

QDataStream &operator<<(QDataStream &out, const SystemObject &obj)
{
    QByteArray buf = obj.toByteArray();
    if (out.writeRawData(buf, buf.length()) == -1)
        throw SystemObject::cannot_write_object;
    qDebug() << "Wrote!";
    return out;
}

QDebug operator<<(QDebug dbg, const SystemObject &obj)
{
    // http://doc.qt.nokia.com/4.7-snapshot/custom-types.html
    dbg.nospace() << obj.toString();
    return dbg.maybeSpace();
}
