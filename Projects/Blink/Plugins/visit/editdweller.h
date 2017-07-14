#ifndef EDITDweller_H
#define EDITDweller_H

#include <QDialog>
#include <QKeyEvent>
#include "dweller.h"
#include <QSqlQueryModel>
#include  "column2delegate.h"


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
    void AddAddress();
    void RemovePhone();
    void RemoveAddress();
    void baterFoto();
    void AddVeiculo();
    void RemoveVeiculo();
private:
    Ui::EditDweller *ui;
    Dweller* m_mod;
    Dweller* m_lastMod;
    QSqlQueryModel *m_model;
    QSqlQueryModel *m_AddressModel;
    QSqlQueryModel *m_VehicleModel;

    ColumnPhone  *m_PhoneDelegate;
    ColumnCenter *m_CenterDelegate;
    ColumnBool   *m_BooleanDelegate;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
    void RefreshPhoneTable();
    void RefreshAddressTable();
    void RefreshVeicTable();
    void CheckIfCanChangeAPTower();
};

#endif // EDITDweller_H
