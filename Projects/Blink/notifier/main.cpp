#include <QApplication>
#include <QSqlDatabase>
#include <QDebug>
#include "task.h"

//Smtp *g_mail = NULL ;

//garden.club.residence@gmail.com
//1234marc

/*
smtp.gmail.com

Requer SSL: Sim

Requer TLS: Sim (se disponível)

Requer autenticação: Sim

Porta para SSL: 465

Porta para TLS/STARTTLS: 587
*/

#include <QtCore>




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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Task parented to the application so that it
        // will be deleted by the application.
        Task *task = new Task(&a);

        // This will cause the application to exit when
        // the task signals finished.
        QObject::connect(task, SIGNAL(finished()), &a, SLOT(quit()));

        configureDatabase();

        // This will run the task from the application event loop.
        QTimer::singleShot(1000, task, SLOT(run()));
        //task->run();

    return a.exec();
}
