#ifndef EDITvisit_H
#define EDITvisit_H

#include <QDialog>
#include <QKeyEvent>
#include "visit.h"
#include <QSqlQueryModel>

namespace Ui {
class Editvisit;
}

class Editvisit : public QDialog
{
    Q_OBJECT

public:
    explicit Editvisit(QWidget *parent = 0);
    ~Editvisit();
    void SetModel(visit* mod);
    visit* GetSaved();

public slots:
    void Save();
    void Cancel();
    void found(int id );
    
    void baterFoto();
    void notFound();
    void DeliveryCheck(bool bChecked);
private:
    Ui::Editvisit *ui;
    visit* m_mod;
    visit* m_lastMod;
    QSqlQueryModel *m_deliveryto;
    int m_idVisitante;
    int m_foto;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
    void CriaTabela();
    bool CanSave();
    void RemovePhotoFile();
};

#endif // EDITvisit_H
