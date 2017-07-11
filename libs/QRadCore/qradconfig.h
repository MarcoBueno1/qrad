#ifndef QRADCONFIG_H
#define QRADCONFIG_H

#include <QString>
#include <QWidget>
#include <QLineEdit>


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

private:
    static bool ShowFullScreen(void);

};

#endif // QRADCONFIG_H
