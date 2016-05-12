#include "qraddoublespinboxenter.h"

QRadDoubleSpinBoxEnter::QRadDoubleSpinBoxEnter(QWidget *parent) : QDoubleSpinBox(parent)
{
    QString styleSheet = QString("QDoubleSpinBox{%1 %2} QDoubleSpinBox:focus{ %3 %4 %5 %6 }")
                                         .arg(FG_COLOR_NORMAL)
                                         .arg(BG_COLOR_NORMAL)
                                         .arg(FG_COLOR_FOCUSED)
                                         .arg(BG_COLOR_FOCUSED)
                                         .arg(FG_SEL_COLOR_FOCUSED)
                                         .arg(BG_SEL_COLOR_FOCUSED);

    this->setStyleSheet(DOUBLE_SPINBOX_STYLESHEET);
    this->setDecimals(4);
}

void QRadDoubleSpinBoxEnter::keyPressEvent( QKeyEvent * event )
{
    if ( (event->key() == Qt::Key_Return) || (event->key() == Qt::Key_Enter) )
    {
        this->focusNextChild();
    }
    else
    {
        QDoubleSpinBox::keyPressEvent(event);
    }
}
