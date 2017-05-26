#ifndef EDITsupplier_H
#define EDITsupplier_H

#include <QDialog>
#include <QKeyEvent>
#include "supplier.h"

namespace Ui {
class Editsupplier;
}

class Editsupplier : public QDialog
{
    Q_OBJECT

public:
    explicit Editsupplier(QWidget *parent = 0);
    ~Editsupplier();
    void SetModel(supplier* mod);
    supplier* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Editsupplier *ui;
    supplier* m_mod;
    supplier* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITsupplier_H
