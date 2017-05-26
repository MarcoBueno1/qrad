#ifndef EDITtipopessoa_H
#define EDITtipopessoa_H

#include <QDialog>
#include <QKeyEvent>
#include "tipopessoa.h"

namespace Ui {
class Edittipopessoa;
}

class Edittipopessoa : public QDialog
{
    Q_OBJECT

public:
    explicit Edittipopessoa(QWidget *parent = 0);
    ~Edittipopessoa();
    void SetModel(tipopessoa* mod);
    tipopessoa* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Edittipopessoa *ui;
    tipopessoa* m_mod;
    tipopessoa* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITtipopessoa_H
