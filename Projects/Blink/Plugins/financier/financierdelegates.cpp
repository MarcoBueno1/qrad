#include "financierdelegates.h"
#include "qradshared.h"
#include "qradmoney.h"
#include "ticket.h"
#include "bankticket.h"

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


#define FIN_COLUMN_FORMAT 10


#define FORMAT_FINACIER { \
if (Format.toString().mid(0,1) == "P") \
{\
 if (option.state & QStyle::State_Enabled)\
 {\
  painter->fillRect(option.rect, BG_FIN_COLOR_GREEN);\
 }\
}\
else if (Format.toString().mid(0,1) == "V")\
{\
 if (option.state & QStyle::State_Enabled)\
 {\
  painter->fillRect(option.rect, BG_FIN_COLOR_RED);\
 }\
}\
else if (Format.toString().mid(0,1) == "H")\
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
    QString accountType = FormatValue(text).toString();


    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;
    drawDisplay(painter, myOption, myOption.rect, accountType);
    drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnAccountType::FormatValue(QVariant value) const
{
    QString accountType;
    if (value.toInt() == AccountTypeToPay)
    {
        accountType = QString::fromUtf8("CONTAS A PAGAR");
    }
    else if (value.toInt() == AccountTypeToReceive)
    {
        accountType = QString::fromUtf8("CONTAS A RECEBER");
    }
    else
    {
        accountType = QString::fromUtf8("CONTAS A PAGAR/RECEBER");
    }

    return accountType;
}


