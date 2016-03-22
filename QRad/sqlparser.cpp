#include "sqlparser.h"
#include <QFile>
#include <foreingkey.h>
#include "table.h"

#define QRAD_TYPE_SERIAL    " serial"
#define QRAD_TYPE_BIGSERIAL " bigserial"
#define QRAD_TYPE_BIGINT    " bigint"
#define QRAD_TYPE_INTEGER   " integer"
#define QRAD_TYPE_CHARACVAR " character varying"
#define QRAD_TYPE_DATE      " date"
#define QRAD_TYPE_TIME      " time"
#define QRAD_TYPE_NUMERIC   " numeric"
#define QRAD_TYPE_BOOLEAN   " boolean"

SqlParser::SqlParser()
{
  m_skels =NULL;
}

SqlParser::~SqlParser()
{
}

int SqlParser::Build( skeleton *skel, QString filename)
{
    if( !m_skels )
        m_skels = new QList<skeleton *>;

    m_skels->append(skel);
    return Parse(filename);
}

QList<skeleton *> *SqlParser::Build( QString filename )
{
    if( !m_skels )
        m_skels = new QList<skeleton *>;

    return Parse2(filename);
}

void SqlParser::Dump()
{
    QList<CTable*> *tables =  m_skels->at(m_skels->count()-1)->GetTables();
    for( int i = 0; tables && (i < tables->count()); i++ )
    {
        qDebug() << "Dump\n------";
        qDebug() << "Table:" << tables->at(i)->getName();
        QList<CField*> *fields =  tables->at(i)->getFields();
        for( int j = 0; fields  && (j < fields->count()); j++ )
        {
            CField *f = fields->at(j);
            qDebug() << "Field: " << f->getField();
            if( f->HasFKey() )
            {
                qDebug() << "HasForeingKey: ";
                CForeingKey *fo = f->getForeingKey();
                qDebug() << "Ref. Table: " << fo->getTable()->getName();
                qDebug() << "Ref. Field: " << fo->getKey()->getField();
            }
        }
    }
}


QList<skeleton *> *SqlParser::Parse2(QString filename)
{
    // default 1 x 1 (skl x tbl)

    QString   TableName;
    CTable   *table = NULL;
    skeleton *skel = NULL;
    QFile *file = new QFile(filename);

    if(!file->open(QIODevice::ReadOnly))
    {
        qDebug() << "Nao foi possivel abrir " << filename << " para leitura!! ";
        delete file;
        return 0;
    }

    while( !file->atEnd() )
    {
        QString Line = file->readLine();

        if(SkipLine(Line))
        {
            getForeingKey(Line);
            continue;
        }

        TableName = GetTableName(Line);
        if(!TableName.length())
        {
            table = new CTable;
            table->setName(TableName);
            skel = new skeleton;
            skel->SetName(TableName);
            skel->AddTable(table);
            m_skels->append(skel);
        }
        else if( table )
        {
            QString Field = GetFieldName(Line);
            QString Type =  Match(Line);
            if( !Field.length() || !Type.length() )
            {
                qDebug() << "Field: " << Field << " Type: " << Type << " Problem!!!";
            }
            else
            {
                CField *fld = new CField;
                fld->setField(Field,Type);
                table->AddField(fld);
            }
        }
    }

    file->close();
    delete file;

    Dump();

    return m_skels;
}



int SqlParser::Parse(QString filename)
{
    // default 1 x 1 (skl x tbl)

    QString TableName;
    CTable *table = NULL;
    QFile *file = new QFile(filename);

    if(!file->open(QIODevice::ReadOnly))
    {
        qDebug() << "Nao foi possivel abrir " << filename << " para leitura!! ";
        delete file;
        return 0;
    }

    while( !file->atEnd() )
    {
        QString Line = file->readLine();

        if(SkipLine(Line))
        {
            getForeingKey(Line);
            continue;
        }

        TableName = GetTableName(Line);
        if(!TableName.length())
        {
            table = new CTable;
            table->setName(TableName);
            m_skels->at(m_skels->count()-1)->AddTable(table);
        }
        else if( table )
        {
            QString Field = GetFieldName(Line);
            QString Type =  Match(Line);
            if( !Field.length() || !Type.length() )
            {
                qDebug() << "Field: " << Field << " Type: " << Type << " Problem!!!";
            }
            else
            {
                CField *fld = new CField;
                fld->setField(Field,Type);
                table->AddField(fld);
            }
        }
    }

    file->close();
    delete file;

    if( TableName.length() )
        m_skels->at(m_skels->count()-1)->SetName(TableName);

    Dump();

    return TableName.length();
}

QString SqlParser::Match( QString Line)
{

    if( Line.contains(QRAD_TYPE_SERIAL))
        return "int";
    if( Line.contains(QRAD_TYPE_BIGSERIAL))
        return "int";
    if( Line.contains(QRAD_TYPE_BIGINT))
        return "int";
    if( Line.contains(QRAD_TYPE_INTEGER))
        return "int";
    if( Line.contains(QRAD_TYPE_CHARACVAR))
        return "QString";
    if( Line.contains(QRAD_TYPE_DATE))
        return "QDate";

    if( Line.contains(QRAD_TYPE_NUMERIC))
        return "double";
    if( Line.contains(QRAD_TYPE_TIME))
        return "QTime";
    if( Line.contains(QRAD_TYPE_BOOLEAN))
        return "bool";

    return "";
}


