#ifndef EDITvehicle_H
#define EDITvehicle_H

#include <QDialog>
#include <QKeyEvent>
#include "vehicle.h"

namespace Ui {
class Editvehicle;
}

class Editvehicle : public QDialog
{
    Q_OBJECT

public:
    explicit Editvehicle(QWidget *parent = 0);
    ~Editvehicle();
    void SetModel(vehicle* mod);
    vehicle* GetSaved();
    void setOwner(int owner);
public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Editvehicle *ui;
    vehicle* m_mod;
    vehicle* m_lastMod;
    int m_owner;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITvehicle_H
