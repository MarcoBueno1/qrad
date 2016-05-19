#ifndef EDITphone_H
#define EDITphone_H

#include <QDialog>
#include <QKeyEvent>
#include "phone.h"

namespace Ui {
class Editphone;
}

class Editphone : public QDialog
{
    Q_OBJECT

public:
    explicit Editphone(QWidget *parent = 0);
    ~Editphone();
    void SetModel(Phone* mod);
    Phone* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Editphone *ui;
    Phone* m_mod;
    Phone* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITphone_H
