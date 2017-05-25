#include "dsmradiobuttonenter.h"

DSMRadioButtonEnter::DSMRadioButtonEnter(QWidget *parent) : QRadioButton(parent)
{
}

void DSMRadioButtonEnter::keyPressEvent( QKeyEvent * event )
{
    if ( (event->key() == Qt::Key_Return) || (event->key() == Qt::Key_Enter) )
    {
        this->focusNextChild();
    }
    else
    {
        QRadioButton::keyPressEvent(event);
    }
}
