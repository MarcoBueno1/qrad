#ifndef EDITticketconfig_H
#define EDITticketconfig_H

#include <QDialog>
#include <QKeyEvent>
#include "ticketconfig.h"

namespace Ui {
class Editticketconfig;
}

class Editticketconfig : public QDialog
{
    Q_OBJECT

public:
    explicit Editticketconfig(QWidget *parent = 0);
    ~Editticketconfig();
    void SetModel(ticketconfig* mod);
    ticketconfig* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Editticketconfig *ui;
    ticketconfig* m_mod;
    ticketconfig* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITticketconfig_H
