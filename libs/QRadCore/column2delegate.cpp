#include <QRegExp>
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
    drawDisplay(painter, myOption, myOption.rect, FormatValue(val).toString());
    drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnMoney2::FormatValue(QVariant value) const
{
    return QVariant(QRadMoney::MoneyHumanForm2(value.toDouble()));
}

ColumnSCombo::ColumnSCombo(QObject *parent, SComboBox* cmb)
    : QRadDelegate(parent)
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

QVariant ColumnSCombo::FormatValue(QVariant value) const
{
    return value;
}


ColumnCombo::ColumnCombo(QObject *parent, QComboBox* cmb)
    : QRadDelegate(parent)
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
QVariant ColumnCombo::FormatValue(QVariant value) const
{
    return value;
}


ColumnSpin::ColumnSpin(QObject *parent, QSpinBox* cmb)
    : QRadDelegate(parent)
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

QVariant ColumnSpin::FormatValue(QVariant value) const
{
    return value;
}



void ColumnObs::paint(QPainter *painter,
                      const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;

    /* Como sera o alinhamento */
    myOption.displayAlignment = (Qt::AlignLeft | Qt::AlignVCenter);
    drawDisplay(painter, myOption, myOption.rect, FormatValue(text).toString());
    drawFocus(painter, myOption, myOption.rect);
}
QVariant ColumnObs::FormatValue(QVariant value) const
{
    return value.toString().mid(0,48);
}


