#include "qradcalendar.h"
#include "qraddebug.h"
#include <QMap>

QRadCalendar::QRadCalendar(QWidget *parent) : QCalendarWidget(parent)
{
//    this->setStyleSheet("selection-background-color: rgba(102, 176, 239, 75%); selection-color: rgb(255, 255, 255);");

    this->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
//    this->setHorizontalHeaderFormat(QCalendarWidget::SingleLetterDayNames);
  //  this->setGridVisible(true);
}

void QRadCalendar::setReserveDate(QList<QDate> dates)
{
    QTextCharFormat format;


    for( int i = 0; i < dates.count(); i++ )
    {

        format = this->dateTextFormat().find(dates.at(i)).value();
        debug_message("Encontrada data: %s\n", dates.at(i).toString(FMT_DATE).toLatin1().data());
        format.setFontUnderline(true);
        format.setToolTip("Dia Com Reservas");
        format.setForeground(BG_FIN_COLOR_BLUE);
        debug_message("Setou Foreground\n");

        this->setDateTextFormat(dates.at(i), format);
        debug_message("depois de setDateTextFormat\n");
    }

  //  QString         descToPay = "",
  //                  descToReceive = "",
  //                  description = "";

//    if (!accountCalendarItem.descriptionToPay.isEmpty())
//    {
//        format.setBackground(BG_FIN_COLOR_RED);
//    }

//    if (!accountCalendarItem.descriptionToReceive.isEmpty())
//    {
//        format.setFontUnderline(true);
//    }

//    foreach(QString desc, accountCalendarItem.descriptionToPay)
//    {
//        if (!descToPay.isEmpty())
//        {
//            descToPay += "\n";
//        }
//        descToPay += desc;
//    }

//    foreach(QString desc, accountCalendarItem.descriptionToReceive)
//    {
//        if (!descToReceive.isEmpty())
//        {
//            descToReceive += "\n";
//        }
//        descToReceive += desc;
//    }

//    if (!descToPay.isEmpty())
//    {
//        description = "Contas a Pagar:\n" + descToPay;
//    }
//    if (!descToReceive.isEmpty())
//    {
//        if (!description.isEmpty())
//        {
//            description += "\n";
//        }
//        description += "Contas a Receber:\n" + descToReceive;
//    }

//    format.setToolTip("Dia Com Reservas");
//    format.setForeground(Qt::black);

//    this->setDateTextFormat(accountCalendarItem.date, format);
}

void QRadCalendar::ClearFormat(int year, int month)
{
//    QTextCharFormat format;
    QDate date = QDate(year,month,1);

    for( int i = 0; i < date.daysInMonth(); i++ )
        setDateTextFormat( QDate(year,month,i+1) , QTextCharFormat() );

/*
    this->
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
*/
}
