#include "dsmspinboxenter.h"

DSMSpinBoxEnter::DSMSpinBoxEnter(QWidget *parent) : QSpinBox(parent)
{
    QString styleSheet = QString("QSpinBox{%1 %2} QSpinBox:focus{ %3 %4 %5 %6 }")
                                         .arg(FG_COLOR_NORMAL)
                                         .arg(BG_COLOR_NORMAL)
                                         .arg(FG_COLOR_FOCUSED)
                                         .arg(BG_COLOR_FOCUSED)
                                         .arg(FG_SEL_COLOR_FOCUSED)
                                         .arg(BG_SEL_COLOR_FOCUSED);

    //this->setStyleSheet(styleSheet);
    this->setStyleSheet(SPINBOX_STYLESHEET);
}

void DSMSpinBoxEnter::keyPressEvent( QKeyEvent * event )
{
    if ( (event->key() == Qt::Key_Return) || (event->key() == Qt::Key_Enter) )
    {
        this->focusNextChild();
    }
    else
    {
        QSpinBox::keyPressEvent(event);
    }
}
