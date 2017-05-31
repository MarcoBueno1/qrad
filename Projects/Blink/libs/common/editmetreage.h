#ifndef EDITmetreage_H
#define EDITmetreage_H

#include <QDialog>
#include <QKeyEvent>
#include "metreage.h"

namespace Ui {
class Editmetreage;
}

class Editmetreage : public QDialog
{
    Q_OBJECT

public:
    explicit Editmetreage(QWidget *parent = 0);
    ~Editmetreage();
    void SetModel(metreage* mod);
    metreage* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Editmetreage *ui;
    metreage* m_mod;
    metreage* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITmetreage_H
