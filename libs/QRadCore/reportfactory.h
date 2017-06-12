#ifndef REPORTMANAGER_H
#define REPORTMANAGER_H

#include "exaroreport.h"
//#include "rptreport.h"

class ReportFactory
{
public:
    static QRadReportInterface* getReport(const QString& report);
};

#endif // REPORTMANAGER_H
