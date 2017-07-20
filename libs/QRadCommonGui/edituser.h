#ifndef EDITuser_H
#define EDITuser_H

#include <QDialog>
#include <QKeyEvent>
#include "user.h"

namespace Ui {
class Edituser;
}

class Edituser : public QDialog
{
    Q_OBJECT

public:
    explicit Edituser(QWidget *parent = 0);
    ~Edituser();
    void SetModel(user* mod);
    user* GetSaved();

public slots:
    void Save();
    void Cancel();
    
    void ResetSenha();
private:
    Ui::Edituser *ui;
    user* m_mod;
    user* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITuser_H
