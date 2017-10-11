#ifndef EDITcommon_area_H
#define EDITcommon_area_H

#include <QDialog>
#include <QKeyEvent>
#include "common_area.h"

namespace Ui {
class Editcommon_area;
}

class Editcommon_area : public QDialog
{
    Q_OBJECT

public:
    explicit Editcommon_area(QWidget *parent = 0);
    ~Editcommon_area();
    void SetModel(common_area* mod);
    common_area* GetSaved();

public slots:
    void Save();
    void Cancel();
    
private:
    Ui::Editcommon_area *ui;
    common_area* m_mod;
    common_area* m_lastMod;
    void Load();
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EDITcommon_area_H
