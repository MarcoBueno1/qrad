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

    bool ToAll();
    void setVencto(QDate date);
    QDate getVencto();
    QList<int> GetSavedIds();
public slots:
    void Save();
    void Cancel();
    
    void Add();
    void Remove();
    void MovedOut();
private:
    QSqlQueryModel *m_selected;
    QList<int> m_selectedIds;

    Ui::Editextratx *ui;
    extratx* m_mod;
    extratx* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
    void RefreshListView();
};

#endif // EDITextratx_H
