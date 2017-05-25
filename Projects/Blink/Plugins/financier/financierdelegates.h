#ifndef _FINANCIER_DELEGATES_
#define _FINANCIER_DELEGATES_
#include <QItemDelegate>
#include "accounttypemodel.h"

class ColumnAccountType : public QItemDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};

class ColumnFinancierDescription: public QItemDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};

class ColumnFinancierDate: public QItemDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};

class ColumnFinancierMoney : public QItemDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};


class ColumnFinancierAccountPaid : public QItemDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};


#endif
