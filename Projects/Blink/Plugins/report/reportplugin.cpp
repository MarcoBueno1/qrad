#include "reportplugin.h" 
#include "qraddebug.h"
#include "pdfwrapper.h"
#include <QCoreApplication>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlField>

ReportPlugin::ReportPlugin()
{
   m_Title = "Relatorio";
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
   QList< FieldFormat *> headers;
   QList<QStringList *> lines;
   QStringList LeftHead;

   LeftHead.append("Diebold Inc. @2016");
   LeftHead.append("Distrito Industrial");
   LeftHead.append("Manaus - AM");

   if( action.contains( "REPORT_SQL"))
   {
        int i;
        QSqlQueryModel *model  = new QSqlQueryModel;
        model->setQuery(m_strSQL);

        QStringList *line;
        int nColumns = model->columnCount();
        for( i = 0; i < nColumns; i++ )
        {
           QString strAux = model->headerData(i, Qt::Horizontal).toString();
           FieldFormat *f = (FieldFormat *)malloc(sizeof(FieldFormat));
           strcpy(f->Name,strAux.toLatin1().data());
           f->Percent = 100/nColumns;
           f->Align   = ALIGN_CENTER;
           headers.append(f);
        }


        for(  i = 0; i < model->rowCount(); i++ )
        {
          QSqlRecord rec = model->record(i);
          line = new QStringList;
          for( int j = 0; j < nColumns; j++ )
          {
            line->append(rec.field(j).value().toString() )    ;
          }
          lines.append(line);    

        }

        pdfwrapper::Build( QString("%1%2").arg(m_Title).arg(".pdf"), LeftHead, m_Title, headers, lines );
   }
   else
   {
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


       QStringList *linha1 = new QStringList; linha1->append("CAIXA"); linha1->append("Dispensador, depositário"); linha1->append("USB" );
       lines.append( linha1 );

       QStringList *linha2 = new QStringList; linha2->append("ITAU"); linha2->append("Dispensador, depositário, LCB"); linha2->append("USB, SERIAL" );
       lines.append( linha2 );

       QStringList *linha3 =  new QStringList; linha3->append("BB"); linha3->append("Depositário, DIP, IMPRESSORA"); linha3->append("USB, SERIAL, PARALELA" );
       lines.append( linha3 );

       QStringList *linha4 = new QStringList; linha4->append("BANCOB"); linha4->append("Depositário"); linha4->append("USB, SERIAL" );
       lines.append( linha4 );

       QStringList *linha5 = new QStringList; linha5->append("BANRISUL"); linha5->append("AFD, DEP, LCB, DIP"); linha5->append("USB,SERIAL" );
       lines.append( linha5 );

       QStringList *linha6 = new QStringList; linha6->append("SANTANDER"); linha6->append("Dispensador, depositário, LCB"); linha6->append("PARALELA, SERIAL" );
       lines.append( linha6 );

       pdfwrapper::Build( "RelatorioDispositivos.pdf", LeftHead, "Relatório ATM CAIXA", headers, lines );
    }
   
}

void ReportPlugin::setParam(QString str, QVariant v)
{
  if( str == "Title" )
    m_Title = v.toString();
  else if( str == "SQL" )
    m_strSQL = v.toString();
}

QVariant ReportPlugin::getParam(QString str)
{
    Q_UNUSED(str);
    return QVariant();
}

