#include "qradfile.h"
#include "qraddebug.h"
#include "qradsecuresettings.h"

//#define SIZE_LIMIT  150

QRadFile::QRadFile()
{
}

QRadFile::QRadFile(QString &name) : QFile(name)
{
}

bool QRadFile::Copy(const QString &fileName, const QString &newName)
{
    this->remove(newName);
    return this->copy(fileName, newName);
}

bool QRadFile::SCopy(const QString &fileName, const QString &newName)
{
    QFile::remove(newName);
    return QFile::copy(fileName, newName);
}

qint64 QRadFile::Size(QString fileName)
{
    QRadFile file(fileName);

    return file.size();
}

QString QRadFile::GetParamValue(QString param, QString strConfigFile)
{

    QString     value, path, path2;

    path = path2 = QCoreApplication::applicationDirPath() + "/";

    value.clear();
    path.append(strConfigFile);

    QSettings   settings(path, QRadSecureSettings::EncryptedInitFormat);

    value = settings.value(param).toString();

    if (value.isEmpty())
    {
        value = GetDefaultParamValue(param);
    }


    return value;
}

void QRadFile::SetParamValue(QString param, QString value, QString strConfigFile)
{
    debug_message("[QRadFile] -->SetParamValue\n");

    QString path;

    path = QCoreApplication::applicationDirPath() + "/";

    debug_message("[QRadFile] path=%s\n", path.toLatin1().data());


    path.append(strConfigFile);

    debug_message("[QRadFile] path=%s\n", path.toLatin1().data());
    QSettings   settings(path, QRadSecureSettings::EncryptedInitFormat);

    debug_message("[QRadFile] settings.setValue=%s\n", path.toLatin1().data());
    settings.setValue(param, value);

    debug_message("[QRadFile] <--SetParamValue\n");
}

QString QRadFile::GetDefaultParamValue(QString param)
{
    if (param == "banco/hostname") return "localhost";
    if (param == "banco/database") return "QPSQL";
    if (param == "banco/databasename") return "qrad";

    return "";
}