void ColumnFinancierDescription::paint(QPainter *painter,
                                       const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QVariant Format = index.model()->index(index.row(),FIN_COLUMN_FORMAT).data(Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;

    FORMAT_FINACIER;

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;

    drawDisplay(painter, myOption, myOption.rect, FormatValue(text).toString());
    drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnFinancierDescription::FormatValue(QVariant value) const
{
    return value.toString();
}


void ColumnFinancierDate::paint(QPainter *painter,
                                const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QVariant Format = index.model()->index(index.row(),FIN_COLUMN_FORMAT).data(Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString dateStr = FormatValue(text).toString();

    FORMAT_FINACIER;

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;

    drawDisplay(painter, myOption, myOption.rect, dateStr);
    drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnFinancierDate::FormatValue(QVariant value) const
{
    QDate date;
    QString dateStr;

    date = value.toDate();

    if (date.daysTo(QDate(2000,1,1)) == 0)
    {
        dateStr = "-";
    }
    else
    {
        dateStr = date.toString(FMT_DATE);
    }
    return dateStr;
}

void ColumnFinancierMoney::paint(QPainter *painter,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QVariant Format = index.model()->index(index.row(),FIN_COLUMN_FORMAT).data(Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString moneyPaid = FormatValue(text).toString();

    FORMAT_FINACIER;

    if (moneyPaid == "-" )
    {
        myOption.displayAlignment = (Qt::AlignHCenter | Qt::AlignVCenter);
    }
    else
    {
        myOption.displayAlignment = (Qt::AlignRight | Qt::AlignVCenter);
    }

    /* Como sera o alinhamento */

    drawDisplay(painter, myOption, myOption.rect, moneyPaid);
    drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnFinancierMoney::FormatValue(QVariant value) const
{
    QString moneyPaid;
    if (value.toString().toDouble() == 0)
    {
        moneyPaid = "-";
    }
    else
    {
        moneyPaid = QRadMoney::MoneyHumanForm2(QRadMoney::Round(value.toString().toDouble()));
    }

    return moneyPaid;
}


void ColumnFinancierAccountPaid::paint(QPainter *painter,
                                       const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QVariant Format = index.model()->index(index.row(),FIN_COLUMN_FORMAT).data(Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString paid = FormatValue(text).toString();

    FORMAT_FINACIER;

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;
    drawDisplay(painter, myOption, myOption.rect, paid);
    drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnFinancierAccountPaid::FormatValue(QVariant value) const
{
    QString paid;
    if (value.toString().mid(0) == "T")
    {
        paid = QString::fromUtf8("PAGO");
    }
    else
    {
        paid = QString::fromUtf8("EM ABERTO");
    }

    return paid;
}

void ColumnFinancierAccountOperation::paint(QPainter *painter,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString operation = FormatValue(text).toString();

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;
    drawDisplay(painter, myOption, myOption.rect, operation);
    drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnFinancierAccountOperation::FormatValue(QVariant value) const
{
    QString operation;
    if (value.toInt() == AccountOpCreate)
    {
        operation = QString::fromUtf8("CRIAÇÃO");
    }
    else if (value.toInt() == AccountOpEdit)
    {
        operation = QString::fromUtf8("EDIÇÃO");
    }
    else if (value.toInt() == AccountOpRemove)
    {
        operation = QString::fromUtf8("REMOÇÃO");
    }
    else if (value.toInt() == AccountOpPaid)
    {
        operation = QString::fromUtf8("QUITAÇÃO");
    }
    else
    {
        operation = QString::fromUtf8("ESTORNO");
    }

    return operation;
}



void ColumnEmail::paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString status = FormatValue(text).toString();

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;
    drawDisplay(painter, myOption, myOption.rect, status);
    drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnEmail::FormatValue(QVariant value) const
{
    QString status;
    switch (value.toInt())
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

    return status;
}

void ColumnTktStatus::paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString status = FormatValue(text).toString();


    switch (text.toInt())
    {
      case stPaid:
         painter->fillRect(option.rect, BG_FIN_COLOR_GREEN);
         break;
      default:
         break;
    }

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;
    drawDisplay(painter, myOption, myOption.rect, status);
    drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnTktStatus::FormatValue(QVariant value) const
{
    QString status;
    switch (value.toInt())
    {
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
    return status;
}


void ColumnDateLate::paint(QPainter *painter,
                         const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
 QVariant text = index.model()->data(index, Qt::DisplayRole);
 int status = index.model()->data(index.model()->index(index.row(),10)).toInt();
 QStyleOptionViewItem myOption = option;

 QDate date = text.toDate();
 if(( QDate::currentDate() > date.addDays(1)) && (stPaid!= status))
 {
      painter->fillRect(option.rect, BG_FIN_COLOR_RED);
 }

 /* Como sera o alinhamento */
 myOption.displayAlignment = Qt::AlignCenter;
 drawDisplay(painter, myOption, myOption.rect, FormatValue(text).toString());
 drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnDateLate::FormatValue(QVariant value) const
{
  return value.toDate().toString(FMT_DATE);
}


void ColumnTktType::paint(QPainter *painter,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
  QVariant text = index.model()->data(index, Qt::DisplayRole);
  QStyleOptionViewItem myOption = option;
  QString status = FormatValue(text).toString();


  /* Como sera o alinhamento */
  myOption.displayAlignment = Qt::AlignCenter;
  drawDisplay(painter, myOption, myOption.rect, status);
  drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnTktType::FormatValue(QVariant value) const
{
    QString status;
    switch (value.toInt())
    {
    case 0:
         status = QString::fromUtf8("Tx Conominial");
         break;
    case 1:
         status = QString::fromUtf8("Tx Extra");
         break;
    }
    return status;
}

void ColumnDateTicketNull::paint(QPainter *painter,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
//    QTime time = index.model()->data(index.model()->index(index.row(),4)).toTime();

    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString strText= FormatValue(text).toString(); //toDate().toString(QString("%1").arg(FMT_DATE));


    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;


    drawDisplay(painter, myOption, myOption.rect, strText);
    drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnDateTicketNull::FormatValue(QVariant value) const
{
    QString strText;
    if( value.toDate() == QDate(2000,1,1) )
    {
       //painter->fillRect(option.rect, BG_COLOR_YELLOW);
        strText = "PENDENTE";
    }
    else
        strText = value.toDate().toString(FMT_DATE);

    return strText;
}
void ColumnMoneyTktPaid::paint(QPainter *painter,
                        const QStyleOptionViewItem &option,
                        const QModelIndex &index) const
{
    QVariant PaidValue = index.model()->data(index, Qt::DisplayRole);
    QVariant Value = index.sibling(index.row(),7).data();
    QVariant Discount = index.sibling(index.row(),16).data();

    QStyleOptionViewItem myOption = option;

    if( (Value.toString() != PaidValue.toString()) && ( Discount.toDouble() > 0.00 ) )
        if( PaidValue.toDouble() > 0.00 )
        {
            double DiscountR = Value.toDouble() - (Value.toDouble()/100*Discount.toDouble());
            if( (DiscountR-1) > PaidValue.toDouble())
                painter->fillRect(option.rect, BG_FIN_COLOR_RED);
        }

    /* Como sera o alinhamento */
    myOption.displayAlignment = (Qt::AlignRight | Qt::AlignVCenter);
    drawDisplay(painter, myOption, myOption.rect, FormatValue(PaidValue).toString());
    drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnMoneyTktPaid::FormatValue(QVariant value) const
{
   return QRadMoney::MoneyHumanForm2(value.toDouble());
}


void ColumnApTower::paint(QPainter *painter,
                        const QStyleOptionViewItem &option,
                        const QModelIndex &index) const
{
    QVariant Value = index.model()->data(index, Qt::DisplayRole);

    QStyleOptionViewItem myOption = option;

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;

    drawDisplay(painter, myOption, myOption.rect, FormatValue(Value).toString());
    drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnApTower::FormatValue(QVariant value) const
{
   QString Tower;
   QString text = value.toString();

   if( text.at(0) == '1' )
       Tower =  " O";
   else if( text.at(0) == '2' )
       Tower =  " M";
   else
       Tower =  " G";

   Tower =text.mid(1) + Tower;
   return Tower;
}


