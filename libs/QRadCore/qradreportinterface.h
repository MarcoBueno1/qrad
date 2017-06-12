#ifndef QRadREPORTINTERFACE_H
#define QRadREPORTINTERFACE_H

#include <QtCore>
#include <QMap>

typedef struct ReportCapabilities_tag
{
   bool capPreview;
   bool capPrint;
   bool capPrintToPDF;
   bool capAttributeChange;
   bool capQueryChange;
}
ReportCapabilities;

class QRadReportInterface
{
public:
    virtual ~QRadReportInterface() {}

    virtual bool load(const QString& reportName = QString::null) = 0;
    virtual bool show( void ) = 0;
    virtual bool print() = 0;
    virtual bool printToPDF(QString& pdfFile) = 0;
    virtual ReportCapabilities& capabilities() const = 0;

    virtual void setAttributeValue( QString attrName, QString value ) = 0;
    virtual void setAttributeValue( QString attrName, int value ) = 0;
    virtual void setAttributeValue( QString attrName, double value ) = 0;
    virtual void setAttributeValue( QString attrName, float value ) = 0;

    virtual void replace( QString oldValue, QString newValue ) = 0;
    virtual void setQuery( QString queryName, QString query ) = 0;
    virtual void setFriendlyName( QString name) = 0;
};

#endif // QRadREPORTINTERFACE_H
