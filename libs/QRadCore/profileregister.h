#ifndef PROFILEREGISTER_H
#define PROFILEREGISTER_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegExpValidator>
#include <QRegExp>
//#include "profilemodel.h"

namespace Ui {
    class ProfileRegister;
}

class ProfileRegister : public QDialog {
    Q_OBJECT
public:
    ProfileRegister(QWidget *parent = 0);
    ~ProfileRegister();

    void    SendProfileId(int profileId);
    void    SendCurrentTab(int currentTab);
    QString GetProfileName(void);

protected:
    int     m_profileId;
    QString m_profileName;

    void    changeEvent(QEvent *e);
    void    ConfigDsmType(void);

private:
    Ui::ProfileRegister *m_ui;

private slots:
    void    SaveProfile(void);
    void    OnClickedProducts(bool);
    void    OnClickedProduct(void);
    void    OnClickedClients(bool);
    void    OnClickedClient(void);
};

#endif // PROFILEREGISTER_H
