#ifndef DSMDATEEDITENTER_H
#define DSMDATEEDITENTER_H

#include <QDateEdit>
#include <QKeyEvent>
#include "qradshared.h"

class DSMDateEditEnter : public QDateEdit
{
public:
    DSMDateEditEnter(QWidget *parent);
    void keyPressEvent(QKeyEvent * event);
};

#endif // DSMDATEEDITENTER_H
