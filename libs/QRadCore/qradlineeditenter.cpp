#include "qradlineeditenter.h"

QRadLineEditEnter::QRadLineEditEnter(QWidget *parent):QLineEdit(parent)
{
    connect(this,SIGNAL(returnPressed()), this, SLOT(returnPressed()));
}
void QRadLineEditEnter::returnPressed()
{
        this->focusNextChild();
}
