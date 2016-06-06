#include "reportplugin.h" 
#include "qraddebug.h"
#include "pdfwrapper.h"

ReportPlugin::ReportPlugin()
{
}

ReportPlugin::~ReportPlugin()
{
}

void ReportPlugin::onLoad(QRadPluginContainer* container)
{
    m_container = container;

    appendAction("Build");
}

void ReportPlugin::Process( const QString& action )
{
    QStringList list;

    if (action.toLower() == QString("Build").toLower())
    {
       list.append("Linha de teste 1");
       list.append("Linha de teste 2");
       list.append("Linha de teste 3");
       list.append("Linha de teste 4");
       list.append("Linha de teste 5");
       list.append("Linha de teste 6");
       list.append("Linha de teste 7");
       list.append("Linha de teste 8");
       list.append("Linha de teste 9");
       list.append("Linha de teste 10");
       list.append("Linha de teste 11");
       list.append("Linha de teste 12");
       list.append("0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 01");

       pdfwrapper::Build( "RELATORIO_TESTE.PDF", "Titulo de Teste", "campo1 campo2 campo3 campo4", list );
    }
}

void ReportPlugin::setParam(QString str, QVariant v)
{
    Q_UNUSED(str);
    Q_UNUSED(v);
}

QVariant ReportPlugin::getParam(QString str)
{
    Q_UNUSED(str);
    return QVariant();
}

