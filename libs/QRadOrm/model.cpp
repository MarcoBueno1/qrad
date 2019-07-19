#ifdef __linux__
#include <syslog.h>
#else
#include <QDebug>
#endif
#include "model.h"
#include "qradmodellog.h"
#include "pgsqlasync.h"
#include <QPixmap>

#ifdef __linux__
#include <unac.h>
#endif

#include "qraddebug.h"

#define IS_STRING_VALUE(attr) ((ATTR_QString == attr->type()) || (ATTR_QDate == attr->type()) || \
                               (ATTR_QTime == attr->type()) || ( ATTR_QChar == attr->type()) || \
                               (ATTR_char  == attr->type()))

#define ATTR_VALUE(attr) IS_STRING_VALUE(attr) ? ("\'" + attr->toString().replace("\'", "\'\'") + "\'") : attr->toString()

QMap<QString, QString> Model::m_fieldMap;


#include <QApplication>
#include <QWidget>
#include <QDebug>
bool Model::logError()
{
    QString strLog;
    QWidget *pWin = QApplication::activeWindow();

    strLog = pWin?pWin->windowTitle()+" ":"";
    strLog +=  m_tableName + ":";
    strLog += lastError().text();
    QRad_HISEVERITYLOG( strLog );
    return true;
}


Model::Model()
{

}

QList<ModelAttribute*> Model::attributes()
{
    return m_attrList;
}

bool Model::TableExist()
{
    bool bRet = false;
    QString query = QString::fromUtf8("SELECT true FROM pg_tables WHERE tablename = '%1'").arg(m_tableName);

    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery(query);
    if (model->rowCount() > 0 )
    {
        qDebug() << "Tabela existe..." ;
        bRet = true;
    }
    else
   {
       qDebug() << "Tablea nao Existe";
   }
    delete model;
    return bRet;
}

bool Model::FieldExist(QString FieldName)
{
    bool bRet = false;
    QString query = QString::fromUtf8("WHERE table_name='%1' and column_name='%2'")
                                       .arg(m_tableName)
                                       .arg(FieldName);

    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery(query);
    if (model->rowCount() > 0 )
    {
        bRet = true;
    }
    delete model;
    return bRet;
}

void Model::EnsureFieldsExist()
{
    QString strField;
    QString StrCreate = QString::fromUtf8("ALTER TABLE %1 ADD COLUMN ").arg(m_tableName);
    QSqlDatabase Database;

    if(isCustomDatabase())
    {
        Database =  getDatabase();
    }
    else
    {
        Database = QSqlDatabase::database();
    }

    foreach(ModelAttribute *attr, m_attrList)
    {
       if( !FieldExist( attr->fieldName() ) )
       {
            strField = CreateFieldTxt( attr ) + ";";
            QSqlQuery *query = new QSqlQuery(Database);

            if( query->exec( QString(StrCreate + strField) ) < 1 )
            {
                QString Error = QString::fromUtf8("Postgres Erro: %1, Descricao: %2")
                                                 .arg( query->lastError().number())
                                                 .arg( query->lastError().text());

                QRad_HISEVERITYLOG( QString::fromUtf8("ERRO %1:%2 ").arg(Error).arg(Error) );
            }
            else
            {
                QRad_HISEVERITYLOG( QString::fromUtf8("Field %1:%2 criada com sucesso!!").arg(m_tableName).arg(attr->fieldName()) );
            }
            delete query;
       }
    }
}

QString Model::CreateFieldTxt( ModelAttribute *attr )
{
    QString StrCreate;
    QString strFieldType;
    QSqlDatabase Database;


    if(isCustomDatabase())
    {
        Database =  getDatabase();
    }
    else
    {
        Database = QSqlDatabase::database();
    }

    if( attr == m_primaryKey)
    {
		
		QString strPrimType = "serial NOT NULL";
//		debug_message(" drivername: %s\n", Database.driverName().toUtf8().data() );
		if( Database.driverName()	 == "QSQLITE" )
            strPrimType = "INTEGER PRIMARY KEY";
		
        StrCreate += QString::fromUtf8("%1 %2").arg(attr->fieldName()).arg(strPrimType);
    }
    else
    {
        switch( attr->type() )
        {
            case ATTR_Serial:
                if( Database.driverName() == "QSQLITE" )
                    strFieldType = "INTEGER";
                else
                    strFieldType = "serial";
                break;

            case ATTR_int    :
                strFieldType = "INTEGER";
                break;
            case ATTR_double:
                if( Database.driverName() == "QSQLITE" )
                    strFieldType = "REAL";
                else
                    strFieldType = "numeric(11,3)";
                break;
            case ATTR_bool:
                if( Database.driverName() == "QSQLITE" )
                    strFieldType = "INTEGER";
                else
                    strFieldType = "boolean";
                break;
            case ATTR_QDate:
                if( Database.driverName() == "QSQLITE" )
                    strFieldType = "TEXT";
                else
                    strFieldType = "date";
                break;
            case ATTR_QTime:
                if( Database.driverName() == "QSQLITE" )
                    strFieldType = "TEXT";
                else
                    strFieldType = "time without time zone";
                break;
            case ATTR_QString:
                default:
                //if( Database.driverName() == "QSQLITE" )
                //    strFieldType = "TEXT";
                //else
                    strFieldType = "character varying";
                break;
        }
        StrCreate += QString::fromUtf8(" %1 %2").arg(attr->fieldName()).arg(strFieldType);
    }
    return StrCreate;
}

