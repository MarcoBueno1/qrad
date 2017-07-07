﻿#include <QRegExp>
#include <QCheckBox>


#include "column2delegate.h"
#include "qradmoney.h"

#define BG_COLOR_YELLOW     QColor(255, 255, 0)

#define FORMAT(p, w, r) qSetPadChar(p) << qSetRealNumberPrecision(r) << qSetFieldWidth(w)

void ColumnMoney2::paint(QPainter *painter,
                         const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    QVariant val = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;

    myOption.displayAlignment = (Qt::AlignRight | Qt::AlignVCenter);
    drawDisplay(painter, myOption, myOption.rect, QRadMoney::MoneyHumanForm2(val.toDouble()));
    drawFocus(painter, myOption, myOption.rect);
}

ColumnSCombo::ColumnSCombo(QObject *parent, SComboBox* cmb)
    : QItemDelegate(parent)
{
	m_combobox = cmb;
	
	if( !cmb )
        m_combobox = new SComboBox;
}


QWidget* ColumnSCombo::createEditor( QWidget *parent, 
					   const QStyleOptionViewItem &option, 
					   const QModelIndex &index )
{
	Q_UNUSED(option);
	Q_UNUSED(index);
	
    m_combobox->setParent(parent);
	return m_combobox;   
}

void ColumnSCombo::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	SComboBox* comboBox = qobject_cast<SComboBox*>(editor);
 
    int id  = comboBox->model()->data(comboBox->model()->index(comboBox->currentIndex(), 0)).toInt();
	
	model->setData(index, id);
}

void ColumnSCombo::setCombo(SComboBox* combo)
{
	if( m_combobox )
		delete m_combobox;
	
	m_combobox = combo;
}

ColumnCombo::ColumnCombo(QObject *parent, QComboBox* cmb)
    : QItemDelegate(parent)
{
	m_combobox = cmb;
	
	if( !cmb )
        m_combobox = new QComboBox;
}

QWidget* ColumnCombo::createEditor( QWidget *parent, 
					   const QStyleOptionViewItem &option, 
					   const QModelIndex &index )
{
	Q_UNUSED(option);
	Q_UNUSED(index);
	
    m_combobox->setParent(parent);
	return m_combobox;   
}

void ColumnCombo::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
 
    int id  = comboBox->model()->data(comboBox->model()->index(comboBox->currentIndex(), 0)).toInt();
	
	model->setData(index, id);
}

void ColumnCombo::setCombo(QComboBox* combo)
{
	if( m_combobox )
		delete m_combobox;
	
	m_combobox = combo;
}


ColumnSpin::ColumnSpin(QObject *parent, QSpinBox* cmb)
    : QItemDelegate(parent)
{
	m_spinbox = cmb;
	
	if( !cmb )
        m_spinbox = new QSpinBox;
}

QWidget* ColumnSpin::createEditor( QWidget *parent, 
					   const QStyleOptionViewItem &option, 
					   const QModelIndex &index )
{
	Q_UNUSED(option);
	Q_UNUSED(index);
	
    m_spinbox->setParent(parent);
	return m_spinbox;   
}

void ColumnSpin::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor);
 
	model->setData(index, spinBox->value());
}

void ColumnSpin::setSpin(QSpinBox* spin)
{
	if( m_spinbox )
		delete m_spinbox;
	
	m_spinbox = spin;
}



void ColumnObs::paint(QPainter *painter,
                      const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;

    /* Como sera o alinhamento */
    myOption.displayAlignment = (Qt::AlignLeft | Qt::AlignVCenter);
    drawDisplay(painter, myOption, myOption.rect, text.toString().mid(0,48));
    drawFocus(painter, myOption, myOption.rect);
}

void ColumnPercent::paint(QPainter *painter,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString percent = text.toString();

    percent += "%";

    /* Como sera o alinhamento */
    myOption.displayAlignment = (Qt::AlignHCenter | Qt::AlignVCenter);
    drawDisplay(painter, myOption, myOption.rect, percent);
    drawFocus(painter, myOption, myOption.rect);
}


void ColumnCenter::paint(QPainter *painter,
                         const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;
    drawDisplay(painter, myOption, myOption.rect, text.toString());
    drawFocus(painter, myOption, myOption.rect);
}

void ColumnLeft::paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignVCenter | Qt::AlignLeft ;
    drawDisplay(painter, myOption, myOption.rect, text.toString());
    drawFocus(painter, myOption, myOption.rect);
}
void ColumnBalance::paint(QPainter *painter,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString balance = text.toString();

    balance.replace(" ","");

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;
    drawDisplay(painter, myOption, myOption.rect, balance);
    drawFocus(painter, myOption, myOption.rect);
}

void ColumnRight::paint(QPainter *painter,
                        const QStyleOptionViewItem &option,
                        const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
    drawDisplay(painter, myOption, myOption.rect, text.toString());
    drawFocus(painter, myOption, myOption.rect);
}

void ColumnCPF::paint(QPainter *painter,
                      const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString cpf;

    if (text.toString().length() == 0)
    {
        cpf = QString::fromUtf8("NÃO INFORMADO");
    }
    else
    {
        cpf = text.toString().mid(0,3) + "." + text.toString().mid(3,3) + "." + text.toString().mid(6,3) + "-" + text.toString().mid(9,2);
    }

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;
    drawDisplay(painter, myOption, myOption.rect, cpf);
    drawFocus(painter, myOption, myOption.rect);
}


