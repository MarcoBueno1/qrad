#include "financierdelegates.h"
#include "qradshared.h"
#include "qradmoney.h"
#include "ticket.h"

#include <QColor>
#include <QPainter>
#include <QStyle>

#define BG_FIN_COLOR_GREEN      QColor(100, 240, 100)
#define BG_FIN_COLOR_RED        QColor(240, 100, 100)
#define BG_FIN_COLOR_ORANGE     QColor(240, 125, 40)
#define BG_FIN_COLOR_YELLOW     QColor(255, 255, 0)
#define BG_FIN_COLOR_PURPLE     QColor(85, 85, 255)
#define BG_FIN_COLOR_BLACK      QColor(0, 0, 0)
#define BG_FIN_COLOR_BLUE       QColor(0, 85, 255)


#define FORMAT_FINACIER { \
if (text.toString().mid(0,1) == "P") \
{\
 if (option.state & QStyle::State_Enabled)\
 {\
  painter->fillRect(option.rect, BG_FIN_COLOR_GREEN);\
 }\
}\
else if (text.toString().mid(0,1) == "V")\
{\
 if (option.state & QStyle::State_Enabled)\
 {\
  painter->fillRect(option.rect, BG_FIN_COLOR_RED);\
 }\
}\
else if (text.toString().mid(0,1) == "H")\
{\
 if (option.state & QStyle::State_Enabled)\
 {\
  painter->fillRect(option.rect, BG_FIN_COLOR_ORANGE);\
 }\
}\
}


void ColumnAccountType::paint(QPainter *painter,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString accountType;

    if (text.toInt() == AccountTypeToPay)
    {
        accountType = QString::fromUtf8("CONTAS A PAGAR");
    }
    else if (text.toInt() == AccountTypeToReceive)
    {
        accountType = QString::fromUtf8("CONTAS A RECEBER");
    }
    else
    {
        accountType = QString::fromUtf8("CONTAS A PAGAR/RECEBER");
    }

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;
    drawDisplay(painter, myOption, myOption.rect, accountType);
    drawFocus(painter, myOption, myOption.rect);
}

void ColumnFinancierDescription::paint(QPainter *painter,
                                       const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;

    FORMAT_FINACIER;

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;

    drawDisplay(painter, myOption, myOption.rect, text.toString().mid(1));
    drawFocus(painter, myOption, myOption.rect);
}

void ColumnFinancierDate::paint(QPainter *painter,
                                const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QDate   date;
    QString dateStr;

    date.setDate(text.toString().mid(1,4).toInt(), text.toString().mid(6,2).toInt(), text.toString().mid(9,2).toInt());

    FORMAT_FINACIER;

    if (date.daysTo(QDate(2000,1,1)) == 0)
    {
        dateStr = "-";
    }
    else
    {
        dateStr = date.toString(FMT_DATE);
    }

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;

    drawDisplay(painter, myOption, myOption.rect, dateStr);
    drawFocus(painter, myOption, myOption.rect);
}

void ColumnFinancierMoney::paint(QPainter *painter,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString moneyPaid;

    FORMAT_FINACIER;

    if (text.toString().mid(1).toDouble() == 0)
    {
        moneyPaid = "-";
        myOption.displayAlignment = (Qt::AlignHCenter | Qt::AlignVCenter);
    }
    else
    {
        moneyPaid = QRadMoney::MoneyHumanForm2(QRadMoney::Round(text.toString().mid(1).toDouble()));
        myOption.displayAlignment = (Qt::AlignRight | Qt::AlignVCenter);
    }

    /* Como sera o alinhamento */

    drawDisplay(painter, myOption, myOption.rect, moneyPaid);
    drawFocus(painter, myOption, myOption.rect);
}

void ColumnFinancierAccountPaid::paint(QPainter *painter,
                                       const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString paid;

    FORMAT_FINACIER;

    if (text.toString().mid(1) == "T")
    {
        paid = QString::fromUtf8("PAGO");
    }
    else
    {
        paid = QString::fromUtf8("EM ABERTO");
    }

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;
    drawDisplay(painter, myOption, myOption.rect, paid);
    drawFocus(painter, myOption, myOption.rect);
}

