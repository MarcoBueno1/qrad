#include "dsmcomboboxenter.h"

DSMComboBoxEnter::DSMComboBoxEnter(QWidget *parent) : QComboBox(parent)
{
}

void DSMComboBoxEnter::keyPressEvent ( QKeyEvent * event )
{
    if ( (event->key() == Qt::Key_Return) || (event->key() == Qt::Key_Enter) )
    {
        this->focusNextChild();
    }
    else
    {
        QComboBox::keyPressEvent(event);
    }
}
