#include "exaroreport.h"
#include "qraddebug.h"
#include "qradmoney.h"

#define QRad_CHANGE_PICUTURE_CMD  "@#m2smart#@"
#define QRad_REPORT_ATTRIBUTE( attrName )    "{" + attrName + "}"

Report::ReportEngine        *m_reportEngine;
Report::ReportInterface     *m_reportInterface;


ExaroReport::ExaroReport(const QString &reportName)
{
    m_hasContent = false;

    m_reportInterface = NULL;
    m_reportEngine = new Report::ReportEngine();
    m_queries = new QMap<QString, QVariant>();

    m_cap = new ReportCapabilities;

    m_cap->capAttributeChange = true;
    m_cap->capPreview = true;
    m_cap->capPrint = false;
    m_cap->capPrintToPDF = false;

    m_reportName = reportName;
}

ExaroReport::~ExaroReport()
{
    delete m_cap;
    delete m_queries;
    delete m_reportEngine;
}

ReportCapabilities& ExaroReport::capabilities() const
{
    return *m_cap;
}

bool ExaroReport::load(const QString& reportName)
{
    if (QString::null != reportName)
        m_reportName = reportName;

    QFile file(m_reportName);

    if ( !file.open( QFile::ReadOnly ) )
    {
        debug_message("[ExaroReport.cpp]Trying to load: %s = false\n",m_reportName.toUtf8().data());
        return false;
    }

    m_reportContent = QString::fromUtf8(file.readAll());
    m_hasContent = true;

    debug_message("[ExaroReport.cpp]Trying to load: %s\n",m_reportName.toUtf8().data());
    qApp->processEvents();
    return true;
}

bool ExaroReport::show()
{
    debug_message("[ExaroReport.cpp]Trying show: %s\n",m_reportName.toUtf8().data());
    m_reportInterface = m_reportEngine->loadReportFromString( m_reportContent);

    if ( NULL == m_reportInterface )
    {
        debug_message("[ExaroReport.cpp]Trying show: %s = false\n",m_reportName.toUtf8().data());
        return false;
    }
    debug_message("[ExaroReport.cpp]Trying show: Content \n%s\n",m_reportContent.toUtf8().data());


    m_reportInterface->setShowSplashScreen( true );
    qApp->processEvents();

    // set report queries
    if ( m_queries->count() > 0 )
    {
        m_reportInterface->setQueries( *m_queries );
    }
    m_reportInterface->setShowSplashScreen( true );
    qApp->processEvents();


    debug_message("[ExaroReport.cpp]Trying show: %s\n",m_reportName.toUtf8().data());

    if( m_friendlyname.isEmpty())
        m_friendlyname = QString::fromUtf8("Relatório");

    m_reportInterface->setName(m_friendlyname);
    return m_reportInterface->exec();
}

void ExaroReport::setFriendlyName( QString name)
{
    m_friendlyname = name;
}


void ExaroReport::replace( QString strBefore, QString strAfter )
{
    m_reportContent.replace( strBefore, strAfter);
}

void ExaroReport::setAttributeValue( QString attrName, QString value )
{
    if( attrName.contains(QRad_CHANGE_PICUTURE_CMD))
    {
        QString strSrc = attrName.remove(QRad_CHANGE_PICUTURE_CMD);
        debug_message( "strSrc:%s\nValue:%s\n", strSrc.toLatin1().data(), value.toLatin1().data());

        m_reportContent.replace( strSrc, value);
    }
    else
    {
        value.replace("&","&amp;");
        value.replace("<", "&lt;");
        m_reportContent.replace( QRad_REPORT_ATTRIBUTE( attrName ), value );
    }
}

void ExaroReport::setAttributeValue( QString attrName, int value )
{
    setAttributeValue( attrName, QString::number( value ) );
}

void ExaroReport::setAttributeValue( QString attrName, double value )
{
    setAttributeValue( attrName, QRadMoney::MoneyHumanForm2( value ) );
}

void ExaroReport::setAttributeValue( QString attrName, float value )
{
    setAttributeValue( attrName, QRadMoney::MoneyHumanForm2( value ) );
}

void ExaroReport::setAttributeValue( QString attrName, QDate value )
{
    setAttributeValue( attrName, value.toString( FMT_DATE ) );
}

void ExaroReport::setAttributeValue( QString attrName, QTime value )
{
    setAttributeValue( attrName, value.toString( FMT_TIME ) );
}

void ExaroReport::setQuery( QString queryName, QString query )
{
    m_queries->insert( queryName, query );
}

bool ExaroReport::print()
{
    return false;
}

bool ExaroReport::printToPDF(QString &pdfFile)
{
    Q_UNUSED(pdfFile);
    return false;
}

void ExaroReport::setTitles( QString Title1, QString Title2, QString Title3, QString Title4, QString Title5, QString Title6,
                             QString Title7, QString Title8, QString Title9, QString Title10 )
{
    replace( "{TITLE1}", Title1 );
    replace( "{TITLE2}", Title2 );
    replace( "{TITLE3}", Title3 );
    replace( "{TITLE4}", Title4 );
    replace( "{TITLE5}", Title5 );
    replace( "{TITLE6}", Title6 );
    replace( "{TITLE7}", Title7 );
    replace( "{TITLE8}", Title8 );
    replace( "{TITLE9}", Title9 );
    replace( "{TITLE10}", Title10 );
}
void ExaroReport::setFields( QString Field1, QString Field2, QString Field3, QString Field4, QString Field5, QString Field6,
                             QString Field7, QString Field8, QString Field9, QString Field10 )
{
    replace( "{FIELD1}", Field1 );
    replace( "{FIELD2}", Field2 );
    replace( "{FIELD3}", Field3 );
    replace( "{FIELD4}", Field4 );
    replace( "{FIELD5}", Field5 );
    replace( "{FIELD6}", Field6 );
    replace( "{FIELD7}", Field7 );
    replace( "{FIELD8}", Field8 );
    replace( "{FIELD9}", Field9 );
    replace( "{FIELD10}", Field10 );
}

void ExaroReport::setQueryName( QString QueryName )
{
    replace( "{QNAME}", QueryName );

}
void ExaroReport::setReportTitle( QString ReportTitle )
{
    replace( "{TITLE}", ReportTitle );
}

