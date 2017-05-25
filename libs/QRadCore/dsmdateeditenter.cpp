#include "dsmdateeditenter.h"

DSMDateEditEnter::DSMDateEditEnter(QWidget *parent) : QDateEdit(parent)
{
    QString styleSheet = QString("QDateEdit{%1 %2} QDateEdit:focus{ %3 %4 %5 %6} QLineEdit:disabled{ %7 }")
                         .arg(FG_COLOR_NORMAL)
                         .arg(BG_COLOR_NORMAL)
                         .arg(FG_COLOR_FOCUSED)
                         .arg(BG_COLOR_FOCUSED)
                         .arg(FG_SEL_COLOR_FOCUSED)
                         .arg(BG_SEL_COLOR_FOCUSED)
                         .arg(BG_COLOR_DISABLED);

    //this->setStyleSheet(styleSheet);
}

void DSMDateEditEnter::keyPressEvent (QKeyEvent * event)
{
    if ( (event->key() == Qt::Key_Return) || (event->key() == Qt::Key_Enter) )
    {
        this->focusNextChild();
    }
    else
    {
        QDateEdit::keyPressEvent(event);
    }
}
