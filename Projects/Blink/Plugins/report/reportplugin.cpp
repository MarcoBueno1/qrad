#include "reportplugin.h" 
#include "qraddebug.h"
#include "pdfwrapper.h"
#include <QCoreApplication>

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
      list.append("Linha de teste 13");
      list.append("Linha de teste 14");
      list.append("Linha de teste 15");	  
      list.append("Linha de teste 16");
      list.append("Linha de teste 17");
      list.append("Linha de teste 18");	  
      list.append("Linha de teste 19");
      list.append("Linha de teste 20");
      list.append("Linha de teste 21");	  	  
	  list.append("Linha de teste 22");
      list.append("Linha de teste 23");
      list.append("Linha de teste 24");	  
      list.append("Linha de teste 25");
      list.append("Linha de teste 26");
      list.append("Linha de teste 27");	  
      list.append("Linha de teste 28");
      list.append("Linha de teste 29");
      list.append("Linha de teste 30");	  
      list.append("Linha de teste 31");
      list.append("Linha de teste 32");
      list.append("Linha de teste 33");	  	  
	  list.append("Linha de teste 34");
      list.append("Linha de teste 35");
      list.append("Linha de teste 36");	  
      list.append("Linha de teste 37");
      list.append("Linha de teste 38");
      list.append("Linha de teste 39");	  
      list.append("Linha de teste 40");
      list.append("Linha de teste 41");
      list.append("Linha de teste 42");	  
      list.append("Linha de teste 43");
      list.append("Linha de teste 44");
      list.append("Linha de teste 45");	  	  
	  list.append("Linha de teste 46");
      list.append("Linha de teste 47");
      list.append("Linha de teste 48");	  
	  list.append("Linha de teste 49");
      list.append("Linha de teste 50");
      list.append("Linha de teste 51");	  
      list.append("Linha de teste 52");
      list.append("Linha de teste 53");
      list.append("Linha de teste 54");	  
      list.append("Linha de teste 55");
      list.append("Linha de teste 56");
      list.append("Linha de teste 57");	  	  
	  list.append("Linha de teste 58");
      list.append("Linha de teste 59");
      list.append("Linha de teste 60");	  
      list.append("Linha de teste 61");	  
      list.append("Linha de teste 62");
      list.append("Linha de teste 63");
      list.append("Linha de teste 64");	  
      list.append("Linha de teste 65");
      list.append("Linha de teste 66");
      list.append("Linha de teste 67");	  	  
	  list.append("Linha de teste 68");
      list.append("Linha de teste 69");
      list.append("Linha de teste 70");	  	  
       pdfwrapper::Build( "RELATORIO_TESTE.PDF", "Titulo de Teste", "campo1 campo2 campo3 campo4", list );
    }
   else
   {

       QList< FieldFormat *> headers;
//       QStringList fields;
       QList<QStringList> lines;

       FieldFormat *f = (FieldFormat *)malloc(sizeof(FieldFormat));
       strcpy(f->Name, "ATM");
       //    = QString();
       f->Percent = 30;
       f->Align   = ALIGN_CENTER;
       headers.append(f);

       f = (FieldFormat *)malloc(sizeof(FieldFormat));
       strcpy(f->Name, "Perifericos");
       //f->Name    = QString();
       f->Percent = 40;
       f->Align   = ALIGN_LEFT;
       headers.append(f);

       f = (FieldFormat *)malloc(sizeof(FieldFormat));
       strcpy(f->Name, "Conexao");
       //f->Name    = QString("Conexão");
       f->Percent = 30;
       f->Align   = ALIGN_CENTER;
       headers.append(f);


       QStringList linha1; linha1.append("CAIXA"); linha1.append("Dispensador, depositário"); linha1.append("USB" );
       lines.append( linha1 );

       QStringList linha2; linha2.append("ITAU"); linha2.append("Dispensador, depositário, LCB"); linha2.append("USB, SERIAL" );
       lines.append( linha2 );

       QStringList linha3; linha3.append("BB"); linha3.append("Depositário, DIP, IMPRESSORA"); linha3.append("USB, SERIAL, PARALELA" );
       lines.append( linha3 );

       QStringList linha4; linha4.append("BANCOB"); linha4.append("Depositário"); linha4.append("USB, SERIAL" );
       lines.append( linha4 );

       QStringList linha5; linha5.append("BANRISUL"); linha5.append("AFD, DEP, LCB, DIP"); linha5.append("USB,SERIAL" );
       lines.append( linha5 );

       QStringList linha6; linha6.append("SANTANDER"); linha6.append("Dispensador, depositário, LCB"); linha6.append("PARALELA, SERIAL" );
       lines.append( linha6 );


       pdfwrapper::Build( "RelatorioDispositivos.pdf", "Relatório ATM CAIXA", headers, lines );
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

