#ifndef EDITreason_H
#define EDITreason_H

#include <QDialog>
#include <QKeyEvent>
#include "reason.h"

namespace Ui {
class Editreason;
}

class Editreason : public QDialog
{
    Q_OBJECT

public:
    explicit Editreason(QWidget *parent = 0);
    ~Editreason();
    void SetModel(reason* mod);
    reason* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Editreason *ui;
    reason* m_mod;
    reason* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITreason_H
