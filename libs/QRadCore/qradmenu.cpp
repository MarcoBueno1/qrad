#include "qradmenu.h"

QRadMenu::QRadMenu(QWidget *parent) : QMenuBar(parent)
{
    QString styleSheet = QString("QMenu:item { padding: 2px 25px 2px 20px; } QMenu:item:selected{ %1 %2 }")
                            .arg(FG_COLOR_FOCUSED)
                            .arg(BG_COLOR_FOCUSED);

    this->setStyleSheet(styleSheet);
}
