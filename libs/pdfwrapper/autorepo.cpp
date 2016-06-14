#include "autorepo.h"
#include "pdfwrapper.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlField>
#include <QUrl>
#include <QDesktopServices>

/*Anotaçoes:

      model->setHeaderData(0, Qt::Horizontal, tr("Name"));
      model->setHeaderData(1, Qt::Horizontal, tr("obs"));
	  
Aparentemente pode-se usar:

      for( int i= 0; i < model->columnCount(), i++ )
	  {
		  Variant  var = model->headerData(i, Qt::Horizontal );
		  Variant var2 = model->headerData(i, Qt::Horizontal );
	  }
*/
AutoRepo::AutoRepo()
{

}
AutoRepo::~AutoRepo()
{
	
}


bool AutoRepo::Build(QString FileName, QString Title, QString Sql)
{
  int i;
  int j;
  
  QStringList *pStringList;
  QList<QStringList *> lines;
  QList< FieldFormat *> headers;
  QSqlQueryModel *pModel = new QSqlQueryModel;
  
  
  pModel->setQuery(Sql);
  if( !pModel->rowCount() )
  {
    delete pModel;
    return false;	
  }
  
  for( i = 0; i < pModel->columnCount(); i++ )
  {
	  QString strHeader = pModel->headerData(i, Qt::Horizontal ).toString();
	  if( !strHeader.trimmed().isEmpty() )
	  {
		   FieldFormat *f = (FieldFormat *)malloc(sizeof(FieldFormat));
		   strcpy(f->Name, strHeader.toUtf8().data());
		   //    = QString();
		   f->Percent = 100/pModel->columnCount();
		   f->Align   = ALIGN_CENTER; // default;
           headers.append(f);
	  }
  }
  for( j = 0; j < pModel->rowCount(); j++ )
  {
	  pStringList = new QStringList;
	  QSqlRecord record = pModel->record(j);
	  
	  for( i = 0; i < pModel->columnCount(); i++ )
	  {
		  QSqlField field = record.field(i);
		  pStringList->append(field.value().toString());
	  }
	  lines.append(pStringList);
  }
     
  delete pModel;

  bool bRet = (bool)!pdfwrapper::Build( FileName, Title, headers, lines );
  if( bRet )
  {  
	  QUrl url(FileName);
	  QDesktopServices::openUrl(url);  
  }
  for( i = 0; i < lines.count(); i++ )
  {
	  
	  delete lines.at(i);
	  lines.removeAt(i);
  }
  for( i= 0; i < headers.count(); i++)
  {
	  free( headers.at(i) );
	  headers.removeAt(i);
  }

  return bRet;
}
