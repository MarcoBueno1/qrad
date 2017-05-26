#ifndef EDITtipocobranca_H
#define EDITtipocobranca_H

#include <QDialog>
#include <QKeyEvent>
#include "tipocobranca.h"

namespace Ui {
class Edittipocobranca;
}

class Edittipocobranca : public QDialog
{
    Q_OBJECT

public:
    explicit Edittipocobranca(QWidget *parent = 0);
    ~Edittipocobranca();
    void SetModel(tipocobranca* mod);
    tipocobranca* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Edittipocobranca *ui;
    tipocobranca* m_mod;
    tipocobranca* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITtipocobranca_H
