#ifndef NEWEDITINVITED_H
#define NEWEDITINVITED_H

#include <QDialog>
#include "invited.h"


namespace Ui {
class NewEditInvited;
}

class NewEditInvited : public QDialog
{
    Q_OBJECT

public:
    explicit NewEditInvited(QWidget *parent = 0);
    ~NewEditInvited();
    int Exec( int reserveId, int InvitedId=0);

public slots:
    void Save();

private:
    Ui::NewEditInvited *ui;
    Invited *m_invited;
    int m_reserveId;
};

#endif // NEWEDITINVITED_H
