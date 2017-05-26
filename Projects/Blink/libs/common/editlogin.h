#ifndef EDITlogin_H
#define EDITlogin_H

#include <QDialog>
#include <QKeyEvent>
#include "login.h"

namespace Ui {
class Editlogin;
}

class Editlogin : public QDialog
{
    Q_OBJECT

public:
    explicit Editlogin(QWidget *parent = 0);
    ~Editlogin();
    void SetModel(login* mod);
    login* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Editlogin *ui;
    login* m_mod;
    login* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITlogin_H
