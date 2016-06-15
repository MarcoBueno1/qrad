#ifndef EDITmaincompany_H
#define EDITmaincompany_H

#include <QDialog>
#include <QKeyEvent>
#include "maincompany.h"

namespace Ui {
class Editmaincompany;
}

class Editmaincompany : public QDialog
{
    Q_OBJECT

public:
    explicit Editmaincompany(QWidget *parent = 0);
    ~Editmaincompany();
    void SetModel(maincompany* mod);
    maincompany* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Editmaincompany *ui;
    maincompany* m_mod;
    maincompany* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITmaincompany_H
