#include "reportfactory.h"
//#include "version.h"

#define EXARO_EXTENSION ".bdrt"
#define RPT_EXTENSION   ".xml"

QRadReportInterface* ReportFactory::getReport(const QString &report)
{
    QString fileName = ":/reports/" + report;

    // test the report type

    QFile file;

    if (file.exists(fileName + EXARO_EXTENSION))
        return new ExaroReport(fileName + EXARO_EXTENSION);

//    if (file.exists(fileName + RPT_EXTENSION))
//        return new RptReport(fileName + RPT_EXTENSION);

    // report file not found
    qWarning() << __FUNCTION__ << "Report NOT Found" << fileName;

    return NULL;
}
