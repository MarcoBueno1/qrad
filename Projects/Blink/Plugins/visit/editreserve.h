#ifndef EDITreserve_H
#define EDITreserve_H

#include <QDialog>
#include <QKeyEvent>
#include "reserve.h"

namespace Ui {
class Editreserve;
}

class Editreserve : public QDialog
{
    Q_OBJECT

public:
    explicit Editreserve(QWidget *parent = 0);
    ~Editreserve();
    void SetModel(reserve* mod);
    reserve* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Editreserve *ui;
    reserve* m_mod;
    reserve* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITreserve_H
