#ifndef EDITphone_H
#define EDITphone_H

#include <QDialog>
#include <QKeyEvent>
#include "phone.h"

namespace Ui {
class Editphone;
}

class Editphone : public QDialog
{
    Q_OBJECT

public:
    explicit Editphone(QWidget *parent = 0);
    ~Editphone();
    void SetModel(Phone* mod);
    Phone* GetSaved();
    void setOwner(int id);
    void setOwnerType(int id);

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Editphone *ui;
    Phone* m_mod;
    Phone* m_lastMod;
    int m_Owner;
    int m_OwnerType;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITphone_H
