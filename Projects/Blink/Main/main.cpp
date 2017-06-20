#include <QApplication>
#include <QDesktopWidget>
#include <QSplashScreen>
#include <QBitmap>
#include <QSqlDatabase>
#include "qradsplashscreen.h"
#include "qradcoreplugin.h"
#include "mainwindow.h"
#include "qraddebug.h"
#include "editlogin.h"
#include "login.h"
#include "qradlicup.h"
#include "clicense.h"
#include <QMessageBox>
#include "qradlicensecontrol.h"

void centerWidget(QWidget *widget)
{
    QRect rect = QApplication::desktop()->availableGeometry(widget);
    widget->move(rect.center() - widget->rect().center());
}

void configureDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL7");
///    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("qraddb");
    db.setHostName("localhost");
    db.setPassword("dsmpassword");
    db.setUserName("dsm");

    if(!db.open())
    {
        qDebug() <<"Impossivel conectar no banco";
    }
    else
    {
        qDebug() << "Conexão Ok";
    }

}

QRadPluginLoader *g_pluginLoader;
MainWindow      *g_mainWindow;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QRadSplashScreen *splash;
    QPixmap pix(":/png/splash.png");
//    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
    int rc;

    splash = new QRadSplashScreen(pix, Qt::WindowStaysOnTopHint);
    splash->setMask(pix.mask());


    /* Configure database */

    debug_message("[main] -->configureDatabase...\n");

    configureDatabase();

    debug_message("[main] <--configureDatabase...\n");

    /* Translate application components */

//    QTranslator tradutorQt;

//    if(!tradutorQt.load("qt_pt.qm", "C:\\Dvl\\qrad"))
//        QMessageBox::information(NULL,"ERRO AO CARREGAR LINGUAGM", "Erro" );
//    a.installTranslator(&tradutorQt);

    Editlogin *el = new Editlogin;

    if( QDialog::Accepted != el->exec())
       return 0;
 

    QRadLicUp *pLic = new QRadLicUp;
    pLic->SyncLicence();
    delete pLic;

    Clicense *pl = new Clicense;
    if( pl->IsValidlicense() != DSM_SUCCESS )
    {
       debug_message("Licenca invalida, preparar janela de entrada de licenca... atualize!!!!!");
       LicenseControl *pLC = new LicenseControl(pl) ;
       if( QDialog::Accepted != pLC->exec())
       {
           delete pLC;
           return 0;
       }
       delete pLC;
    }

    delete pl;

    splash->show();
    splash->setMessage("Configurando janela principal");
    a.processEvents();

    g_mainWindow = new MainWindow();
    centerWidget(g_mainWindow);
    g_mainWindow->setParent(0,0);

    splash->setMessage("Carregando Plugins");
    a.processEvents();

    g_pluginLoader = new QRadPluginLoader(g_mainWindow,splash);
    g_pluginLoader->Load("plugins.xml");

    splash->finish(g_mainWindow);
    g_mainWindow->setParent(0, 0);
    g_mainWindow->show();

    rc = a.exec();

    // manter sempre esta ordem de delete, pois o objeto mainwindow utiliza o objeto pluginloader

    delete splash;
    delete g_pluginLoader;
    delete g_mainWindow;

    return rc;
}
