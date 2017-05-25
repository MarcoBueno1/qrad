#ifndef QRADCONFIG_H
#define QRADCONFIG_H

#include <QString>
#include <QWidget>


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

private:
    static bool ShowFullScreen(void);

};

#endif // QRADCONFIG_H
