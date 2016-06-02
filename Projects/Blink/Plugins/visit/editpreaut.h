#ifndef EDITpreaut_H
#define EDITpreaut_H

#include <QDialog>
#include <QKeyEvent>
#include "preaut.h"

namespace Ui {
class Editpreaut;
}

class Editpreaut : public QDialog
{
    Q_OBJECT

public:
    explicit Editpreaut(QWidget *parent = 0);
    ~Editpreaut();
    void SetModel(preaut* mod);
    preaut* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Editpreaut *ui;
    preaut* m_mod;
    preaut* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITpreaut_H
