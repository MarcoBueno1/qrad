#include "dsmtimeeditenter.h"

QRadTimeEditEnter::QRadTimeEditEnter(QWidget *parent) : QTimeEdit(parent)
{
    QString styleSheet = QString("QTimeEdit{%1 %2} QTimeEdit:focus{ %3 %4 %5 %6}")
                         .arg(FG_COLOR_NORMAL)
                         .arg(BG_COLOR_NORMAL)
                         .arg(FG_COLOR_FOCUSED)
                         .arg(BG_COLOR_FOCUSED)
                         .arg(FG_SEL_COLOR_FOCUSED)
                         .arg(BG_SEL_COLOR_FOCUSED);

    this->setStyleSheet(styleSheet);
}

void QRadTimeEditEnter::keyPressEvent (QKeyEvent * event)
{
    if ( (event->key() == Qt::Key_Return) || (event->key() == Qt::Key_Enter) )
    {
        this->focusNextChild();
    }
    else
    {
        QTimeEdit::keyPressEvent(event);
    }
}
