#ifndef EDITfiles_H
#define EDITfiles_H

#include <QDialog>
#include <QKeyEvent>
#include "files.h"

namespace Ui {
class Editfiles;
}

class Editfiles : public QDialog
{
    Q_OBJECT

public:
    explicit Editfiles(QWidget *parent = 0);
    ~Editfiles();
    void SetModel(Files* mod);
    Files* GetSaved();

    void setFile(QString path);
public slots:
    void Save();
    void Cancel();
    
    void doSelect();
    void doAssociate(int current);
private slots:
    void doView();
private:
    Ui::Editfiles *ui;
    Files* m_mod;
    Files* m_lastMod;
    int m_associatedId;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITfiles_H
