#ifndef QRADDELEGATE_H
#define QRADDELEGATE_H

#include <QItemDelegate>
#include <QObject>

class QRadDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    QRadDelegate(QObject *parent=NULL);
    virtual QVariant FormatValue(QVariant value) const =0;
};

#endif // QRADDELEGATE_H
