#ifndef QRADCONFIG_H
#define QRADCONFIG_H

#include <QString>

class QRadConfig
{
public:
    QRadConfig();
    ~QRadConfig();

    static bool GetUserProfile(QString strPermsission );
    static QString GetCurrentUserLogin();
};

#endif // QRADCONFIG_H
