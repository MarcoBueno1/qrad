#include "qraddebug.h"
#include "qradsecuresettings.h"

#define REGEXP_INI_GROUP        "\\[(.*)\\]"
#define REGEXP_LINESEPARATOR    "\n|\r|(\r\n)|(\n\r)"
#define REGEXP_INI_VARIABLE         "([\\w\\d-\\_]+)[\\s\t]*=[\\s\t]*([\\S]+[\\S\\s\t]*)"
#define REGEXP_INI_GROUPPATH         "\\b([\\w\\d-\\_]+)/([\\w|\\d-]+)\\b"

QSettings::Format QRadSecureSettings::EncryptedInitFormat = QSettings::registerFormat("enc",
                                                                                      encryptedReadFunc,
                                                                                      encryptedWriteFunc,
                                                                                      Qt::CaseSensitive);

QRadSecureSettings::QRadSecureSettings(QObject *parent)
    : QSettings(parent)
{
    setDefaultFormat(QRadSecureSettings::EncryptedInitFormat);
}

/* Read and Write Functions */

bool encryptedReadFunc(QIODevice &device, QSettings::SettingsMap &map)
{
    debug_message("[QRadSecureSettings] -->encryptedReadFunc\n");

    QByteArray encryptedContent;
    QByteArray decryptedContent;
    QString curGroup = "";
    QStringList lines;
    QRegExp regExpGroup(REGEXP_INI_GROUP);
    QRegExp regExpVariable(REGEXP_INI_VARIABLE);
    QRadSecure *secure = new QRadSecure();

    if (!secure->initialize())
    {
        qDebug() << "secure->initialize() failed.";
        delete secure;
        debug_message("[QRadSecureSettings] <--encryptedReadFunc false\n");
        return false;
    }


    if (!device.isOpen())
    {
//        device.close();
        device.open(QIODevice::ReadOnly);
    }
    device.reset();


    device.seek(0);

    encryptedContent = device.readAll();

    decryptedContent = secure->decrypt(encryptedContent);

    lines = QString(decryptedContent).split(QRegExp(REGEXP_LINESEPARATOR));

    for (int curLine = 0; curLine < lines.length(); curLine++)
    {
        QString str = lines.at(curLine);

        if (regExpGroup.indexIn(str) != -1)
        {
            curGroup = regExpGroup.cap(1);
        }
        else if (regExpVariable.indexIn(str) != -1)
        {
            QString tmp = curGroup;
            QString value = regExpVariable.cap(2);

            if (!curGroup.isEmpty())
                tmp += "/";

            tmp += regExpVariable.cap(1);
            tmp = tmp.replace(QRegExp("\\s|\t"), "");

            value = value.trimmed();

            if (!map.contains(tmp))
            {
                map.insert(tmp, value);
            }
        }
    }

    delete secure;
    debug_message("[QRadSecureSettings] <--encryptedReadFunc true\n");
    return true;
}

bool encryptedWriteFunc(QIODevice &device, const QSettings::SettingsMap &map)
{
    debug_message("[QRadSecureSettings] -->encryptedWriteFunc\n");

    QRegExp regExpGroupPath(REGEXP_INI_GROUPPATH);
    QStringList groupList;
    QByteArray decryptedContent;
    QByteArray encryptedContent;
    QRadSecure *secure = new QRadSecure();

    if (!secure->initialize())
    {
        delete secure;
        debug_message("[QRadSecureSettings] <--encryptedWriteFunc false\n");
        return false;
    }


    if (!device.isOpen())
    {
        debug_message("[QRadSecureSettings] --encryptedWriteFunc Open-- L:%d\n", __LINE__);
        device.open(QIODevice::WriteOnly);
    }

    if( !device.isWritable() )
    {
        debug_message("[QRadSecureSettings] encryptedWriteFunc::fatal error, no writable file L:%d\n", __LINE__);
    }
    device.reset();

    decryptedContent.clear();
    encryptedContent.clear();

    bool firstLine = true;
    foreach (QString key, map.keys())
    {
        QString value = map.value(key).toString();
        QString group = "";
        QString variable = key;

        if (regExpGroupPath.indexIn(key) != -1)
        {
            group       = regExpGroupPath.cap(1);
            variable    = regExpGroupPath.cap(2);
        }

        if (!groupList.contains(group))
        {
            groupList.append(group);

            if (firstLine)
            {
                decryptedContent.append(QString("[%1]\n").arg(group));
                firstLine = false;
            }
            else
            {
                decryptedContent.append(QString("\n[%1]\n").arg(group));
            }
        }

        decryptedContent.append(QString("%1 = %2\n").arg(variable).arg(value));
    }

    encryptedContent = secure->encrypt(decryptedContent);

    device.seek(0);
    device.write(encryptedContent);

    delete secure;

    debug_message("[QRadSecureSettings] <--encryptedWriteFunc true\n");
    return true;
}