void ColumnFinancierAccountOperation::paint(QPainter *painter,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString operation;

    if (text.toInt() == AccountOpCreate)
    {
        operation = QString::fromUtf8("CRIAÇÃO");
    }
    else if (text.toInt() == AccountOpEdit)
    {
        operation = QString::fromUtf8("EDIÇÃO");
    }
    else if (text.toInt() == AccountOpRemove)
    {
        operation = QString::fromUtf8("REMOÇÃO");
    }
    else if (text.toInt() == AccountOpPaid)
    {
        operation = QString::fromUtf8("QUITAÇÃO");
    }
    else
    {
        operation = QString::fromUtf8("ESTORNO");
    }

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;
    drawDisplay(painter, myOption, myOption.rect, operation);
    drawFocus(painter, myOption, myOption.rect);
}


void ColumnEmail::paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString status;

    switch (text.toInt())
    {
    case -1:
            status = QString::fromUtf8("Desabilitado");
            break;
    case 0:
            status = QString::fromUtf8("Pendente");
            break;
    case 1:
            status = QString::fromUtf8("Enviado");
            break;
    }

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;
    drawDisplay(painter, myOption, myOption.rect, status);
    drawFocus(painter, myOption, myOption.rect);
}

void ColumnTktStatus::paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString status;


    switch (text.toInt())
    {
    /*
     *   stCreated,
  stBuiltShipp,
  stRegistered,
  stPaid,
  stModified,
  stDateModified,
  stValueModified,
  stDateValueModified
     */
    case stCreated:
         status = QString::fromUtf8("Criado");
         break;
    case stBuiltShipp:
         status = QString::fromUtf8("Em Remessa");
         break;
    case stRegistered:
        status = QString::fromUtf8("Registrado");
        break;
    case stPaid:
         status = QString::fromUtf8("Pago");
         painter->fillRect(option.rect, BG_FIN_COLOR_GREEN);
         break;
    case stModified:
        status = QString::fromUtf8("Modificado");
        break;
    case stValueModified:
        status = QString::fromUtf8("Valor Alterado");
        break;
    case stDateValueModified:
        status = QString::fromUtf8("Valor e Data Alterados");
        break;
    }

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;
    drawDisplay(painter, myOption, myOption.rect, status);
    drawFocus(painter, myOption, myOption.rect);
}


void ColumnDateLate::paint(QPainter *painter,
                         const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
 QVariant text = index.model()->data(index, Qt::DisplayRole);
 int status = index.model()->data(index.model()->index(index.row(),10)).toInt();
 QStyleOptionViewItem myOption = option;

 QDate date = text.toDate();
 if(( QDate::currentDate() > date.addDays(3)) && (stPaid!= status))
 {
      painter->fillRect(option.rect, BG_FIN_COLOR_RED);
 }

 /* Como sera o alinhamento */
 myOption.displayAlignment = Qt::AlignCenter;
 drawDisplay(painter, myOption, myOption.rect, text.toDate().toString(FMT_DATE));
 drawFocus(painter, myOption, myOption.rect);
}


void ColumnTktType::paint(QPainter *painter,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
  QVariant text = index.model()->data(index, Qt::DisplayRole);
  QStyleOptionViewItem myOption = option;
  QString status;


  switch (text.toInt())
  {
  case 0:
       status = QString::fromUtf8("Tx Conominial");
       break;
  case 1:
       status = QString::fromUtf8("Tx Extra");
       break;
  }

  /* Como sera o alinhamento */
  myOption.displayAlignment = Qt::AlignCenter;
  drawDisplay(painter, myOption, myOption.rect, status);
  drawFocus(painter, myOption, myOption.rect);
}

void ColumnDateTicketNull::paint(QPainter *painter,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    QDate date = index.model()->data(index, Qt::DisplayRole).toDate();
//    QTime time = index.model()->data(index.model()->index(index.row(),4)).toTime();

    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString strText= text.toString(); //toDate().toString(QString("%1").arg(FMT_DATE));


    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;

    if( date == QDate(2000,1,1) )
    {
       //painter->fillRect(option.rect, BG_COLOR_YELLOW);
        strText = "PENDENTE";
    }

    drawDisplay(painter, myOption, myOption.rect, strText);
    drawFocus(painter, myOption, myOption.rect);
}
