#include "dsmcalendar.h"


DSMCalendar::DSMCalendar(QWidget *parent) : QCalendarWidget(parent)
{
    this->setStyleSheet("selection-background-color: rgba(102, 176, 239, 75%); selection-color: rgb(255, 255, 255);");

    this->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    this->setHorizontalHeaderFormat(QCalendarWidget::SingleLetterDayNames);
    this->setGridVisible(true);
}

void DSMCalendar::setAccountDate(DSM_AccountCalendar_t accountCalendarItem)
{
    QTextCharFormat format;
    QString         descToPay = "",
                    descToReceive = "",
                    description = "";

    if (!accountCalendarItem.descriptionToPay.isEmpty())
    {
        format.setBackground(BG_FIN_COLOR_RED);
    }

    if (!accountCalendarItem.descriptionToReceive.isEmpty())
    {
        format.setFontUnderline(true);
    }

    foreach(QString desc, accountCalendarItem.descriptionToPay)
    {
        if (!descToPay.isEmpty())
        {
            descToPay += "\n";
        }
        descToPay += desc;
    }

    foreach(QString desc, accountCalendarItem.descriptionToReceive)
    {
        if (!descToReceive.isEmpty())
        {
            descToReceive += "\n";
        }
        descToReceive += desc;
    }

    if (!descToPay.isEmpty())
    {
        description = "Contas a Pagar:\n" + descToPay;
    }
    if (!descToReceive.isEmpty())
    {
        if (!description.isEmpty())
        {
            description += "\n";
        }
        description += "Contas a Receber:\n" + descToReceive;
    }

    format.setToolTip(description);
    format.setForeground(Qt::black);

    this->setDateTextFormat(accountCalendarItem.date, format);
}

void DSMCalendar::ClearFormat(int year, int month)
{
    QTextCharFormat format;
    QDate date;

    format.setBackground(Qt::white);
    format.setFontUnderline(false);

    for (int index = 0; index < 31; index++)
    {
        date.setDate(year, month, index + 1);
        if (date.isValid())
        {
            this->setDateTextFormat(date, format);
        }
    }
}
