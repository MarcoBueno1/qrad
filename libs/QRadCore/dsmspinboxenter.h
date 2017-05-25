#ifndef DSMSPINBOXENTER_H
#define DSMSPINBOXENTER_H

#include <QSpinBox>
#include <QEvent>
#include <QKeyEvent>
#include "dsmshared.h"

class DSMSpinBoxEnter : public QSpinBox
{
public:
    DSMSpinBoxEnter(QWidget *parent);
    void keyPressEvent(QKeyEvent * event);
};

#endif // DSMSPINBOXENTER_H
