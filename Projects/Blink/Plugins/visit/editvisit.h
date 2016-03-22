#ifndef EDITvisit_H
#define EDITvisit_H

#include <QDialog>
#include <QKeyEvent>
#include "visit.h"

namespace Ui {
class Editvisit;
}

class Editvisit : public QDialog
{
    Q_OBJECT

public:
    explicit Editvisit(QWidget *parent = 0);
    ~Editvisit();
    void SetModel(visit* mod);
    visit* GetSaved();

public slots:
    void Save();
    void Cancel();
    
    void baterFoto();
private:
    Ui::Editvisit *ui;
    visit* m_mod;
    visit* m_lastMod;
    int m_idVisitante;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
    void CriaTabela();
    bool CanSave();
};

#endif // EDITvisit_H
