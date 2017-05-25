#include "financierdelegates.h"

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
}:

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
        moneyPaid = DSMMoney::MoneyHumanForm2(DSMMoney::Round(text.toString().mid(1).toDouble()));
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