void Model::CreateTable( bool bUpdate)
{
    bool bFirst =  true;
    ModelAttribute *pKey = NULL;
    QSqlDatabase Database;

    QString StrCreate = QString::fromUtf8("CREATE TABLE %1 (\n ").arg(m_tableName);

    if(isCustomDatabase())
    {
        Database =  getDatabase();
    }
    else
    {
        Database = QSqlDatabase::database();
    }

    foreach(ModelAttribute *attr, m_attrList)
    {
        if( !bFirst )
            StrCreate +=  ",\n";

        if( attr == m_primaryKey )
        {
            pKey =  attr;
        }

        StrCreate += CreateFieldTxt( attr );
        bFirst = false;
    }

    if(pKey)
	{
		if( Database.driverName() != "QSQLITE" ) 
		   StrCreate += QString::fromUtf8(", CONSTRAINT %1_pkey PRIMARY KEY (%2) ")
										  .arg(m_tableName)
										  .arg(pKey->fieldName());
	}

    if( Database.driverName() != "QSQLITE" )
    {
        StrCreate += QString::fromUtf8(") WITH ( OIDS=FALSE );");
        StrCreate += QString::fromUtf8( " ALTER TABLE %1 OWNER TO ").arg(m_tableName);
        StrCreate += Database.userName() +" ;";
    }
    else
        StrCreate += QString::fromUtf8(");");


    QSqlQuery *newTable =  new QSqlQuery(Database);
    if( newTable->exec( StrCreate ) < 1 )
    {

        QString Error = QString::fromUtf8(" Postgres Erro: %1, Descricao: %2")
                                         .arg(newTable->lastError().number())
                                         .arg( newTable->lastError().text());
        QRad_HISEVERITYLOG( Error );
        DumpSQLError(StrCreate + Error,bUpdate );
    }
    else
    {
        QRad_HISEVERITYLOG( QString::fromUtf8("Tabela %1 criada com sucesso!!").arg(m_tableName) );
    }
    QString strCreateSequence = QString(" CREATE SEQUENCE %1_id_seq "\
                                        " START WITH 1 "\
                                        " INCREMENT BY 1 "\
                                        " NO MINVALUE "\
                                        " NO MAXVALUE "\
                                        "CACHE 1;").arg(m_tableName);

    newTable->exec( strCreateSequence );

    qDebug() << "Comando:" << StrCreate;
    delete newTable;
}

void Model::copyFrom(Model* model)
{
    for (int i = 0; i < m_attrList.length(); i++)
    {
        ModelAttribute* attrTo = m_attrList.at(i);
        ModelAttribute* attrFrom = model->attributes().at(i);

        if ((attrFrom->fieldName() != attrTo->fieldName()) || (attrFrom->type() != attrTo->type()))
        {
            continue;
        }

        attrTo->setValue(attrFrom->value());
    }
}

void Model::fillModel(QSqlRecord record)
{
    foreach(ModelAttribute *attr, m_attrList)
    {
        QVariant value = record.value(attr->fieldName().remove("\""));
        attr->setValue(value);
    }
}

bool Model::refresh()
{
    QString query = (IS_STRING_VALUE(m_primaryKey) ?
                    QString("select * from %1 where %2 = \'%3\'") :
                    QString("select * from %1 where %2 = %3"))
                    .arg(m_tableName)
                    .arg(m_primaryKey->fieldName())
                    .arg(m_primaryKey->toString());

    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery(query);

    if (model->rowCount() == 0)
    {
        delete model;
        return false;
    }

    fillModel(model->record(0));

    delete model;
    return true;
}

