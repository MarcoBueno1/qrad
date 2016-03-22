#include "qradmodellog.h"

QRadModelLog::QRadModelLog()
{
}

void QRadModelLog::AuxWriteLog(QString fileName, int line, QString log, QString type,
                         QString fullpath)
{
    QString timeStamp, user;

    QFile file(fullpath);


    fileName = fileName.mid(fileName.lastIndexOf("/")+1);
    fileName.replace(".cpp", "");
    fileName = FormatInfo(fileName, 10) + "|";

    user.sprintf("L:%04d", line);

    if (!file.open(QIODevice::Append | QIODevice::Text)) return;

    QTextStream out(&file);

    timeStamp = QDate::currentDate().toString("dd-MM-yyyy") + "|" +
                QTime::currentTime().toString("HH:mm:ss.zzz") + "|" +
                type + "|" + user + "|";

    out << timeStamp << fileName << log << endl;

    file.flush();
    file.close();
}

void QRadModelLog::WriteHiSeverityLog(QString fileName, int line, QString log, QString type)
{
    QString path;
    QDate date = QDate::currentDate();

    path.sprintf("modelerror-%02d-%02d-%02d.txt",
          date.day(),
          date.month(),
          date.year());

    AuxWriteLog(fileName, line, log, type, path);
}

QString QRadModelLog::FormatInfo(QString text, int size)
{
    if (text.length() > size)
    {
        text = text.mid(0,size);
    }
    else
    {
        int length = text.length();
        for (int i = 0; i < (size - length); i++)
        {
            text.append(" ");
        }
    }
    return text;
}
