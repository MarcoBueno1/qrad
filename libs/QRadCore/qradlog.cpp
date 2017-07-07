#include "qradlog.h"
#include "qradconfig.h"
/**
  () - inteiros
  [] - floats
  {} - strings
  */

#ifdef __WIN32__
#define QRad_PATH "C:\\lnx\\"
#else
#define QRad_PATH "/opt/QRad/bin/"
#endif


QRadLog::QRadLog()
{
}

void QRadLog::Log(QString fileName, QString log)
{
    WriteLog(fileName, log, "LOG");
}

void QRadLog::LogError(QString fileName, QString log)
{
    WriteLog(fileName, log, "ERR");
}

void QRadLog::WriteLog(QString fileName, QString log, QString type)
{
    if (1)
    {
        QString path, timeStamp, user;

        path = LOG_FILE_PATH;
        QFile file(path);

        if (file.size() > 5 * 1024 * 1024)
        {
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            QString text = file.readAll();
            file.close();

            if (text.contains("|ERR|"))
            {
                QProcess process;
                process.execute(QString("tar -jcvf %1%2-%3.tar.bz2 %4")
                                .arg((QFile::exists(DIR_INSTALL)?DIR_INSTALL:""))
                                .arg(QDate::currentDate().toString(FMT_DATE_LOG2))
                                .arg(QTime::currentTime().toString(FMT_TIME_LOG))
                                .arg(LOG_FILE_PATH));
                process.waitForFinished();
            }
            file.remove(LOG_FILE_PATH);
        }

        fileName = fileName.mid(fileName.lastIndexOf("/")+1);
        fileName.replace(".cpp", "");
        fileName = FormatInfo(fileName, 10) + "|";

        user = FormatInfo(QRadConfig::GetCurrentUserLogin(), 7);

        if (!file.open(QIODevice::Append | QIODevice::Text)) return;

        QTextStream out(&file);

        timeStamp = QDate::currentDate().toString(FMT_DATE_LOG) + "|" +
                    QTime::currentTime().toString(FMT_TIME) + "|" +
                    type + "|" + user + "|";

        out << timeStamp << fileName << log << endl;

        file.close();
    }
}

void QRadLog::Error(QString fileName, int line, QString log,  long thid)
{
    WriteServiceLog(fileName, line, log, "ERR", thid);
}

void QRadLog::Trace(QString fileName, int line, QString log, long thid)
{
    WriteServiceLog(fileName, line, log, "LOG", thid);
}

void QRadLog::Trace(QString fileName, int line, QString log, QString strName)
{
    WriteServiceLog(fileName, line, log, "LOG", strName);
}


void QRadLog::WriteServiceLog(QString fileName, int line, QString log, QString type, long thid)
{
    QDate date = QDate::currentDate();
    //if ((QFile::GetParamValue("QRad-paf/servicelog") == "true") ||  (QFile::GetParamValue("replicator/servicelog") == "true") || (QFile::GetParamValue("integrator/servicelog") == "true"))
    {
        QString path, timeStamp, user;

        path.sprintf("QRad-s-%02d-%02d-%02d-tid%lu.txt",
                     date.day(),
                     date.month(),
                     date.year(),
                     thid);

        QFile file(path);

        if (file.size() > 5 * 1024 * 1024)
        {
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            QString text = file.readAll();
            file.close();

            if (text.contains("|ERR|"))
            {
                QProcess process;
                process.execute(QString("tar -jcvf %1%2-%3.tar.bz2 %4")
                                .arg((QFile::exists(DIR_INSTALL)?DIR_INSTALL:""))
                                .arg(QDate::currentDate().toString(FMT_DATE_LOG2))
                                .arg(QTime::currentTime().toString(FMT_TIME_LOG))
                                .arg(LOG_FILE_PATH));
                process.waitForFinished();
            }
            file.remove(path);
        }

        fileName = fileName.mid(fileName.lastIndexOf("/")+1);
        fileName.replace(".cpp", "");
        fileName = FormatInfo(fileName, 10) + "|";

        user.sprintf("L:%04d", line);

        if (!file.open(QIODevice::Append | QIODevice::Text)) return;

        QTextStream out(&file);

        timeStamp = QDate::currentDate().toString(FMT_DATE_LOG) + "|" +
                    QTime::currentTime().toString(FMT_TIME_PLUS) + "|" +
                    type + "|" + user + "|";

        out << timeStamp << fileName << log << endl;

        file.flush();
        file.close();
    }
}