bool Model::do_insert()
{
    QString query;
    QString Error;
    QSqlDatabase Database;
    bool ret;
    for( int i = 0; i < 2; i++)
    {
        QString fields = "";
        QString values = "";
        int count = 0;

        query = "";

        foreach(ModelAttribute *attr, m_attrList)
        {
            if ((attr == m_primaryKey) && (attr->value().toInt() == 0))
                continue;

            if (attr->value().isNull()) {
                continue;
            }

            if (count > 0)
            {
                fields += ",";
                values += ",";
            }

            fields += attr->fieldName();
            values += "?";

            count++;
        }

        if(isCustomDatabase())
        {
            Database =  getDatabase();
        }
        else
        {
            Database = QSqlDatabase::database();
        }
        if(Database.driverName() != "QSQLITE" )
        {
            query = QString("insert into %1 (%2)values(%3) RETURNING %4 ")
                    .arg(m_tableName)
                    .arg(fields)
                    .arg(values)
                    .arg(m_primaryKey->fieldName());
        }
        else
        {
            query = QString("insert into %1 (%2)values(%3) ")
                .arg(m_tableName)
                .arg(fields)
                .arg(values);
        }
        QSqlQuery *insert;

        if(isCustomDatabase())
        {
            insert = new QSqlQuery(getDatabase());
        }
        else
        {
            insert = new QSqlQuery();
        }

        insert->prepare(query);

        foreach(ModelAttribute *attr, m_attrList)
        {
            if ((attr == m_primaryKey) && (attr->value().toInt() == 0))
                continue;

            if (attr->value().isNull()) {
                continue;
            }

            insert->addBindValue(attr->value());
        }

        ret = insert->exec();

        if (ret)
        {
            if(Database.driverName() != "QSQLITE" )
            {
                insert->next();
                m_primaryKey->setValue( insert->record().value( m_primaryKey->fieldName() ).toInt());
            }
            else
                m_primaryKey->setValue(lastInsertId());

            delete insert;
            break;
        }
        else
        {
           // QRad_HISEVERITYLOG( insert-> );
            QRad_HISEVERITYLOG( insert->lastError().text() );
            Error = QString::fromUtf8("Postgres Erro: %1, Descricao: %2")
                                            .arg(insert->lastError().number())
                                            .arg(insert->lastError().text());

            // debug try create
            if( !TableExist() )
            {
                CreateTable();
            }
			else
				EnsureFieldsExist();
        }
        delete insert;
    }
    if( !ret )
    {
        DumpSQLError(query + Error);
    }
    return ret;
}
void Model::DumpSQLError( QString query, bool bUpdate  )
{
//#ifdef __linux__
    QString strQyeryDebug;
    int count = 0;

    strQyeryDebug = query;
    if( !bUpdate )
    {
      strQyeryDebug.truncate(query.indexOf("values(")+7);
    }
    else
    {
        strQyeryDebug += " Valores:  " ;
    }
    count = 0;

    foreach(ModelAttribute *attr, m_attrList)
    {
        if ((attr == m_primaryKey) && (attr->value().toInt() == 0))
            continue;

        if (attr->value().isNull()) {
            continue;
        }

        if( count )
            strQyeryDebug +=",";
        strQyeryDebug += DebugAttribute(attr);
        count++;
    }
    strQyeryDebug +=");";


    QString strTrace;
    QWidget *pWin = QApplication::activeWindow();

    strTrace = pWin?pWin->windowTitle()+" ":"";
    strTrace +=  m_tableName + ": ";

    strTrace += QString("SQL ERROR!! - Conexao:") + (isCustomDatabase()?getDatabase().hostName(): QString("localhost")) + QString(" ") + strQyeryDebug;
    QRad_HISEVERITYLOG( strTrace );
//    syslog( LOG_ERR, "%s",strTrace.toUtf8().data());
//    syslog( LOG_ERR, "%s",m_lastError.text().toUtf8().data());
//#endif

}

QString Model::DebugAttribute(ModelAttribute *attr)
{
    QString strTemp;
    switch( attr->type() )
    {
        case ATTR_QString:
        case ATTR_QDate:
        case ATTR_QTime:
        case ATTR_QChar:
        case ATTR_char:
                    strTemp.sprintf( "'%s'", attr->value().toString().toUtf8().data());
                    break;
        case ATTR_int:
        case ATTR_Serial:
                    strTemp.sprintf( "%d", attr->value().toInt());
                    break;
        case ATTR_double:
                    strTemp.sprintf( "%0.02f", attr->value().toDouble());
                    break;
        case ATTR_bool:
                    strTemp = attr->value().toString();
                    break;
    }

    return strTemp;
}

