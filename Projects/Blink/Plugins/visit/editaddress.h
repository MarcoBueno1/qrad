#ifndef EDITaddress_H
#define EDITaddress_H

#include <QDialog>
#include <QKeyEvent>
#include "address.h"

namespace Ui {
class Editaddress;
}

class Editaddress : public QDialog
{
    Q_OBJECT

public:
    explicit Editaddress(QWidget *parent = 0);
    ~Editaddress();
    void SetModel(Address* mod);
    Address* GetSaved();
    void setOwner(int id);
    void setOwnerType(int id);

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Editaddress *ui;
    Address* m_mod;
    Address* m_lastMod;
    int m_Owner;
    int m_OwnerType;

    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITaddress_H
