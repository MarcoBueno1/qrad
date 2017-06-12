#ifndef EXAROREPORT_H
#define EXAROREPORT_H

#include "reportengine.h"
#include "qradshared.h"
//#include "qradmoney.h"

#include "qradreportinterface.h"

class ExaroReport : public QRadReportInterface
{
public:
    ExaroReport(const QString& reportName = QString::null);
    ~ExaroReport();

    bool load(const QString& reportName = QString::null);
    bool show( void );
    bool print();
    bool printToPDF(QString &pdfFile);

    ReportCapabilities& capabilities() const;

    void setAttributeValue( QString attrName, QString value );
    void setAttributeValue( QString attrName, int value );
    void setAttributeValue( QString attrName, double value );
    void setAttributeValue( QString attrName, float value );
    void setAttributeValue( QString attrName, QDate value );
    void setAttributeValue( QString attrName, QTime value );
    void replace( QString strBefore, QString strAfter );
    void setQuery( QString queryName, QString query );
    void setFriendlyName( QString name);
private:
    bool                        m_hasContent;
    QString                     m_reportContent;
    QMap<QString, QVariant>     *m_queries;
    QString                     m_reportName;
    QString                     m_friendlyname;

    ReportCapabilities          *m_cap;
};

#endif // EXAROREPORT_H
