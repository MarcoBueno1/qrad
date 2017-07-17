#include "reportplugin.h" 
#include "qraddebug.h"
#include "pdfwrapper.h"
#include <QCoreApplication>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlField>
#include <QDesktopServices>
#include <QUrl>

ReportPlugin::ReportPlugin()
{
   m_Title = "Relatorio";
   

   m_LeftHead.append(QString("DSM @2017"));
   m_LeftHead.append(QString("Manaus - AM"));
   m_LeftHead.append(QString("Watsapp +55 92 98415-1066"));
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
           if( m_percents.count() == nColumns )
              f->Percent = m_percents.at(i).toDouble();
           else
              f->Percent = 100/nColumns;

           if( m_aligns.count() == nColumns )
              f->Align   = m_aligns.at(i).toInt();
           else
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

        QString strAux =  QString("%1%2").arg(m_Title).arg(".pdf");
        if( 0 == pdfwrapper::Build( strAux , m_LeftHead, m_Title, headers, lines ))
             QDesktopServices::openUrl(QUrl(strAux, QUrl::TolerantMode));
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

       pdfwrapper::Build( "RelatorioDispositivos.pdf", m_LeftHead, "Relatório ATM CAIXA", headers, lines );
    }
   
}

void ReportPlugin::setParam(QString str, QVariant v)
{
  if( str == "Title" )
    m_Title = v.toString();
  else if( str == "SQL" )
    m_strSQL = v.toString();
  else if( str == "LeftHead" )
  {
    m_LeftHead.clear();
    m_LeftHead = v.toStringList();
  }
  else if( str == "Percents" )
  {
    m_percents.clear();
    m_percents = v.toStringList();
  }
  else if( str == "aligns" )
  {
    m_aligns.clear();
    m_aligns = v.toStringList();
  }
}

QVariant ReportPlugin::getParam(QString str)
{
    Q_UNUSED(str);
    return QVariant();
}

