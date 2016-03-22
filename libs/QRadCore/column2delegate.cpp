#include <QRegExp>

#include "column2delegate.h"
#include "qradmoney.h"

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

    phone = "(" + text.toString().mid(0,2) + ") " + text.toString().mid(2,4) + "-" + text.toString().mid(6);

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
