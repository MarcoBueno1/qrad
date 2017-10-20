#ifndef QRadCalendar_H
#define QRadCalendar_H

#include <QCalendarWidget>
#include <QEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QTextCharFormat>
#include "qradshared.h"

class QRadCalendar : public QCalendarWidget
{
public:
    QRadCalendar(QWidget *parent);

    void    setReserveDate(QList<QDate> dates);
    void    ClearFormat(int year, int month);
};

#endif // QRadCalendar_H
