#include "scombobox.h"
#include <QMessageBox>
#include <QDir>
#include <QEvent>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QSqlQuery>
#include "qraddebug.h"
#include <QDebug>

SComboBox::SComboBox(QWidget *parent) :
    QComboBox(parent)
{
    debug_message("-->SComboBox\n");
    m_CurrentId = -1;
    m_pEditor = NULL;
    m_pModelLocal = new QSqlQueryModel;
    connect(this, SIGNAL(OnEdit()), this, SLOT(doEdit()));
    connect(this, SIGNAL(OnUpdate(QString)), this, SLOT(doUpdate(QString)));
    debug_message("<--SComboBox\n");

}
SComboBox::~SComboBox()
{
    if(m_pEditor)
        delete m_pEditor;

    delete m_pModelLocal;
}

void SComboBox::showEvent ( QShowEvent * event )
{
    Q_UNUSED(event );
    debug_message("-->showEvent\n");

    if( Table().isEmpty() )
    {
        QMessageBox::warning(this,
                             "Oops!",
                             QCoreApplication::translate("SComboBox",
                                                         QString("Use setTable() to %1")
                                                         .arg(objectName())
                                                         .toLatin1().data()));
        return;
    }
    if( UserName().isEmpty() )
    {
        QMessageBox::warning(this,
                             "Oops!",
                             QCoreApplication::translate("SComboBox",
                                                         QString("Use setUserName() to %1")
                                                         .arg(objectName())
                                                         .toLatin1().data()));
        return;
    }
    if( Field().isEmpty() )
    {
        QMessageBox::warning(this,
                             "Oops!",
                             QCoreApplication::translate("SComboBox",
                                                         QString("Use setField() to %1")
                                                         .arg(objectName())
                                                         .toLatin1().data()));
        return;
    }

    PersistObjects();
    connect(this,SIGNAL(activated(QString)),this,SLOT(NovoActivacted(QString)));

    debug_message("<--showEvent\n");
}


/**
 @brief CreateSourceFile
 Esta funcao escreve no ecran a string "Hello, world" e sai.
 @param argc numero de parametros que ocorrem no argv
 @param argv[] vector com os parametros escritos
 na linha de comando pelo utilizador
 @return valor indicador de erro ou sucesso (
 */

bool SComboBox::ConnectDB()
{
    if(!Host().isEmpty())
    {
        debug_message("tentando criar uma conexao especifica ... \n");

        QSqlDatabase database = QSqlDatabase::addDatabase(QSqlDatabase::database().driverName(), objectName());
        database.setHostName(Host() );
        database.setDatabaseName(DatabaseName());
        database.setUserName(UserName());
        database.setPassword(Password());

        if( !database.open() || !database.isValid() )
        {
            QMessageBox::warning(NULL,
                                 QCoreApplication::translate("SComboBox","Error: Database connection"),
                                 database.lastError().text());
            return false;
        }

        m_currentdb = database;
    }
    else
    {
        m_currentdb = QSqlDatabase::database();
        debug_message("usando conexao default ..\n");
    }

    return true;
}

QString SComboBox::CreateEditUi()
{
    QString New;
    QDialog *Window =  new QDialog(qobject_cast<QWidget *>(parent()));
    QFormLayout *Layout = new QFormLayout(Window);
    QPushButton *Button = new QPushButton(Window);
    QLineEdit *LineEdit = new QLineEdit(Window);
    QLabel *Label = new QLabel(QString::fromUtf8("%1:").arg(FieldCaption()),Window);

    Window->setWindowTitle(QCoreApplication::translate("SComboBox",QString("Add %1").arg(TableCaption()).toLatin1().data()));
    Button->setText(QCoreApplication::translate("SComboBox","Save"));
    Layout->addRow( Label );
    Layout->addRow( LineEdit );
    Layout->addRow( Button );

    connect(Button,SIGNAL(clicked(bool)),Window,SLOT(accept()));

    Window->setLayout(Layout);
    LineEdit->setFocus();
    Button->setDefault(true);
    if( Window->exec() == QDialog::Accepted )
    {
        if( !LineEdit->text().trimmed().isEmpty())
        {
            QSqlQueryModel *AlreadyExist = new QSqlQueryModel;
            AlreadyExist->setQuery(QString("select %1 from %2 where %1 = '%3' and removed <> %4 order by tp, %1")
                                   .arg(FieldName().toLower())
                                   .arg(TableName().toLower())
                                   .arg(LineEdit->text().trimmed())
                                   .arg(TrueToken()),
                                   m_currentdb);

            if( !AlreadyExist->rowCount() )
            {
                AlreadyExist->setQuery(QString("insert into %1 (%2) values ('%3')")
                                         .arg(TableName().toLower())
                                         .arg(FieldName().toLower())
                                         .arg(LineEdit->text().trimmed()),
                                       m_currentdb);
                New = LineEdit->text().trimmed();

            }

            delete AlreadyExist;
        }
    }
    delete LineEdit;
    delete Button;
    delete Layout;
    delete Label;
    delete Window;
    return New;
}

