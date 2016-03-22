#include "qradlineeditfakeenter.h"

QRadLineEditFakeEnter::QRadLineEditFakeEnter(QWidget *parent) : QLineEdit(parent)
{
    QString styleSheet = QString("QLineEdit{ %1 %2 } QLineEdit:focus{ %3 %4 %5 %6} QLineEdit:disabled{ %7 }")
                            .arg(FG_COLOR_NORMAL)
                            .arg(BG_COLOR_NORMAL)
                            .arg(FG_COLOR_FOCUSED)
                            .arg(BG_COLOR_FOCUSED)
                            .arg(FG_SEL_COLOR_FOCUSED)
                            .arg(BG_SEL_COLOR_FOCUSED)
                            .arg(BG_COLOR_DISABLED);

    this->setStyleSheet(LINE_EDIT_STYLESHEET);
}

void QRadLineEditFakeEnter::clear()
{
    setText("");
}


