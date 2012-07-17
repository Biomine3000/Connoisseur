#ifndef CONTENTTYPE_H
#define CONTENTTYPE_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QVariant>

class ContentType : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariant m_type READ contentType)
    Q_PROPERTY(QVariant m_subtType READ subType)
    Q_PROPERTY(QVariantMap m_metadata READ metadata)
public:
    explicit ContentType(QObject *parent = 0);
    ~ContentType();
    ContentType(const QString typeString, QObject *parent = 0);
    ContentType(const ContentType &other, QObject *parent = 0);

    Q_INVOKABLE bool ofContentType(const QString &type) const;
    Q_INVOKABLE bool ofType(const QString &contentType, const QString &subType) const;

    static ContentType* NoType;

    Q_INVOKABLE QString contentType() const;
    Q_INVOKABLE QString subType() const;
    Q_INVOKABLE QMap<QString,QVariant> metadata() const;

    QString toString() const;
signals:
    
public slots:

private:
    QString m_type;
    QString m_subType;
    QMap<QString,QVariant> m_metadata;
};

typedef ContentType * ContentTypePtr;
Q_DECLARE_METATYPE(ContentTypePtr)
Q_DECLARE_METATYPE(ContentType)

#endif // CONTENTTYPE_H
