#ifndef PERSISTFILETODATABASE_H
#define PERSISTFILETODATABASE_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class PersistFileToDatabase;
}

class PersistFileToDatabase : public QDialog
{
    Q_OBJECT

public:
    explicit PersistFileToDatabase(QWidget *parent = 0);
    ~PersistFileToDatabase();

private slots:
    void doAdd();
    void doRemove();
    void doView();
    void PrepareHeaders();
private:
    Ui::PersistFileToDatabase *ui;
    QSqlQueryModel *m_model;

};

#endif // PERSISTFILETODATABASE_H
