#ifndef QRADEMAIL_H
#define QRADEMAIL_H

#include "m2smtp.h"

class QRadEmail
{
public:
    QRadEmail();
    ~QRadEmail();
    static QRadEmail* getInstance();
    bool Send( QString Name,
               QString to,
               QString Subject,
               QString Body,
               QStringList files );

private:
    static QRadEmail* m_instance;
    QList<M2Smtp *> m_emails;
    QString g_host;
    QString g_user;
    QString g_password;
    QString g_port;

};

#endif // QRADEMAIL_H
