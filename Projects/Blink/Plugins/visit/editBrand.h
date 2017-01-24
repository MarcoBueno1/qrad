#ifndef EDITBrand_H
#define EDITBrand_H

#include <QDialog>
#include <QKeyEvent>
#include "Brand.h"

namespace Ui {
class EditBrand;
}

class EditBrand : public QDialog
{
    Q_OBJECT

public:
    explicit EditBrand(QWidget *parent = 0);
    ~EditBrand();
    void SetModel(Brand* mod);
    Brand* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::EditBrand *ui;
    Brand* m_mod;
    Brand* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITBrand_H
