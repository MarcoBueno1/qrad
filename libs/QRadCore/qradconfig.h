#ifndef QRADCONFIG_H
#define QRADCONFIG_H

#include <QString>
#include <QWidget>
#include <QLineEdit>
#include <QMenuBar>
#include <QMenu>


class QRadConfig
{
public:
    QRadConfig();
    ~QRadConfig();

    static bool GetUserProfile(QString strPermsission );
    static QString GetCurrentUserLogin();
    static void centralizarWidget(QWidget *widget);
    static void fullScreen(QWidget *widget);
    static int GetCurrentUserId();
    static bool IsSimpleInterest();
    static void GoTo(QLineEdit *item);
    static QString GetDownloadDir();
    static QStringList  GetAndPersistDir( QString VarName,
                                          QString DefaultPath,
                                          QString WindowTitle,
                                          QString Types,
                                          QWidget *parent );

    static void AdjustMenuToPermissions(QMenuBar *menuBar);
private:
    static bool ShowFullScreen(void);

    static void enumerateMenu(QMenu *menu);
};

#endif // QRADCONFIG_H
