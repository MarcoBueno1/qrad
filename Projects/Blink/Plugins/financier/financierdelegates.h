#ifndef _FINANCIER_DELEGATES_
#define _FINANCIER_DELEGATES_
#include "qraddelegate.h"
#include "accounttypemodel.h"

class ColumnAccountType : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};

class ColumnFinancierDescription: public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};

class ColumnFinancierDate: public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};

class ColumnFinancierMoney : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};


class ColumnFinancierAccountPaid : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};

class ColumnFinancierAccountOperation : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};
class ColumnEmail : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};

class ColumnTktStatus : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};

class ColumnTktType : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};
class ColumnDateLate : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};

class ColumnDateTicketNull : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};

class ColumnMoneyTktPaid : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};

class ColumnApTower : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};


#endif
