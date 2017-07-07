#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>

void configureDatabase()
{
//    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL7");
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("testedb");
    db.setHostName("localhost");
    db.setPassword("dsmpassword");
    db.setUserName("dsm");

    if(!db.open())
    {
       // qDebug() <<"Impossivel conectar no banco";
    }
    else
    {
       // qDebug() << "Conexão Ok";
    }

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    configureDatabase();

    MainWindow w;
    w.show();

    return a.exec();
}
