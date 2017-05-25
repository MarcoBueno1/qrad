#include "dsmgroupboxenter.h"

DSMGroupBoxEnter::DSMGroupBoxEnter(QWidget *parent) : QGroupBox(parent)
{
}

void DSMGroupBoxEnter::keyPressEvent( QKeyEvent * event )
{
    if ( (event->key() == Qt::Key_Return) || (event->key() == Qt::Key_Enter) )
    {
        this->focusNextChild();
    }
    else
    {
        QGroupBox::keyPressEvent(event);
    }
}