void ColumnPercent::paint(QPainter *painter,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString percent = FormatValue(text).toString();


    /* Como sera o alinhamento */
    myOption.displayAlignment = (Qt::AlignHCenter | Qt::AlignVCenter);
    drawDisplay(painter, myOption, myOption.rect, percent);
    drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnPercent::FormatValue(QVariant value) const
{
    QString percent = value.toString() +"%";

    return percent;
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

QVariant ColumnCenter::FormatValue(QVariant value) const
{
    return value;
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

QVariant ColumnLeft::FormatValue(QVariant value) const
{
    return value;
}

void ColumnBalance::paint(QPainter *painter,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString balance = FormatValue(text).toString();


    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;
    drawDisplay(painter, myOption, myOption.rect, balance);
    drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnBalance::FormatValue(QVariant value) const
{
    return value.toString().replace(" ","");
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

QVariant ColumnRight::FormatValue(QVariant value) const
{
    return value;
}

void ColumnCPF::paint(QPainter *painter,
                      const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString cpf = FormatValue(text).toString();


    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;
    drawDisplay(painter, myOption, myOption.rect, cpf);
    drawFocus(painter, myOption, myOption.rect);
}


QVariant ColumnCPF::FormatValue(QVariant value) const
{
    QString cpf;

    if (value.toString().length() == 0)
    {
        cpf = QString::fromUtf8("NÃO INFORMADO");
    }
    else
    {
        cpf = value.toString().mid(0,3) + "." + value.toString().mid(3,3) + "." + value.toString().mid(6,3) + "-" + value.toString().mid(9,2);
    }

    return cpf;
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

    result = FormatValue(text).toString();
    if (result == "SIM")
        painter->drawPixmap(rect, pix);
    else
        painter->drawPixmap(rect, pixError);

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;
    drawDisplay(painter, myOption, myOption.rect, result);
    drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnBoolIcon::FormatValue(QVariant value) const
{
    if (value.toBool())
        return QString::fromUtf8("SIM");

    return QString::fromUtf8("NãO");
}

void ColumnBool::paint(QPainter *painter,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString result;

    result = FormatValue(text).toString();

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;
    drawDisplay(painter, myOption, myOption.rect, result);
    drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnBool::FormatValue(QVariant value) const
{
    if (value.toBool())
        return QString::fromUtf8("SIM");

    return QString::fromUtf8("NÃO");
}

void ColumnCNPJ::paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString cnpj;

    cnpj = FormatValue(text).toString();

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;
    drawDisplay(painter, myOption, myOption.rect, cnpj);
    drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnCNPJ::FormatValue(QVariant value) const
{
    QString cnpj;
    if (value.toString().length() == 0)
    {
        cnpj = QString::fromUtf8("NÃO INFORMADO");
    }
    else
    {
        cnpj = value.toString().mid(0,2) + "." + value.toString().mid(2,3) + "." + value.toString().mid(5,3) + "/" + value.toString().mid(8,4) + "-" + value.toString().mid(12,2);
    }

    return cnpj;
}

void ColumnPhone::paint(QPainter *painter,
                        const QStyleOptionViewItem &option,
                        const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;
    QString phone;

    phone = FormatValue(text).toString();

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;

    drawDisplay(painter, myOption, myOption.rect, phone);
    drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnPhone::FormatValue(QVariant value) const
{
    return "(" + value.toString().mid(0,2) + ") " + value.toString().mid(2,5) + "-" + value.toString().mid(7);
}


void ColumnDate::paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;

    QString strDate = FormatValue(text).toString();



    drawDisplay(painter, myOption, myOption.rect, strDate);
    drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnDate::FormatValue(QVariant value) const
{
    return value.toDate().toString(FMT_DATE);;
}


void ColumnDateTime::paint(QPainter *painter,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;

    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;

    drawDisplay(painter, myOption, myOption.rect, FormatValue(text).toString());
    drawFocus(painter, myOption, myOption.rect);
}


QVariant ColumnDateTime::FormatValue(QVariant value) const
{
    return value.toDateTime().toString(QString("%1 %2").arg(FMT_DATE).arg(FMT_TIME));
}

void ColumnDateTimeNull::paint(QPainter *painter,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    QStyleOptionViewItem myOption = option;
    QString strText= FormatValue(index.model()->data(index.model()->index(index.row(),5)).toDate()).toString();


    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;

    if( strText == "NO CONDOMÍNIO" )
    {
        painter->fillRect(option.rect, BG_COLOR_YELLOW);
    }

    drawDisplay(painter, myOption, myOption.rect, strText);
    drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnDateTimeNull::FormatValue(QVariant value) const
{
    if( value.toDate() == QDate(2000,1,1) )
        return QString("NO CONDOMÍNIO");
    return value.toDate().toString(FMT_DATE);
}



void ColumnTimeNull::paint(QPainter *painter,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    QStyleOptionViewItem myOption = option;
    QString strText= FormatValue(index.model()->data(index.model()->index(index.row(),5)).toDate()).toString();
    QString strText2= index.model()->data(index.model()->index(index.row(),6)).toDate().toString(FMT_TIME);


    /* Como sera o alinhamento */
    myOption.displayAlignment = Qt::AlignCenter;

    if( strText == "NO CONDOMÍNIO" )
    {
        painter->fillRect(option.rect, BG_COLOR_YELLOW);
    }
    else
    {
        strText = strText2;
    }

    drawDisplay(painter, myOption, myOption.rect, strText);
    drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnTimeNull::FormatValue(QVariant value) const
{
    if( value.toDate() == QDate(2000,1,1) )
        return QString("NO CONDOMÍNIO");
    return value.toDate().toString(FMT_DATE);
}




void ColumnMoney::paint(QPainter *painter,
                        const QStyleOptionViewItem &option,
                        const QModelIndex &index) const
{
    QVariant text = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;

    /* Como sera o alinhamento */
    myOption.displayAlignment = (Qt::AlignRight | Qt::AlignVCenter);
    drawDisplay(painter, myOption, myOption.rect, FormatValue(text).toString());
    drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnMoney::FormatValue(QVariant value) const
{
   return QRadMoney::MoneyHumanForm2(value.toDouble());
}


ColumnCheckBox::ColumnCheckBox(QObject *parent)
    : QRadDelegate(parent)
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

QVariant ColumnCheckBox::FormatValue(QVariant value) const
{
   return value;
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
            return QRadDelegate::eventFilter(object,event);
        }
    }
    return QRadDelegate::eventFilter(object,event);
}


void ColumnImageMail::paint(QPainter *painter,
                         const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    QString val;
    QVariant cell = index.model()->data(index, Qt::DisplayRole);
    QStyleOptionViewItem myOption = option;

     if( cell.toInt() )
     {
         QString imgFile = ":/png/icon_mail.png";
         QPixmap pixmap(imgFile);
         painter->drawPixmap(myOption.rect, pixmap);
     }
     else
     {
         QString imgFile = ":/png/icon_id.png";
         QPixmap pixmap(imgFile);
         painter->drawPixmap(myOption.rect, pixmap);

     }



//    val = val.toDate().toString(FMT_DATE);
    drawDisplay(painter, myOption, myOption.rect, val);
    drawFocus(painter, myOption, myOption.rect);
}

QVariant ColumnImageMail::FormatValue(QVariant value) const
{
    QString strType = "Visita";
    if(value.toInt())
        strType = "Entrega Doc";

   return strType;
}
