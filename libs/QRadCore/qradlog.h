#ifndef QRadLOG_H
#define QRadLOG_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QTime>
#include "qradshared.h"

#define QRad_SRVERR( log ) QRadLog::Error(__FILE__, __LINE__,log, (long)QThread::currentThreadId())
#define QRad_SRVLOG( log ) QRadLog::Trace(__FILE__, __LINE__,log, (long)QThread::currentThreadId())
#define QRad_SRVLOGNF( log ) QRadLog::Trace( "", 0, log, (long)QThread::currentThreadId())
#define QRad_NFELOG( log ) QRadLog::WriteNFeLog(__FILE__, __LINE__,log, "INF");
#define QRad_NFELOG_ERR( log ) QRadLog::WriteNFeLog(__FILE__, __LINE__,log, "ERR");
#define QRad_TRANSPORTLOG( log ) QRadLog::WriteTransportLog(__FILE__, __LINE__,log, "INF");

#define QRad_SRVERRTH( log ) QRadLog::Error(__FILE__, __LINE__,log, currentThreadId())
#define QRad_SRVLOGTH( log ) QRadLog::Trace(__FILE__, __LINE__,log, currentThreadId())
#define QRad_SRVLOGNFTH( log ) QRadLog::Trace( "", 0, log, QThread::currentThreadId())

#define QRad_SRVLOGSN( log ) QRadLog::Trace( "", 0, log, m_storename );


class QRadLog
{
public:
    QRadLog();

    static void     Log(QString fileName, QString log);
    static void     LogError(QString fileName, QString error);
    static void     LogWarning(QString fileName, QString warning);

    /** funcao interna - NAO USAR */
    static void     WriteLog(QString fileName, QString log, QString type);
    static QString  FormatInfo(QString text, int size);

    static void Error(QString fileName, int line, QString log, long thid=0);
    static void Trace(QString fileName, int line,QString log, long thid=0);
    static void Trace(QString fileName, int line,QString log, QString strName="");
    static void WriteServiceLog(QString fileName, int line, QString log, QString type, long thid);
    static void WriteServiceLog(QString fileName, int line, QString log, QString type, QString strName);
    static void WriteNFeLog(QString fileName, int line, QString log, QString type);
    static void WriteTransportLog(QString fileName, int line, QString log, QString type);
};

#endif // QRadLOG_H
