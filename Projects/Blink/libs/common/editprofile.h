#ifndef EDITprofile_H
#define EDITprofile_H

#include <QDialog>
#include <QKeyEvent>
#include "profile.h"

namespace Ui {
class Editprofile;
}

class Editprofile : public QDialog
{
    Q_OBJECT

public:
    explicit Editprofile(QWidget *parent = 0);
    ~Editprofile();
    void SetModel(profile* mod);
    profile* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Editprofile *ui;
    profile* m_mod;
    profile* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITprofile_H
