#ifndef EDITveicname_H
#define EDITveicname_H

#include <QDialog>
#include <QKeyEvent>
#include "veicname.h"

namespace Ui {
class Editveicname;
}

class Editveicname : public QDialog
{
    Q_OBJECT

public:
    explicit Editveicname(QWidget *parent = 0);
    ~Editveicname();
    void SetModel(veicname* mod);
    veicname* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Editveicname *ui;
    veicname* m_mod;
    veicname* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITveicname_H
