#include "qraddateeditfakeenter.h"

QRadDateEditFakeEnter::QRadDateEditFakeEnter(QWidget *parent) : QDateEdit(parent)
{
    QString styleSheet = QString("QDateEdit{%1 %2} QDateEdit:focus{ %3 %4 %5 %6}")
                         .arg(FG_COLOR_NORMAL)
                         .arg(BG_COLOR_NORMAL)
                         .arg(FG_COLOR_FOCUSED)
                         .arg(BG_COLOR_FOCUSED)
                         .arg(FG_SEL_COLOR_FOCUSED)
                         .arg(BG_SEL_COLOR_FOCUSED);

    this->setStyleSheet(styleSheet);
}