bool Model::do_update()
{
    bool ret;
    QString query;
    QString Error;

    for( int i = 0; i < 2; i++)
    {
        QString fields = "";
        QString values = "";
        int count = 0;

        if (NULL == m_primaryKey)
            return false;

        foreach(ModelAttribute *attr, m_attrList)
        {
            if (attr == m_primaryKey)
                continue;

            if (attr->value().isNull()) {
                continue;
            }

            if (count > 0)
            {
                fields += ",";
                values += ",";
            }

            fields += attr->fieldName();
            fields += " = ?";
            //values += "?";

            count++;
        }

                 query = QString("update %1 set %2  where %4 = %5")
                       .arg(m_tableName)
                       .arg(fields)
                      // .arg(values)
                       .arg(m_primaryKey->fieldName())
                       .arg(ATTR_VALUE(m_primaryKey));

/*
 *         query = QString("update %1 set (%2) = (%3) where %4 = %5")
                .arg(m_tableName)
                .arg(fields)
                .arg(values)
                .arg(m_primaryKey->fieldName())
                .arg(ATTR_VALUE(m_primaryKey));
*/

        QSqlQuery *update = NULL;

        if (isCustomDatabase())
            update = new QSqlQuery(getDatabase());
        else
            update = new QSqlQuery();

        update->prepare(query);

        foreach(ModelAttribute *attr, m_attrList)
        {
            if (attr == m_primaryKey)
                continue;

            if (attr->value().isNull()) {
                continue;
            }

            qDebug() << "Value: " <<attr->value();
            update->addBindValue(attr->value());
        }

        ret = update->exec();

        if (ret)
        {
            delete update;
            break;
        }
        else
        {
            qDebug() << query;
            qDebug() << update->lastQuery();

            Error = QString::fromUtf8(" Postgres Erro: %1, Descricao: %2")
                                            .arg(update->lastError().number())
                                            .arg(update->lastError().text());

            qDebug() << Error;

            // debug try create
            if( !TableExist() )
            {
                CreateTable();
            }
			else
				EnsureFieldsExist();
        }
        delete update;
    }
    if( !ret )
    {
        DumpSQLError(query + Error);
    }

    return ret;
}

bool Model::do_delete()
{
    if (NULL == m_primaryKey)
        return false;

    QString delQuery = QString ("delete from %1 where %2 = %3")
                       .arg(m_tableName)
                       .arg(m_primaryKey->fieldName())
                       .arg(ATTR_VALUE(m_primaryKey));

    QSqlQuery *query = NULL;

    if (isCustomDatabase())
        query = new QSqlQuery(getDatabase());
    else
        query = new QSqlQuery();

    bool ret = query->exec(delQuery);
    
    m_lastError =query->lastError();

    delete query;

    return ret;
}

int Model::lastInsertId()
{
    if (ATTR_int != m_primaryKey->type() || (isCustomDatabase()))
        return 0;

    QString query = QString("select %2 from %1 order by %2 desc limit 1")
                    .arg(m_tableName)
                    .arg(m_primaryKey->fieldName());

    QSqlQueryModel *model = new QSqlQueryModel();

    if (isCustomDatabase())
        model->setQuery(QSqlQuery(query, getDatabase()));
    else
        model->setQuery(query);

    if (model->rowCount() <= 0)
    {
        delete model;
        return 0;
    }

    int id = model->record(0).value(m_primaryKey->fieldName()).toInt();

    delete model;
    return id;
}

void  Model::Audit()
{
   QCoreApplication *app = QCoreApplication::instance();
   int CurrentUser = app->property("CurrentUserId").toInt();
   QString CurrentTable = m_tableName;
   m_tableName += QString("_%1").arg(CurrentUser);

   Create();

   m_tableName = CurrentTable;
}

bool Model::Save()
{
    bool bRet;

    if (m_primaryKey->value().toInt() == 0)
    {
        bRet = Create();
    }
    else 
    {
        bRet = Update();
    }
    if( bRet )
        Audit();

    return bRet;
}


bool Model::Create()
{
    bool bRet = do_insert();

    if( !bRet )
    {
        logError();
    }

    return bRet;
}

bool Model::Update()
{
    bool bRet = do_update();
    if( !bRet )
    {
       logError();
    }
    return bRet;
}

bool Model::Delete()
{
    logError();
    return do_delete();
}

