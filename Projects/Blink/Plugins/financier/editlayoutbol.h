#ifndef EDITlayoutbol_H
#define EDITlayoutbol_H

#include <QDialog>
#include <QKeyEvent>
#include "layoutbol.h"

namespace Ui {
class Editlayoutbol;
}

class Editlayoutbol : public QDialog
{
    Q_OBJECT

public:
    explicit Editlayoutbol(QWidget *parent = 0);
    ~Editlayoutbol();
    void SetModel(layoutbol* mod);
    layoutbol* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Editlayoutbol *ui;
    layoutbol* m_mod;
    layoutbol* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITlayoutbol_H
