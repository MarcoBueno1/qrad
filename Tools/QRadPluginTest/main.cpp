#include <QApplication>
#include "dialog.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include "qradfile.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase(QRadFile::GetParamValue("banco/database"));

    db.setDatabaseName(QRadFile::GetParamValue("banco/databasename"));
    db.setHostName(QRadFile::GetParamValue("banco/hostname"));
    db.setUserName(QRadFile::GetParamValue("banco/username"));
    db.setPassword(QRadFile::GetParamValue("banco/password"));

    Dialog w;

    if (!db.open())
    {
        QMessageBox::critical(&w, "Error", QString("Could not connect to database. reason: %1")
                              .arg(db.lastError().text()));
        return -1;
    }

	if (argc > 1) {
		w.setFileName(argv[1]);
	}

	if (argc > 2) {
		w.setActionName(argv[2]);
	}

    QCoreApplication *app = QCoreApplication::instance();
    app->setProperty("CurrentUserId", 1);    /// admin user = 1

    w.show();
    return a.exec();
}
