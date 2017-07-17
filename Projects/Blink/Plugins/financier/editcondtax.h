#ifndef Editcondtx_H
#define Editcondtx_H

#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>
#include "extratx.h"

namespace Ui {
class Editcondtx;
}

class Editcondtx : public QDialog
{
    Q_OBJECT

public:
    explicit Editcondtx(QWidget *parent = 0);
    ~Editcondtx();
    void SetModel(extratx* mod);
    QList<int> GetSaved();
    bool ToAll();
    void setVencto(QDate date);
    QDate getVencto();
    double getValue();
    double getDisocunt();
    QString getObs();
    void setObs(QString obs );


public slots:
    void Save();
    void Cancel();
    void Add();
    void Remove();

    void MovedOut();
private:
    double m_discount;
    QSqlQueryModel *m_selected;
    Ui::Editcondtx *ui;
    QList<int> m_selectedIds;

    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
    void RefreshListView();
    void ShowHideValue();
};

#endif // Editcondtx_H