void QRadLog::WriteServiceLog(QString fileName, int line, QString log, QString type, QString strName)
{
    QDate date = QDate::currentDate();
  //  if ((QFile::GetParamValue("QRad-paf/servicelog") == "true") ||  (QFile::GetParamValue("replicator/servicelog") == "true") || (QFile::GetParamValue("integrator/servicelog") == "true"))
    {
        QString path, timeStamp, user;

        path.sprintf("QRad-s-%02d-%02d-%02d-",
                     date.day(),
                     date.month(),
                     date.year());

        path+= strName + ".txt";

        QFile file(path);

        if (file.size() > 5 * 1024 * 1024)
        {
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            QString text = file.readAll();
            file.close();

            if (text.contains("|ERR|"))
            {
                QProcess process;
                process.execute(QString("tar -jcvf %1%2-%3.tar.bz2 %4")
                                .arg((QFile::exists(DIR_INSTALL)?DIR_INSTALL:""))
                                .arg(QDate::currentDate().toString(FMT_DATE_LOG2))
                                .arg(QTime::currentTime().toString(FMT_TIME_LOG))
                                .arg(LOG_FILE_PATH));
                process.waitForFinished();
            }
            file.remove(path);
        }

        fileName = fileName.mid(fileName.lastIndexOf("/")+1);
        fileName.replace(".cpp", "");
        fileName = FormatInfo(fileName, 10) + "|";

        user.sprintf("L:%04d", line);

        if (!file.open(QIODevice::Append | QIODevice::Text)) return;

        QTextStream out(&file);

        timeStamp = QDate::currentDate().toString(FMT_DATE_LOG) + "|" +
                    QTime::currentTime().toString(FMT_TIME_PLUS) + "|" +
                    type + "|" + user + "|";

        out << timeStamp << fileName << log << endl;

        file.flush();
        file.close();
    }
}



void QRadLog::WriteNFeLog(QString fileName, int line, QString log, QString type)
{
    QDate date = QDate::currentDate();
    if(1)
    {
        QString path, timeStamp, user;

        path.sprintf("%sQRad-nfe-%02d-%02d-%02d.txt",
                     QRad_PATH,
                     date.day(),
                     date.month(),
                     date.year());

        QFile file(path);


        fileName = fileName.mid(fileName.lastIndexOf("/")+1);
        fileName.replace(".cpp", "");
        fileName = FormatInfo(fileName, 10) + "|";

        user.sprintf("L:%04d", line);

        if (!file.open(QIODevice::Append | QIODevice::Text))
        {
            return;
        }

        QTextStream out(&file);

        timeStamp = QDate::currentDate().toString(FMT_DATE_LOG) + "|" +
                    QTime::currentTime().toString(FMT_TIME_PLUS) + "|" +
                    type + "|" + user + "|";

        out << timeStamp << fileName << log << endl;

        file.flush();
        file.close();
    }
}


void QRadLog::WriteTransportLog(QString fileName, int line, QString log, QString type)
{
    QDate date = QDate::currentDate();
    if(1)
    {
        QString path, timeStamp, user;

        path.sprintf("%sQRad-transport-%02d-%02d-%02d.txt",
                     QRad_PATH,
                     date.day(),
                     date.month(),
                     date.year());

        QFile file(path);


        fileName = fileName.mid(fileName.lastIndexOf("/")+1);
        fileName.replace(".cpp", "");
        fileName = FormatInfo(fileName, 10) + "|";

        user.sprintf("L:%04d", line);

        if (!file.open(QIODevice::Append | QIODevice::Text))
        {
            return;
        }

        QTextStream out(&file);

        timeStamp = QDate::currentDate().toString(FMT_DATE_LOG) + "|" +
                    QTime::currentTime().toString(FMT_TIME_PLUS) + "|" +
                    type + "|" + user + "|";

        out << timeStamp << fileName << log << endl;

        file.flush();
        file.close();
    }
}


QString QRadLog::FormatInfo(QString text, int size)
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
