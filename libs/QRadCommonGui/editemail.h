#ifndef EDITemail_H
#define EDITemail_H

#include <QDialog>
#include <QKeyEvent>
#include "email.h"

namespace Ui {
class Editemail;
}

class Editemail : public QDialog
{
    Q_OBJECT

public:
    explicit Editemail(QWidget *parent = 0);
    ~Editemail();
    void SetModel(email* mod);
    email* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Editemail *ui;
    email* m_mod;
    email* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITemail_H