void SComboBox::doEdit(void)
{
    QString New;
    if( m_pEditor )
    {
        m_pEditor->exec();
        QSqlQueryModel *LastInserted = new QSqlQueryModel;
        LastInserted->setQuery(QString("select %2 from %1 where removed <> %3 order by id desc limit 1")
                               .arg(TableName().toLower())
                               .arg(FieldName().toLower())
                               .arg(TrueToken()),
                                m_currentdb);
        New = LastInserted->index(0,0).data().toString();
        delete LastInserted;
    }
    else
    {
         New = CreateEditUi();
    }

    if( !New.isEmpty())
    {
        doUpdate(New);
    }
    else
    {
        setCurrentIndex(0);
    }
}
void SComboBox::doUpdate(QString New)
{
            m_pModelLocal->setQuery(QString("select id, %2 from %1 where removed <> %3 order by tp, %2")
                                    .arg(TableName().toLower())
                                    .arg(FieldName().toLower())
                                    .arg(TrueToken()),
                                    m_currentdb);

            setModel(m_pModelLocal);
            setModelColumn(1);
            int Index = findText(New);
            setCurrentIndex(Index);

}
void SComboBox::NovoActivacted(QString Text )
{
    Q_UNUSED(Text);


   if( CanAdd() && currentText() == "::ADICIONAR::" )
   {
         emit OnEdit();
   }
}

bool SComboBox::PersistObjects()
{
    debug_message("\n-->PersistObjects()\n");

    if(!ConnectDB())
    {
        return false;
    }

    bool    bHasNew = false;
    QString strTokenNew  = "::ADICIONAR::";

    CreateTableIfNoExist();

    QSqlQueryModel *pList = new QSqlQueryModel;

    pList->setQuery(QString("select %1 from %2 where removed <> %3")
                    .arg(FieldName().toLower())
                    .arg(TableName().toLower())
                    .arg(TrueToken()),
                    m_currentdb);

    if(pList->rowCount())
    {
        for( int j = 0; j < pList->rowCount(); j++)
        {
            if( pList->index(j,0).data().toString() == strTokenNew )
            {
                bHasNew = true;
                break;
            }
        }
    }


    /// Add each in memory item to database ( if not found in DB )
    for( int i =0; i < count(); i++  )
    {
        bool bFound = false;
        QString Text = itemText(i);

        if( Text == strTokenNew )
            bHasNew = true;

        if(pList->rowCount())
        {
            for( int j = 0; j < pList->rowCount(); j++)
            {
                if( pList->index(j,0).data().toString() == Text )
                {
                    bFound = true;
                    break;
                }

            }
        }

        if(!bFound) // nao encontrado, adiciona ao banco
        {
            QSqlQueryModel *Add = new QSqlQueryModel;
            Add->setQuery(QString("insert into %1 (%2) values ('%3')")
                                     .arg(TableName().toLower())
                                     .arg(FieldName().toLower())
                                     .arg(Text),m_currentdb);

            if( Add->lastError().isValid())
                    QMessageBox::warning(this,"Erro ao Inserir",
                                             QString("Query:%1 Resultado:%2")
                                             .arg(Add->query().lastQuery())
                                             .arg(Add->lastError().text()));
            delete Add;
        }
    }

    delete pList;

    if( !bHasNew && CanAdd())
    {
        QSqlQueryModel *Add = new QSqlQueryModel;
        Add->setQuery(QString("insert into %1 (%2, tp) values ('%3', 2)")
                                 .arg(TableName().toLower())
                                 .arg(FieldName().toLower())
                                 .arg(strTokenNew),m_currentdb);
        delete Add;
    }


    if( CanAdd())
    {
        m_pModelLocal->setQuery(QString("select id, %1 from %2 where %1 <> '' and removed <> %3 order by tp, %1")
                                .arg(FieldName().toLower())
                                .arg(TableName().toLower())
                                .arg(TrueToken())
                               ,m_currentdb);
    }
    else
    {
        m_pModelLocal->setQuery(QString("select id, %1 from %2 where %1 <> '%3' and %1 <> ''  and removed <> %4 order by tp, %1")
                                .arg(FieldName().toLower())
                                .arg(TableName().toLower())
                                .arg(strTokenNew)
                                .arg(TrueToken())
                               ,m_currentdb);
    }

    debug_message("\nm_pModelLocal = %s\n",m_pModelLocal->query().lastQuery().toLatin1().data());



    setModel(m_pModelLocal);
    setModelColumn(1);
    if( m_CurrentId> -1 )
    {
        for(int i = 0; i < model()->rowCount(); ++i)
        {
           if( m_CurrentId == model()->index(i,0).data().toInt())
           {
               setCurrentIndex(i);
               break;
           }
        }
    }

    return true;
}

