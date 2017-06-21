#ifndef EDITextratx_H
#define EDITextratx_H

#include <QDialog>
#include <QKeyEvent>
#include "extratx.h"

namespace Ui {
class Editextratx;
}

class Editextratx : public QDialog
{
    Q_OBJECT

public:
    explicit Editextratx(QWidget *parent = 0);
    ~Editextratx();
    void SetModel(extratx* mod);
    extratx* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Editextratx *ui;
    extratx* m_mod;
    extratx* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITextratx_H
