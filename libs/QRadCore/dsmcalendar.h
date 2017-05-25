#ifndef DSMCALENDAR_H
#define DSMCALENDAR_H

#include <QCalendarWidget>
#include <QEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QTextCharFormat>
#include "dsmshared.h"

class DSMCalendar : public QCalendarWidget
{
public:
    DSMCalendar(QWidget *parent);

    void    setAccountDate(DSM_AccountCalendar_t accountCalendarItem);
    void    ClearFormat(int year, int month);
};

#endif // DSMCALENDAR_H
