#ifndef EDITDweller_H
#define EDITDweller_H

#include <QDialog>
#include <QKeyEvent>
#include "dweller.h"
#include <QSqlQueryModel>

namespace Ui {
class EditDweller;
}

class EditDweller : public QDialog
{
    Q_OBJECT

public:
    explicit EditDweller(QWidget *parent = 0);
    ~EditDweller();
    void SetModel(Dweller* mod);
    Dweller* GetSaved();

public slots:
    void Save();
    void Cancel();
    
    void AddPhone();
    void RemovePhone();
private:
    Ui::EditDweller *ui;
    Dweller* m_mod;
    Dweller* m_lastMod;
    QSqlQueryModel *m_model;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
    void RefreshPhoneTable();
};

#endif // EDITDweller_H
