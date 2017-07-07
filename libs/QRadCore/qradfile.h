#ifndef QRadFILE_H
#define QRadFILE_H

#include <QFile>
#include <QSettings>
#include <QCoreApplication>

class QRadFile : public QFile
{
public:
    QRadFile();
    QRadFile(QString &name);
    bool Copy(const QString &fileName, const QString &newName);
    static bool SCopy(const QString &fileName, const QString &newName);
    static qint64 Size(QString fileName);

    /** Arquivo config.ini */
    static QString  GetParamValue(QString param, QString strConfigFile = "config.enc");
    static void     SetParamValue(QString param, QString value, QString strConfigFile = "config.enc");
    static QString  GetDefaultParamValue(QString param);

};

#endif // QRadFILE_H
