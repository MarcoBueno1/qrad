#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QKeyEvent>

namespace Ui {
    class ProfileManager;
}

class ProfileManager : public QDialog {
    Q_OBJECT
public:
    ProfileManager(QWidget *parent = 0);
    ~ProfileManager();

protected:
    QSqlQueryModel  *m_selectProfiles;
    int             m_profileId;
    void    changeEvent(QEvent *e);
    void    showEvent(QShowEvent *event);
    void    keyPressEvent(QKeyEvent *event);
    void    ConfigHeaderTable(void);
    void    InitialConfig(QString name = "");
    void    ShowCurrentInformations( void );
    void    getAlignment(QModelIndex index);

private:
    Ui::ProfileManager *m_ui;

private slots:
    void    fillTheFields(QModelIndex);
    void    SearchProfile(QString);
    void    NewProfile(void);
    void    EditProfile(void);
    void    DeleteProfile(void);
    void    TableClicked(QModelIndex);
    void    Founded(QModelIndex);
    void    notFound();

};

#endif // PROFILEMANAGER_H
