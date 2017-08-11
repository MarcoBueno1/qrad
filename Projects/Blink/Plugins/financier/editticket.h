#ifndef EDITticket_H
#define EDITticket_H

#include <QDialog>
#include <QKeyEvent>
#include "ticket.h"
#include <QSqlQueryModel>


namespace Ui {
class Editticket;
}

class Editticket : public QDialog
{
    Q_OBJECT

public:
    explicit Editticket(QWidget *parent = 0, bool bReadOnly=false);
    ~Editticket();
    void SetModel(ticket* mod);
    ticket* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Editticket *ui;
    ticket* m_mod;
    ticket* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITticket_H
