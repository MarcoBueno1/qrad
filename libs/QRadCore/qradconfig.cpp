#include "qradconfig.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QSettings>
#include <QFileDialog>
#include <QStandardPaths>

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

int QRadConfig::GetCurrentUserId()
{
   QCoreApplication *app = QCoreApplication::instance();
   return app->property("CurrentUserId").toInt();
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

bool QRadConfig::IsSimpleInterest()
{
  return true;
}

void QRadConfig::GoTo(QLineEdit *item)
{
    item->setFocus();
    item->selectAll();
}

QString QRadConfig::GetDownloadDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
}

QString QRadConfig::GetAndPersistDir( QString VarName,
                                      QString DefaultPath,
                                      QString WindowTitle,
                                      QString Types,
                                      QWidget *parent)
{
    QSettings settings(QString("mem.ini"),
                       QSettings::IniFormat);

    QString Path = settings.value(QString("Paths/%1").arg(VarName)).toString();
    if( Path.isEmpty() )
        Path  = DefaultPath;

    if( Path.isEmpty())
        Path = GetDownloadDir();

    Path = QFileDialog::getOpenFileName(parent,
        WindowTitle, Path, Types);//tr("Arquivos Retorno (*.ret *.RET)"));

    if( !Path.isEmpty() )
    {
        settings.setValue(QString("Paths/%1").arg(VarName),Path);
    }
     return Path;
}
