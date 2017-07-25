#ifndef EDITsupplier_H
#define EDITsupplier_H

#include <QDialog>
#include <QKeyEvent>
#include "supplier.h"
#include <QSqlQueryModel>
#include "column2delegate.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>

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
    void AddPhone();
    void RemovePhone();
    void RemoveAddress();
    void AddAddress();

    
    void onCNPJEdited(QString cnpj);
    void replyFinished(QNetworkReply *pReply);
private:
    Ui::Editsupplier *ui;
    supplier* m_mod;
    supplier* m_lastMod;
    QSqlQueryModel *m_model ;
    QSqlQueryModel *m_AddressModel;
    ColumnPhone *m_PhoneDelegate;
    ColumnCenter *m_CenterDelegate;
    ColumnBool   *m_BooleanDelegate;



    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
    void RefreshPhoneTable();
    void RefreshAddressTable();
    QNetworkAccessManager* m_manager;
};

#endif // EDITsupplier_H
