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
bool QRadConfig::ShowFullScreen(void)
{
 /*   QSqlQueryModel  *select = new QSqlQueryModel;
    bool            result = false;
    select->setQuery(QString(SQL_SELECT_CONFIG));
    if (select->rowCount() != 0)
    {
        result = select->record(0).value("showfullscreen").toBool();
    }
    delete select;

    return result;
*/
    return true;
}

void QRadConfig::fullScreen(QWidget *widget)
{
    if (ShowFullScreen())
    {
        QRect rect = QApplication::desktop()->availableGeometry(widget);
        widget->move(rect.center() - widget->rect().center());
        widget->showMaximized();
    }
    else
    {
        centralizarWidget(widget);
    }
}


