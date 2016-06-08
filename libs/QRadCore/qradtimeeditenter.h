#ifndef QRadTIMEEDITENTER_H
#define QRadTIMEEDITENTER_H

#include <QTimeEdit>
#include <QKeyEvent>
#include "dsmshared.h"

class QRadTimeEditEnter : public QTimeEdit
{
public:
    QRadTimeEditEnter(QWidget *parent);
    void keyPressEvent(QKeyEvent * event);
};

#endif // QRadTIMEEDITENTER_H