void Model::InitAttribute(ModelAttribute *attr)
{
    switch (attr->type())
    {
        case ATTR_QString: attr->setValue("");                     break;
        case ATTR_Serial:
        case ATTR_int:     attr->setValue(0);                      break;
        case ATTR_double:  attr->setValue(0.0);                    break;
        case ATTR_bool:    attr->setValue(false);                  break;
        case ATTR_QDate:   attr->setValue(QDate(2000, 1, 1));      break;
        case ATTR_QTime:   attr->setValue(QTime(0, 0, 0, 0));      break;
        case ATTR_QChar:   attr->setValue('*');                    break;
        case ATTR_char:    attr->setValue('*');                    break;
        default:
           #ifdef __linux
             syslog( LOG_ERR,  "Attribute Failed: %s\n" ,attr->fieldName().toUtf8().data());
           #else
            qDebug() << "Attribute Failed: " << attr->fieldName().toUtf8().data();
           #endif

             return;
    }
}


QSqlError Model::lastError()
{
    return m_lastError;
}


QString Model::getNoAccentuation( QString strPortuguese )
{

    QString strText = strPortuguese;
#if 0

    char *szTeste = (char *)calloc(strText.length() + 10,1 );

    strcpy( szTeste, strText.toLatin1().data() );

    char* out = 0;
    size_t out_length = 0;
    if(!unac_string("ISO-8859-1", szTeste, strlen(szTeste), &out, &out_length))
    {
        out[out_length] = 0;
        strText = QString::fromLatin1(out);
    }

    free(szTeste);
    free(out);

#endif

    return strText;

}

int Model::saveImage( QString path )
{
  QSqlDatabase db = QSqlDatabase::database();

  int nLoId = PGSQLAsync::Send( path,
              db.hostName(),
              db.databaseName(),
              db.userName(),
              db.password() );

  return nLoId;
}

QPixmap Model::getImage(int nLoId )
{
   QRAD_IN();

   debug_message("nLoId:%d\n", nLoId);


   QImage myImage;// = new QImage(strFoto);
   QPixmap pix;

   QSqlDatabase db = QSqlDatabase::database();

   QTime time = QTime::currentTime();
   QString strFoto = QString("image_%1_%2_%3.jpg").arg(time.hour()).arg(time.minute()).arg(time.second());
 
   if(QFile::exists(strFoto))
   {
       debug_message("Removendo arquivo %s Md5:%s\n", strFoto.toLatin1().data(), fileChecksum(strFoto).data());

       QFile *remove= new QFile(strFoto);

       bool bRes = remove->remove();
       if( !bRes )
       {
           debug_message("Erro ao remover arquivo %s Md5:%s\n", strFoto.toLatin1().data(), fileChecksum(strFoto).data());
       }
       delete remove;
   }


   if( 1 != PGSQLAsync::Receive( (unsigned int)nLoId,
                          strFoto,
                          db.hostName(),
                          db.databaseName(),
                          db.userName(),
                          db.password() ))
   {
       debug_message("Nao foi possivel ler a imagem do BD: %s\n", strFoto.toLatin1().data());
       pix = QPixmap(100, 200);

       QRAD_OUT()

       return pix;

   }

    if( !myImage.load(strFoto))
    {
        debug_message("Erro ao tentar carregar imagem: %s\n", strFoto.toLatin1().data());
    }

    if( myImage.isNull())
    {
        debug_message("Objeto myImage nulo: %s\n", strFoto.toLatin1().data());
    }

    pix =  QPixmap::fromImage(myImage);

    QRAD_OUT()

    return pix;
}


QString Model::getImage(int nLoId, QString path )
{
//  Q_UNUSED(path);

  QSqlDatabase db = QSqlDatabase::database();

     QTime time = QTime::currentTime();
     QString strFoto = QString("image_%1_%2_%3.jpg").arg(time.hour()).arg(time.minute()).arg(time.second());
     if( path.trimmed().length() )
         strFoto = path;


     PGSQLAsync::Receive( (unsigned int)nLoId,
                          strFoto,
                          db.hostName(),
                          db.databaseName(),
                          db.userName(),
                          db.password() );


    ///
    /// \brief codigo para ler a imagem do banco de dados
    ///

    return strFoto;
}

//   QCoreApplication *app = QCoreApplication::instance();
//    m_useTableFilter = app->property("useTableFilter").toBool();

QByteArray Model::fileChecksum(const QString &fileName, QCryptographicHash::Algorithm hashAlgorithm)
{
    QFile f(fileName);
    if (f.open(QFile::ReadOnly)) {
        QCryptographicHash hash(hashAlgorithm);
        if (hash.addData(&f)) {
            return hash.result();
        }
    }
    return QByteArray().toHex();
}
