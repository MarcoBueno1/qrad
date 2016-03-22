#ifndef QRadModelLog_H
#define QRadModelLog_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QTime>


#define QRad_HISEVERITYLOG( log ) QRadModelLog::WriteHiSeverityLog(__FILE__,__LINE__,log,"HIS")

class QRadModelLog
{
public:
    QRadModelLog();

    static void WriteHiSeverityLog(QString fileName, int line, QString log, QString type);
private:

    static QString  FormatInfo(QString text, int size);
    static void AuxWriteLog(QString fileName,
                            int line,
                            QString log,
                            QString type,
                            QString fullpath);
};

#endif // QRadModelLog_H
