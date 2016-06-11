#ifndef EDITemailconf_H
#define EDITemailconf_H

#include <QDialog>
#include <QKeyEvent>
#include "emailconf.h"

namespace Ui {
class Editemailconf;
}

class Editemailconf : public QDialog
{
    Q_OBJECT

public:
    explicit Editemailconf(QWidget *parent = 0);
    ~Editemailconf();
    void SetModel(emailconf* mod);
    emailconf* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Editemailconf *ui;
    emailconf* m_mod;
    emailconf* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITemailconf_H
