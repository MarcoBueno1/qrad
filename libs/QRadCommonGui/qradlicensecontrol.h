#ifndef qradLICENSECONTROL_H
#define qradLICENSECONTROL_H

#include <QDialog>
#include <QCloseEvent>
#include "clicense.h"


namespace Ui {
    class LicenseControl;
}

class LicenseControl : public QDialog {
    Q_OBJECT
public:
    LicenseControl(Clicense *pLicense, QWidget *parent = 0);
     ~LicenseControl();

protected:
    void    changeEvent(QEvent *e);
    void    closeEvent(QCloseEvent *event) ;
    void    keyPressEvent(QKeyEvent *event);

private:
    Ui::LicenseControl *m_ui;
    Clicense *m_pLicense;
    QString GetLastLicense();
    bool m_bAllowClose;

private slots:

private slots:
    void    on_pushButtonOk_clicked();
    void    ShowDetails(void);
};

#endif // MAINWINDOW_H
