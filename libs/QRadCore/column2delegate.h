#ifndef COLUMN2DELEGATE_H
#define COLUMN2DELEGATE_H

#include <QDate>
#include <QPainter>
#include <QMessageBox>
#include <QKeyEvent>
#include <QTableView>
#include <QTextDocument>
#include <QTextLayout>
#include <QLabel>
#include <QTextBlock>
#include <QAbstractTextDocumentLayout>
#include <QStyledItemDelegate>
#include <QHBoxLayout>
#include <QSpinBox>
#include "qradshared.h"
#include "scombobox.h"
#include "qraddelegate.h"



class ColumnMoney2 : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;

    QVariant FormatValue(QVariant value) const;

};


class ColumnSCombo : public QRadDelegate
{
    Q_OBJECT
public:
    ColumnSCombo(QObject *parent = 0, SComboBox* cmb = NULL);
    QWidget* createEditor( QWidget *parent, 
						   const QStyleOptionViewItem &option, 
						   const QModelIndex &index) ;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	
	void setCombo(SComboBox* combo);

    QVariant FormatValue(QVariant value) const;

private:
    SComboBox* m_combobox;
   	
};

class ColumnCombo : public QRadDelegate
{
    Q_OBJECT
public:
    ColumnCombo(QObject *parent = 0, QComboBox* cmb = NULL);
    QWidget* createEditor( QWidget *parent, 
						   const QStyleOptionViewItem &option, 
						   const QModelIndex &index) ;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	
	void setCombo(QComboBox* combo);

    QVariant FormatValue(QVariant value) const;

private:
    QComboBox* m_combobox;
   	
};

class ColumnSpin : public QRadDelegate
{
    Q_OBJECT
public:
    ColumnSpin(QObject *parent = 0, QSpinBox* cmb = NULL);
    QWidget* createEditor( QWidget *parent, 
						   const QStyleOptionViewItem &option, 
						   const QModelIndex &index) ;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	
	void setSpin(QSpinBox* spin);

    QVariant FormatValue(QVariant value) const;

private:
    QSpinBox* m_spinbox;
   	
};


class ColumnPercent : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;

    QVariant FormatValue(QVariant value) const;

};

class ColumnCenter : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;

    QVariant FormatValue(QVariant value) const;

};

class ColumnLeft : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};

class ColumnBalance : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};

class ColumnRight : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};


class ColumnCPF : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};

class ColumnBoolIcon : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};

class ColumnBool : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};

class ColumnCNPJ : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};

class ColumnPhone : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};

class ColumnDateTime : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};

class ColumnDate : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};

class ColumnDateTimeNull : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};
class ColumnTimeNull : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};


class ColumnObs : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};

class ColumnMoney : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};

class ColumnCheckBox : public QRadDelegate
{
    Q_OBJECT

public:
    ColumnCheckBox(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool eventFilter(QObject *object, QEvent *event);
    QVariant FormatValue(QVariant value) const;


};

class ColumnImageMail : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;

    QVariant FormatValue(QVariant value) const;
};

class ColumnRetBankAndPaid : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};

class ColumnReserve : public QRadDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
    QVariant FormatValue(QVariant value) const;

};


#endif // COLUMN2DELEGATE_H
