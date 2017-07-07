#ifndef dsmModelLog_H
#define dsmModelLog_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QTime>


#define DSM_HISEVERITYLOG( log ) dsmModelLog::WriteHiSeverityLog(__FILE__,__LINE__,log,"HIS")

class dsmModelLog
{
public:
    dsmModelLog();

    static void WriteHiSeverityLog(QString fileName, int line, QString log, QString type);
private:

    static QString  FormatInfo(QString text, int size);
    static void AuxWriteLog(QString fileName,
                            int line,
                            QString log,
                            QString type,
                            QString fullpath);
};

#endif // dsmModelLog_H