///
/// \brief SComboBox::CreateTableIfNoExist
///
void SComboBox::CreateTableIfNoExist()
{
    /// first check if can do that ...
    ///

    debug_message("-->CreateTableIfNoExist()\n");
    QString strPrimType = "integer NOT NULL";


    if( UserName().trimmed().isEmpty() )
    {
        debug_message("<--CreateTableIfNoExist() Username is empty L:%d\n", __LINE__);
        return;
    }

    ///
    /// create table if no exists
    ///

    QSqlQueryModel *CrateTable = new QSqlQueryModel();

    QSqlDatabase db = QSqlDatabase::database();
    debug_message(" drivername: %s\n", db.driverName().toUtf8().data() );
    if( db.driverName()	 == "QSQLITE" )
        strPrimType = "INTEGER PRIMARY KEY autoincrement";
    else /// QPSQL
    {
        QString strChecExist = QString("SELECT true FROM pg_tables WHERE tablename = '%1'").arg(TableName().toLower().toLower());
        CrateTable->setQuery(strChecExist, m_currentdb);
        if( CrateTable->rowCount())
        {
            delete CrateTable;
            debug_message("<--CreateTableIfNoExist() L:%d\n", __LINE__);
            return;
        }
    }
	
    QString strCreate = QString( "CREATE TABLE %1 ("\
                                 "id %3, "\
                                 "%2 character varying ,"\
                                 "tp integer default 1 ,"\
                                 "removed boolean default false );")
                                .arg(TableName().toLower()).arg(FieldName().toLower()).arg(strPrimType);

    CrateTable->setQuery( strCreate, m_currentdb );

    QString strOwner = QString("ALTER TABLE %1 OWNER TO %2;").arg(TableName().toLower()).arg(UserName());

    CrateTable->setQuery( strOwner, m_currentdb );


    QString strCreateSequence = QString(" CREATE SEQUENCE %1_id_seq "\
                                        " START WITH 1 "\
                                        " INCREMENT BY 1 "\
                                        " NO MINVALUE "\
                                        " NO MAXVALUE "\
                                        "CACHE 1;").arg(TableName().toLower());

    CrateTable->setQuery( strCreateSequence, m_currentdb );

    QString strSeqOwner = QString( "ALTER TABLE %1_id_seq OWNER TO %2; ").arg(TableName().toLower()).arg(UserName());

    CrateTable->setQuery( strSeqOwner, m_currentdb );

    QString strOwned = QString("ALTER SEQUENCE %1_id_seq OWNED BY %1.id;").arg(TableName().toLower());

    CrateTable->setQuery( strOwned, m_currentdb );

    QString strAlterCol = QString("ALTER TABLE ONLY %1 ALTER COLUMN id SET DEFAULT nextval('%1_id_seq'::regclass);")
            .arg(TableName().toLower());

    CrateTable->setQuery( strAlterCol, m_currentdb );

    QString strCata = QString("SELECT pg_catalog.setval('%1_id_seq', 1, false);").arg(TableName().toLower());

    CrateTable->setQuery( strCata, m_currentdb );

    QString strpKey = QString("ALTER TABLE ONLY %1 ADD CONSTRAINT prmary_key_%1 PRIMARY KEY (id); ").arg(TableName().toLower());

    CrateTable->setQuery( strpKey, m_currentdb );

    delete CrateTable;
    debug_message("<--CreateTableIfNoExist() L:%d\n", __LINE__);

}

QString SComboBox::TableName()
{
   QString strAux = Table();
   if( strAux.contains("."))
       strAux.truncate(strAux.indexOf("."));
   return strAux;
}
QString SComboBox::TableCaption()
{
    QString strAux = Table();
    if( strAux.contains("."))
        strAux = strAux.mid(strAux.indexOf(".")+1);
    return strAux;

}
QString SComboBox::FieldName()
{
    QString strAux = Field();
    if( strAux.contains("."))
        strAux.truncate(strAux.indexOf("."));
    return strAux;

}
QString SComboBox::FieldCaption()
{
    QString strAux = Table();
    if( strAux.contains("."))
        strAux = strAux.mid(strAux.indexOf(".")+1);
    return strAux;

}
void SComboBox::SetEditor(QDialog *pEditor)
{

}

QString SComboBox::TrueToken()
{
    return m_currentdb.driverName()=="QSQLITE"?"'true'":"true";
}
