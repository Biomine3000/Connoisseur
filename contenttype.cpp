#include <QDebug>
#include <QString>
#include <QStringList>

#include "contenttype.h"

ContentType::ContentType(QObject *parent) :
    QObject(parent)
{
}

ContentType::~ContentType()
{
    // qDebug() << "~ContentType called!";
}

ContentType::ContentType(const ContentType &other, QObject *parent) :
    QObject(parent),
    m_type(other.m_type), m_subType(other.m_subType),
    m_metadata(other.m_metadata)
{
}

ContentType::ContentType(const QString typeString, QObject *parent) :
    QObject(parent)
{
    // QString typePart;

    if (typeString.contains(";")) {
        // typePart = typeString.section(";", 0);

        // TODO: parse metadata!
        throw 1;
    }

    QStringList parts = typeString.split('/');
    if (parts.length() == 2) {
        m_type = parts[0];
        m_subType = parts[1];
    } else {
        m_type = "no_type";
        m_subType = "no_type";
    }
}

bool ContentType::ofContentType(const QString &type) const
{
    if (m_type == type)
        return true;
    return false;
}

bool ContentType::ofType(const QString &contentType, const QString &subType) const
{
    if (m_type == contentType && m_subType == subType)
        return true;
    return false;
}

QString ContentType::contentType() const
{
    return m_type;
}

QString ContentType::subType() const
{
    return m_subType;
}

QMap<QString,QVariant> ContentType::metadata() const
{
    return m_metadata;
}

QString ContentType::toString() const
{
    return QString("%1 / %2").arg(m_type, m_subType);
}
