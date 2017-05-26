#ifndef EDITrespemis_H
#define EDITrespemis_H

#include <QDialog>
#include <QKeyEvent>
#include "respemis.h"

namespace Ui {
class Editrespemis;
}

class Editrespemis : public QDialog
{
    Q_OBJECT

public:
    explicit Editrespemis(QWidget *parent = 0);
    ~Editrespemis();
    void SetModel(respemis* mod);
    respemis* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Editrespemis *ui;
    respemis* m_mod;
    respemis* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITrespemis_H
