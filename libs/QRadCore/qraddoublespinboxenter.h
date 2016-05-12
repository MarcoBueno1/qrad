#ifndef QRadDOUBLESPINBOXENTER_H
#define QRadDOUBLESPINBOXENTER_H

#include <QDoubleSpinBox>
#include <QEvent>
#include <QKeyEvent>
#include "qradshared.h"

class QRadDoubleSpinBoxEnter : public QDoubleSpinBox
{
public:
    QRadDoubleSpinBoxEnter(QWidget *parent);
    void keyPressEvent(QKeyEvent * event);
};

#endif // QRadDOUBLESPINBOXENTER_H
