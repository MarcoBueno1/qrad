#ifndef EDITcnab_H
#define EDITcnab_H

#include <QDialog>
#include <QKeyEvent>
#include "cnab.h"

namespace Ui {
class Editcnab;
}

class Editcnab : public QDialog
{
    Q_OBJECT

public:
    explicit Editcnab(QWidget *parent = 0);
    ~Editcnab();
    void SetModel(cnab* mod);
    cnab* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Editcnab *ui;
    cnab* m_mod;
    cnab* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITcnab_H
