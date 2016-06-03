#include "qradconfig.h"
#include <QApplication>
#include <QDesktopWidget>

QRadConfig::QRadConfig()
{

}

QRadConfig::~QRadConfig()
{

}

bool QRadConfig::GetUserProfile(QString strPermsission)
{
   if( strPermsission.contains( "development", Qt::CaseInsensitive))
     return true;

   QCoreApplication *app = QCoreApplication::instance();
   return  app->property(strPermsission.toLatin1().data()).toBool();
}

QString QRadConfig::GetCurrentUserLogin()
{

    return "";
}

void QRadConfig::centralizarWidget(QWidget *widget)
{
    QRect rect = QApplication::desktop()->availableGeometry(widget);
//    widget->setMaximumWidth(WIDTH_SCREEN);
//    widget->setMaximumHeight(HEIGHT_SCREEN);
    widget->move(rect.center() - widget->rect().center());
}


