#ifndef COLUMN2DELEGATE_H
#define COLUMN2DELEGATE_H

#include <QItemDelegate>
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



class ColumnMoney2 : public QItemDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};


class ColumnSCombo : public QItemDelegate
{
    Q_OBJECT
public:
    ColumnSCombo(QObject *parent = 0, SComboBox* cmb = NULL);
    QWidget* createEditor( QWidget *parent, 
						   const QStyleOptionViewItem &option, 
						   const QModelIndex &index) ;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	
	void setCombo(SComboBox* combo);
private:
    SComboBox* m_combobox;
   	
};

class ColumnCombo : public QItemDelegate
{
    Q_OBJECT
public:
    ColumnCombo(QObject *parent = 0, QComboBox* cmb = NULL);
    QWidget* createEditor( QWidget *parent, 
						   const QStyleOptionViewItem &option, 
						   const QModelIndex &index) ;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	
	void setCombo(QComboBox* combo);
private:
    QComboBox* m_combobox;
   	
};

class ColumnSpin : public QItemDelegate
{
    Q_OBJECT
public:
    ColumnSpin(QObject *parent = 0, QSpinBox* cmb = NULL);
    QWidget* createEditor( QWidget *parent, 
						   const QStyleOptionViewItem &option, 
						   const QModelIndex &index) ;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	
	void setSpin(QSpinBox* spin);
private:
    QSpinBox* m_spinbox;
   	
};


class ColumnPercent : public QItemDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};

class ColumnCenter : public QItemDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};

class ColumnLeft : public QItemDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};

class ColumnBalance : public QItemDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};

class ColumnRight : public QItemDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};


class ColumnCPF : public QItemDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};

class ColumnBoolIcon : public QItemDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};

class ColumnBool : public QItemDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};

class ColumnCNPJ : public QItemDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};

class ColumnPhone : public QItemDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};

class ColumnDateTime : public QItemDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};

class ColumnDate : public QItemDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};

class ColumnDateTimeNull : public QItemDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};

class ColumnObs : public QItemDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};

class ColumnMoney : public QItemDelegate
{
    Q_OBJECT
public:
    void paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
};

class ColumnCheckBox : public QItemDelegate
{
    Q_OBJECT

public:
    ColumnCheckBox(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool eventFilter(QObject *object, QEvent *event);

};


#endif // COLUMN2DELEGATE_H
