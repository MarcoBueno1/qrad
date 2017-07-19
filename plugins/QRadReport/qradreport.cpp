#include "qradreport.h"
#include <QMainWindow>
#include "ormreport.h"
#include "qraddebug.h"
#include "reportlauncher.h"

QRadReportPlugin::QRadReportPlugin()
{


}

QRadReportPlugin::~QRadReportPlugin()
{


}

void QRadReportPlugin::onLoad(QRadPluginContainer* container)
{
    m_container = container;

    QMainWindow *mainwindow = qobject_cast<QMainWindow*>(m_parent);

    QRadPluginLoader *pLoader = new QRadPluginLoader(mainwindow);


    reportList *rep = report::findAll();
    for( int i = 0; rep  && (i < rep->count()); i++  )
    {
       report *r = rep->at(i);

       if( !r->getVisible()  )
           continue;

       QRadPluginMenu *menu = new QRadPluginMenu(r->getPath(),r->getMenu(),r->getMenu(),"",r->getPermission());

       pLoader->loadMenu(this,menu);

       appendAction(r->getMenu());
    }
    delete pLoader;


}

void QRadReportPlugin::Process( const QString& action )
{
  qDebug() << "Action: " << action;

  ReportLauncher *RepLauncher = new ReportLauncher;

  RepLauncher->Exec(action);

  delete RepLauncher;
}

void QRadReportPlugin::setParam(QString str, QVariant v)
{
    Q_UNUSED(str);
    Q_UNUSED(v);
}

QVariant QRadReportPlugin::getParam(QString str)
{
    Q_UNUSED(str);
    return QVariant();
}
/*
bool QRadReportPlugin::loadMenu(QRadPluginMenu *menu)
{
    if (NULL == m_parent)
        return false;

    QMainWindow *mainwindow = qobject_cast<QMainWindow*>(m_parent);

    QWidget* currentMenu = mainwindow->menuBar();
    QStringList menus = menu->path().split('/');

    for ( int i = 0; i < menus.count(); i++ )
    {
        currentMenu = findMenu(currentMenu, menus.at( i ) );
    }

    QMenu* lastMenu = qobject_cast<QMenu*>( currentMenu );

    if ( 0 == lastMenu )
    {
        return false;
    }

    QRadPluginAction* action = new QRadPluginAction( menu->title() );



#if (DEVELOPMENT_VERSION == 1)
    action->setEnabled(true);
#else
    action->setEnabled(QRadConfig::GetUserProfile(menu->permission()));
#endif

    action->setActionDescriptor(menu->action());
    lastMenu->addAction( action );
    action->connect( action, SIGNAL(triggered(QString)), this, SLOT(Process(QString)) );

    return true;
}

*/
