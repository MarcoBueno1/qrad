
/**
 * @defgroup report Relatórios
 *
 * Agrupamento dos relatórios implementados no QRad
 */
#include "qradreportmanager.h"
#include "qraddebug.h"
#include <QSqlQueryModel>

QRadReportManager::QRadReportManager()
{
    m_report = NULL;
    m_lastError = "";
}

QRadReportManager::~QRadReportManager()
{
    delete m_report;
}


bool QRadReportManager::load(const QString& reportName)
{
    if(!CanExecute())
    {
        debug_message("O relatório não pode ser exibido pois a licença não é válida: %s", reportName.toUtf8().data());
        m_lastError = QString::fromUtf8("Sua licença expirou!\nPara visualizar seus relatórios entre em contato com nosso suporte e solicite a renovação de sua licença.");
        return false;
    }

    m_report = ReportFactory::getReport(reportName);

    if (NULL == m_report)
    {
        debug_message("[QRadReportManager] m_report == NULL\n");
        return false;
    }

    return m_report->load();
}

bool QRadReportManager::show()
{
    if (NULL == m_report)
        return false;

    return m_report->show();
}

void QRadReportManager::setAttributeValue( QString attrName, QString value )
{
    if (NULL != m_report)
        m_report->setAttributeValue(attrName, value);

}

void QRadReportManager::setAttributeValue( QString attrName, int value )
{
    if (NULL != m_report)
        m_report->setAttributeValue(attrName, value);
}

void QRadReportManager::setAttributeMoneyValue( QString attrName, int value )
{
    QString money = QString::number( value );

    setAttributeValue( attrName, QRadMoney::MoneyHumanForm4(money));
}

void QRadReportManager::setAttributeValue( QString attrName, double value )
{
    if (NULL != m_report)
        m_report->setAttributeValue(attrName, value);
}

void QRadReportManager::setAttributeValue( QString attrName, float value )
{
    if (NULL != m_report)
        m_report->setAttributeValue(attrName, QRadMoney::MoneyHumanForm2( value ));
}

void QRadReportManager::setAttributeValue( QString attrName, QDate value )
{
    if (NULL != m_report)
        m_report->setAttributeValue(attrName, value.toString( FMT_DATE ));
}

void QRadReportManager::setAttributeValue( QString attrName, QTime value )
{
    if (NULL != m_report)
        m_report->setAttributeValue(attrName, value.toString( FMT_TIME ));
}

void QRadReportManager::replace(QString oldValue, QString newValue)
{
    if (NULL != m_report)
        m_report->replace(oldValue, newValue);
}

void QRadReportManager::setQuery( QString queryName, QString query )
{
    if (NULL != m_report)
        m_report->setQuery(queryName, query);
}

bool QRadReportManager::CanExecute()
{
#if 0
#define DEVELOPMENT_VERSION 1
#if (!DEVELOPMENT_VERSION)
    Clicense objLicense;

    if( QRad_SUCCESS == objLicense.IsValidlicense())
    {
        return true;
    }
    else
    {
        return false;
    }
#else
    return true;
#endif
#endif
    return true;
}

// id |          name           |       fantasyname       |      cnpj      | ie | phone | email | city | state |    cep    |     address      | neighborhood | housenumber | complement  | max | licence | lastlicense 



#define SQL_USE_LICENSE  "SELECT ul.name, 'CEP: '|| substring(cep " \
"from 1 for 5)||'-'|| " \
"substring(cep " \
"from 5 for 3) as cep, ' - '|| 'FONE: ' || '(' ||substring(phone from 1 for 2)||')'|| " \
"substring(phone from 3 for 4)||'-'||substring(phone from 5 for 4) as fone, " \
"address || ' - ' || neighborhood || ' - '|| city.name ||'/'|| uf.sign as localidade, " \
"'CNPJ: ' || substring(ul.cnpj from 1 for 2)||'.'|| substring(ul.cnpj from 3 for 3)||'.'|| " \
"substring(ul.cnpj from 6 for 3)||'/'|| substring(ul.cnpj from 9 for 4)||'-'|| " \
"substring(ul.cnpj from 13 for 2) as cnpjcliente " \
"FROM maincompany ul INNER JOIN state uf on ul.state = uf.id INNER JOIN city on ul.city=city.id"


void QRadReportManager::ShowSetHeaderClientReport(QRadReportManager *report)
{
    QSqlQueryModel model;

    model.setQuery(SQL_USE_LICENSE);

    report->setAttributeValue("COMPANY_NAME",model.record(0).value("name").toString());
    report->setAttributeValue("LOCALIDADE",model.record(0).value("localidade").toString());
    report->setAttributeValue("CEP",model.record(0).value("cep").toString());
    report->setAttributeValue("CNPJ",model.record(0).value("cnpjcliente").toString());
    report->setAttributeValue("DATA_EMISSAO",QDate::currentDate().toString(FMT_DATE));
    report->setAttributeValue("HORA_EMISSAO",QTime::currentTime().toString(FMT_TIME));
}


QString QRadReportManager::lastError()
{
    return m_lastError;
}

bool QRadReportManager::printToPDF(QString strFile)
{
    if (NULL == m_report)
        return false;

    m_report->printToPDF(strFile);

    return true;
}
void QRadReportManager::setFriendlyName( QString name )
{
    m_report->setFriendlyName(name);
}