void ColumnBoolIcon::paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString result;
    QPixmap pix(":/png/ok-icon.png");
    QPixmap pixError(":/png/cancel-icon.png");
    QRect   rect = option.rect;
    rect.setTop(option.rect.top() + 3);
    rect.setLeft(option.rect.left() + 3);
    rect.setWidth(24);
    rect.setHeight(24);

    if (text.toBool())
    {
        result = QString::fromUtf8("SIM");
        painter->drawPixmap(rect, pix);
    }
    else
    {
        result = QString::fromUtf8("NÃO");
        painter->drawPixmap(rect, pixError);
    }

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;
    drawDisplay(painter, myOption, myOption.rect, result);
    drawFocus(painter, myOption, myOption.rect);
}

void ColumnBool::paint(QPainter *painter,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString result;

    if (text.toBool())
    {
        result = QString::fromUtf8("SIM");
    }
    else
    {
        result = QString::fromUtf8("NÃO");
    }

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;
    drawDisplay(painter, myOption, myOption.rect, result);
    drawFocus(painter, myOption, myOption.rect);
}


void ColumnCNPJ::paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString cnpj;

    if (text.toString().length() == 0)
    {
        cnpj = QString::fromUtf8("NÃO INFORMADO");
    }
    else
    {
        cnpj = text.toString().mid(0,2) + "." + text.toString().mid(2,3) + "." + text.toString().mid(5,3) + "/" + text.toString().mid(8,4) + "-" + text.toString().mid(12,2);
    }

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;
    drawDisplay(painter, myOption, myOption.rect, cnpj);
    drawFocus(painter, myOption, myOption.rect);
}

void ColumnPhone::paint(QPainter *painter,
                        const QStyleOptionViewItem &option,
                        const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString phone;

    phone = "(" + text.toString().mid(0,2) + ") " + text.toString().mid(2,5) + "-" + text.toString().mid(7);

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;

    drawDisplay(painter, myOption, myOption.rect, phone);
    drawFocus(painter, myOption, myOption.rect);
}

void ColumnDate::paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;

    QString strDate;

  //  if(text.toDate() == QDate(1960,01,01) || text.toDate() == QDate(3000,01,01))
  //      strDate = "Sem Validade";
  //  else
        strDate = text.toDate().toString(FMT_DATE);



    drawDisplay(painter, myOption, myOption.rect, strDate);
    drawFocus(painter, myOption, myOption.rect);
}

void ColumnDateTime::paint(QPainter *painter,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;

    drawDisplay(painter, myOption, myOption.rect, text.toDateTime().toString(QString("%1 %2").arg(FMT_DATE).arg(FMT_TIME)));
    drawFocus(painter, myOption, myOption.rect);
}

void ColumnDateTimeNull::paint(QPainter *painter,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    QDate date = index.model()->data(index.model()->index(index.row(),5)).toDate();
//    QTime time = index.model()->data(index.model()->index(index.row(),4)).toTime();

    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString strText= text.toString(); //toDate().toString(QString("%1").arg(FMT_DATE));


    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;

    if( date == QDate(2000,1,1) )
    {
        painter->fillRect(option.rect, BG_COLOR_YELLOW);
        strText = "NO CONDOMÍNIO";
    }

    drawDisplay(painter, myOption, myOption.rect, strText);
    drawFocus(painter, myOption, myOption.rect);
}

void ColumnMoney::paint(QPainter *painter,
                        const QStyleOptionViewItem &option,
                        const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;

    /* Como sera o alinhamento */
    myOption.displayAlignment = (Qt::AlignRight | Qt::AlignVCenter);
    drawDisplay(painter, myOption, myOption.rect, QRadMoney::MoneyHumanForm2(text.toDouble()));
    drawFocus(painter, myOption, myOption.rect);
}


ColumnCheckBox::ColumnCheckBox(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *ColumnCheckBox::createEditor(QWidget *parent,
                                      const QStyleOptionViewItem &/*option*/,
                                      const QModelIndex &/* index */) const
{
    QCheckBox *editor = new QCheckBox(parent);
    editor->setAutoFillBackground(true);
    return editor;
}

void ColumnCheckBox::setEditorData(QWidget *editor,const QModelIndex &index) const
{
    bool value = index.model()->data(index, Qt::EditRole).toBool();

    QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
    checkBox->setChecked(value);
}

void ColumnCheckBox::setModelData(QWidget *editor, QAbstractItemModel *model,
                                  const QModelIndex &index) const
{
    QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
    bool value = checkBox->isChecked();

    model->setData(index, value, Qt::EditRole);
}
void ColumnCheckBox::updateEditorGeometry(QWidget *editor,
                                          const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}

bool ColumnCheckBox::eventFilter(QObject *object, QEvent *event)
{
    QWidget *editor = qobject_cast<QWidget*>(object);
    if (!editor)
        return false;
    if (event->type() == QEvent::KeyPress) {
        switch (static_cast<QKeyEvent *>(event)->key()) {
        case Qt::Key_Tab:
        case Qt::Key_Escape:
            return false;
        default:
            return QItemDelegate::eventFilter(object,event);
        }
    }
    return QItemDelegate::eventFilter(object,event);
}


