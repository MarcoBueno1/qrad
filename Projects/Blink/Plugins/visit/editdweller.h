#ifndef EDITDweller_H
#define EDITDweller_H

#include <QDialog>
#include <QKeyEvent>
#include "dweller.h"

namespace Ui {
class EditDweller;
}

class EditDweller : public QDialog
{
    Q_OBJECT

public:
    explicit EditDweller(QWidget *parent = 0);
    ~EditDweller();
    void SetModel(Dweller* mod);
    Dweller* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::EditDweller *ui;
    Dweller* m_mod;
    Dweller* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITDweller_H
