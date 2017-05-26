#ifndef EDITbankaccount_H
#define EDITbankaccount_H

#include <QDialog>
#include <QKeyEvent>
#include "bankaccount.h"

namespace Ui {
class Editbankaccount;
}

class Editbankaccount : public QDialog
{
    Q_OBJECT

public:
    explicit Editbankaccount(QWidget *parent = 0);
    ~Editbankaccount();
    void SetModel(bankaccount* mod);
    bankaccount* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Editbankaccount *ui;
    bankaccount* m_mod;
    bankaccount* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITbankaccount_H
