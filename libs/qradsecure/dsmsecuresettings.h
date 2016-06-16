#ifndef DSMSECURESETTINGS_H
#define DSMSECURESETTINGS_H

#include <QSettings>
#include <QStringList>
#include <QRegExp>
#include <QDebug>
#include <QMap>

#include "dsmsecure.h"

bool encryptedReadFunc(QIODevice &device, QSettings::SettingsMap &map);
bool encryptedWriteFunc(QIODevice &device, const QSettings::SettingsMap &map);

class DSMSecureSettings : public QSettings
{
public:
    DSMSecureSettings(QObject *parent = 0);

    static QSettings::Format EncryptedInitFormat;
};

#endif // DSMSECURESETTINGS_H
