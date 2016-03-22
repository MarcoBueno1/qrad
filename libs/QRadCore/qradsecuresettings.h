#ifndef QRadSECURESETTINGS_H
#define QRadSECURESETTINGS_H

#include <QSettings>
#include <QStringList>
#include <QRegExp>
#include <QDebug>
#include <QMap>

#include "qradsecure.h"

bool encryptedReadFunc(QIODevice &device, QSettings::SettingsMap &map);
bool encryptedWriteFunc(QIODevice &device, const QSettings::SettingsMap &map);

class QRadSecureSettings : public QSettings
{
public:
    QRadSecureSettings(QObject *parent = 0);

    static QSettings::Format EncryptedInitFormat;
};

#endif // QRadSECURESETTINGS_H