QString SqlParser::GetTableName(QString Line)
{
    int nIndex;
    QString TableName;

    nIndex = Line.toUpper().indexOf("CREATE TABLE");
    if(nIndex > -1)
    {
        TableName = Line.mid(nIndex+13);
        TableName = TableName.remove("(").trimmed();
    }

    return TableName;

}

QString SqlParser::GetFieldName( QString Line )
{
  int i;

 for( i = 0; i < Line.length(); i++ )
 {
     if( Line[i] != ' ')
         break;
 }
  QString tmp = Line.mid(i);
  tmp.truncate(tmp.indexOf(' '));
  tmp = tmp.remove('"');
  return tmp;
}

//ALTER TABLE ONLY dsm_sale_item
//ADD CONSTRAINT dsm_sale_item_productid_fkey FOREIGN KEY (productid) REFERENCES dsm_product(id);

void SqlParser::getForeingKey( QString Line )
{
//    qDebug() << Line;
    QString tmp;
    QString Field;
    if(!Line.contains("FOREIGN KEY"))
        return ;

    tmp = Line.mid(Line.indexOf("FOREIGN KEY"));
    if(!tmp.contains("("))
        return;

    tmp = tmp.mid(tmp.indexOf("(")+1);
    tmp.truncate(tmp.indexOf(")"));
    tmp = tmp.trimmed();


//    fkey = new CField ;
//    fkey->setField(tmp);
    Field = tmp;

    tmp = Line.mid(Line.indexOf("REFERENCES ")+11);
    tmp.truncate(tmp.indexOf("("));

    CForeingKey *f = new CForeingKey;
    f->getTable()->setName(tmp);
    tmp= Line.mid(Line.lastIndexOf("(")+1);
    tmp.truncate(tmp.indexOf(")"));
    f->getKey()->setField(tmp);
 //   fkey->setForeingKey(f);

    ///
    /// Find correct node here
    ///
    QList<CTable *> *Tables = m_skels->at(m_skels->count()-1)->GetTables();
    if( !Tables )
    {
        qDebug() <<"Warning: file must present foreing keys after table definition .. ";
        return ;
    }
    bool bFoundRef = false;
    CTable *tblcurrent = Tables->at(Tables->count()-1);
    QList<CField *> *fields = tblcurrent->getFields();
    for( int j = 0; fields && j < fields->count(); j++ )
    {
        if(( Field == fields->at(j)->getField() ) && (tblcurrent->getName() != f->getTable()->getName() ) )
        {
            fields->at(j)->setForeingKey(f);
            bFoundRef = true;
        }
    }
    if( !bFoundRef )
        delete f;
}

bool SqlParser::SkipLine( QString Line )
{
    if(Line.contains("CONSTRAINT"))
        return true;

    if(Line.length() < 3)
        return true;

    if(Line.startsWith("--"))
        return true;

    return false;
}

void SqlParser::PrintCommand()
{
   QString Command = "qrad -s ";

   for( int i =0; m_skels && (i < m_skels->count()); i++ )
   {
      Command += m_skels->at(i)->GetName();
      QList<CTable *> *pTables = m_skels->at(i)->GetTables();
      for( int j = 0 ; pTables && ( pTables->count()); j++)
      {
          Command += " -t " + pTables->at(j)->getName();
          QList<CField *> *pFields = pTables->at(j)->getFields();
          for( int k = 0; pFields && ( k < pFields->count()); k++ )
          {
              CField *fld = pFields->at(k);
              Command += " -c " + fld->getField() + " -i " + fld->getType()->getType();
          }
      }
   }

/*   QList<QPair<CField*, CType*> > fields = m_skel->GetFields();

  Command += "./qrad -t " + m_skel->GetName();
  for( int i = 0; i< fields.count();i++)  /// skip primary key
  {

  }
*/
  qDebug() << "Comando: " << Command;

}
































/*
 *

  id bigserial NOT NULL,
  total numeric(11,2),
  discount numeric(11,2),
  round numeric(11,2),
  status integer DEFAULT 0,
  tick bigint DEFAULT 0,
  date date,
  "time" time without time zone,
  canceldate date,
  canceltime time without time zone,
  clientid integer,
  cashierid integer,
  sourceid integer,
  ccf numeric(11,0),
  coo numeric(11,0),
  gt numeric(11,2),
  doctype character varying(10) DEFAULT 'ECF'::character varying,
  obs character varying(256),
  isdelivery boolean,
  withpresell boolean,
  popdrug boolean,
  sellerid integer NOT NULL DEFAULT 1,
  lasthdserial character varying(32),
  payback numeric(11,2) DEFAULT 0,
  exported boolean DEFAULT false,
  destid integer DEFAULT 0,
  promotionid integer DEFAULT 0,
  sequence integer,
  predate date,
  pretime time without time zone,
  noiteminsale boolean DEFAULT false,


CREATE TABLE pessoa
(
  id serial NOT NULL,
  nome character varying,
  nascimento date,
  endereco character varying,
  telefone character varying,
  bairro character varying,
  CONSTRAINT pessoa_pkey PRIMARY KEY (id)
)


 */
