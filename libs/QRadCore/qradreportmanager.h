#ifndef QRadREPORT_H
#define QRadREPORT_H

#include <QtCore>
#include <QMap>

#include "qradconfig.h"
#include "qradmoney.h"
//#include "dsmshared.h"
//#include "version.h"
#include "reportfactory.h"

class QRadReportManager
{
public:
    QRadReportManager();
    ~QRadReportManager();

    bool load(const QString& reportName);
    bool show(void);
    bool print(void);
    bool printToPDF(QString strFile);

    ReportCapabilities& capabilities() const;

    void setAttributeValue( QString attrName, QString value );
    void setAttributeValue( QString attrName, int value );
    void setAttributeValue( QString attrName, double value );
    void setAttributeValue( QString attrName, float value );
    void setAttributeValue( QString attrName, QDate value );
    void setAttributeValue( QString attrName, QTime value );
    void setAttributeMoneyValue( QString attrName, int value );
    void replace(QString, QString);
    void setQuery( QString queryName, QString query );
    bool CanExecute();
    void ShowSetHeaderClientReport(QRadReportManager *report);
    void setFriendlyName( QString name );
    QRadReportInterface *getReport(){return m_report;}

    QString lastError();

private:
    QRadReportInterface             *m_report;
    QString                         m_lastError;
};

#endif // QRadREPORT_H
