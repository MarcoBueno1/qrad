#ifndef EDITVisitante_H
#define EDITVisitante_H

#include <QDialog>
#include <QKeyEvent>
#include "Visitante.h"

namespace Ui {
class EditVisitante;
}

class EditVisitante : public QDialog
{
    Q_OBJECT

public:
    explicit EditVisitante(QWidget *parent = 0);
    ~EditVisitante();
    void SetModel(Visitante* mod);
    Visitante* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::EditVisitante *ui;
    Visitante* m_mod;
    Visitante* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITVisitante_H
