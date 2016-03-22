#ifndef QRADLINEEDITENTER_H
#define QRADLINEEDITENTER_H

#include <QLineEdit>

class QRadLineEditEnter : public QLineEdit
{
    Q_OBJECT
public:
    QRadLineEditEnter(QWidget *parent = 0);
public slots:
    void returnPressed();

};

#endif // QRADLINEEDITENTER_H
