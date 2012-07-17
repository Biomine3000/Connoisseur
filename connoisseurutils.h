#ifndef CONNOISSEURUTILS_H
#define CONNOISSEURUTILS_H

#include <QObject>

class ConnoisseurUtils : public QObject
{
    Q_OBJECT
public:
    explicit ConnoisseurUtils(QObject *parent = 0);
    // ConnoisseurUtils(ObjectFlowModel *model, QObject *parent = 0);
    ~ConnoisseurUtils();
    
signals:
    
public slots:
    // payloadToString(QString &objectId);
    // eventPrefix(const QString &objectId);
    // eventSuffix(const QString &objectId);
    
};

#endif // CONNOISSEURUTILS_H
