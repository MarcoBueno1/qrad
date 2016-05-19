#include "scombobox.h"
#include <QMessageBox>
#ifdef _USE_IN_QT_CREATOR
#include <QDesignerFormWindowInterface>
#include <QDesignerPropertyEditorInterface>
#include <QDesignerFormEditorInterface>
#endif
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

//#include "qbuildcombocontroller.h"

SComboBox::SComboBox(QWidget *parent) :
    QComboBox(parent)
{
    debug_message("-->SComboBox\n");
    m_CurrentId = -1;
    m_pModelLocal = new QSqlQueryModel;
    connect(this, SIGNAL(OnEdit()), this, SLOT(doEdit()));
    connect(this, SIGNAL(OnUpdate(QString)), this, SLOT(doUpdate(QString)));
    debug_message("<--SComboBox\n");

}
SComboBox::~SComboBox()
{
    delete m_pModelLocal;
}

void SComboBox::showEvent ( QShowEvent * event )
{
    Q_UNUSED(event );
    debug_message("-->showEvent\n");

#ifdef _USE_IN_QT_CREATOR
    QDesignerFormWindowInterface *formWindow;
    formWindow = QDesignerFormWindowInterface::findFormWindow(this);
    if(formWindow)
    {
        connect( formWindow,SIGNAL(aboutToUnmanageWidget(QWidget*)),this, SLOT(aboutToUnmanage(QWidget*)));
        connect( formWindow,SIGNAL(changed()),this, SLOT(FormChanged()));


//        QMessageBox::information(this, "Caminho do arquivo corrente",
//                                 formWindow->absoluteDir().absolutePath() + "\\" + formWindow->fileName() );
//                                   formWindow->fileName() );
        m_UIFileName = formWindow->fileName();

        QDesignerFormEditorInterface *editor = formWindow->core();
        QDesignerPropertyEditorInterface *ProprietyEditor = editor->propertyEditor();
        connect(ProprietyEditor, SIGNAL(propertyChanged(QString,QVariant)),
                              this, SLOT(checkProperty(QString,QVariant)));


/*        QBuildComboController *newFiles =  new QBuildComboController(this,
                                                                    m_UIFileName,
                                                                    objectName(),
                                                                    Table().toLower(),
                                                                    Field().toLower());

        delete newFiles;*/
    }
    else
#endif
    {

        if( Table().isEmpty() )
        {
            QMessageBox::warning(this, "Oops!", QString("Use setTable() para o componente %1").arg(objectName()));
            return;
        }
        if( UserName().isEmpty() )
        {
            QMessageBox::warning(this, "Oops!", QString("Use setUserName() para o componente %1").arg(objectName()));
            return;
        }
        if( Field().isEmpty() )
        {
            QMessageBox::warning(this, "Oops!", QString("Use setField() para o componente %1").arg(objectName()));
            return;
        }

        PersistObjects();
        connect(this,SIGNAL(activated(QString)),this,SLOT(NovoActivacted(QString)));
    }

///
///  Verificar depois porque o trecho abaixo nao funcionou
///

//            connect(m_FormEditor, SIGNAL(propertyChanged(QString,QVariant)),
  //                        this, SLOT(checkProperty(QString,QVariant)));

    connect( this, SIGNAL(BuildChanged(bool)), this, SLOT(OnBuildChanged(bool)));
    debug_message("<--showEvent\n");
}

void SComboBox::OnBuildChanged(bool bValue)
{
    Q_UNUSED(bValue);
    QMessageBox::information(this, "OnBuildChanged!!",
                               "OnBuildChanged" );
}
void SComboBox::aboutToUnmanage(QWidget* w)
{
    if( w == this )
    {
        QMessageBox::information(this, "aboutToUnmanage!!",
    //                                 formWindow->absoluteDir().absolutePath() + "\\" + formWindow->fileName() );
                                   "aboutToUnmanage" );

    }
}

void SComboBox::focusOutEvent ( QFocusEvent * event )
{
    Q_UNUSED(event);
//    QDesignerFormWindowInterface *formWindow;
  //  formWindow = QDesignerFormWindowInterface::findFormWindow(this);

//    if (event->type() == QEvent::FocusOut)
//    {
//        QMessageBox::information(this, "Perdendo o foco",
//                                 formWindow->absoluteDir().absolutePath() + "\\" + formWindow->fileName() );
  //                                 QString("Build=%1").arg(Build()?"true":"false") );


//    }
//    return false;
}

void SComboBox::checkProperty(QString prop,QVariant val)
{
    Q_UNUSED(val);
    Q_UNUSED(prop);
//    if( prop == "Build" && val.toBool() ==  true )
    {
        QMessageBox::information(this, "OH yeah!!!",
                                   "Propriedade Mudou... "+ QString("Build=%1").arg(Build()?"true":"false") );
    }
}


/**
 @brief CreateSourceFile
 Esta funcao escreve no ecran a string "Hello, world" e sai.
 @param argc numero de parametros que ocorrem no argv
 @param argv[] vector com os parametros escritos
 na linha de comando pelo utilizador
 @return valor indicador de erro ou sucesso (
 */

void SComboBox::FormChanged()
{
//    QMessageBox::information(this, "OH yeah!!!",
//                               "Alguma coisa no form mudou..."+ QString("Build=%1").arg(Build()?"true":"false") );

/*    if( Build() == true)
    {
        QBuildComboController *newFiles =  new QBuildComboController(this,
                                                                    m_UIFileName,
                                                                    objectName(),
                                                                    Table().toLower().toLower(),
                                                                    Field().toLower());

        delete newFiles;
        setBuild(false);
    }
*/
}


void SComboBox::ConnectDB()
{
    if(!Host().isEmpty())
    {
        debug_message("tentando criar uma conexao especifica ... \n");
        QSqlDatabase database = QSqlDatabase::addDatabase("QPSQL", objectName());
        database.setHostName(Host() );
        database.setDatabaseName(DatabaseName());
        database.setUserName(UserName());
        database.setPassword(Password());

        if (!database.open())
        {
            QMessageBox::warning(NULL,
                                 QString::fromUtf8("Erro na conexão com Banco de dados"),
                                 database.lastError().text());
        }
        m_currentdb = database;
    }
    else
    {
        m_currentdb = QSqlDatabase::database();
        debug_message("usando conexao default ..\n");
    }
}

QString SComboBox::CreateEditUi()
{
    QString New;
    QDialog *Window =  new QDialog(qobject_cast<QWidget *>(parent()));
    QFormLayout *Layout = new QFormLayout(Window);
    QPushButton *Button = new QPushButton(Window);
    QLineEdit *LineEdit = new QLineEdit(Window);
    QLabel *Label = new QLabel(QString::fromUtf8("%1:").arg(Field()),Window);

    Window->setWindowTitle(QString::fromUtf8("Adicionar %1").arg(Table()));
    Button->setText("Salvar");
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
            AlreadyExist->setQuery(QString("select %1 from %2 where %1 = '%3' order by tp, %1")
                                   .arg(Field().toLower())
                                   .arg(Table().toLower())
                                   .arg(LineEdit->text().trimmed()),
                                   m_currentdb);

            if( !AlreadyExist->rowCount() )
            {
                AlreadyExist->setQuery(QString("insert into %1 (%2) values ('%3')")
                                         .arg(Table().toLower())
                                         .arg(Field().toLower())
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
        QString New = CreateEditUi();
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
            m_pModelLocal->setQuery(QString("select id, %2 from %1 order by tp, %2")
                               .arg(Table().toLower()).arg(Field().toLower()), m_currentdb);

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
    bool    bHasNew = false;
    QString strTokenNew  = "::ADICIONAR::";

    ConnectDB();

    debug_message("\n-->PersistObjects()\n");
//    m_currentdb = QSqlDatabase::database(objectName());
  //  if(!m_currentdb.isValid())
    //    m_currentdb = QSqlDatabase::database();




    if(!m_currentdb.isValid())
    {
        QMessageBox::information(NULL,
                                 "Database Erro",
                                 "Sem Conexao com banco de dados, abortando persistencia");
        return false;
    }

    CreateTableIfNoExist();

 //   if( count() )
  //  {
        QSqlQueryModel *pList = new QSqlQueryModel;

        pList->setQuery(QString("select %1 from %2 ").arg(Field().toLower()).arg(Table().toLower()),m_currentdb);
        if(pList && pList->rowCount())
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


        for( int i =0; i < count(); i++  )
        {
            bool bFound = false;
            QString Text = itemText(i);

            if( Text == strTokenNew )
                bHasNew = true;

            if(pList && pList->rowCount())
            {
                for( int j = 0; j < pList->rowCount(); j++)
                {
                    if( pList->index(j,0).data().toString() == strTokenNew )
                        bHasNew = true;

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
                                         .arg(Table().toLower())
                                         .arg(Field().toLower())
                                         .arg(Text),m_currentdb);

                if( Add->lastError().isValid())
                        QMessageBox::warning(this,"Erro ao Inserir",
                                                 QString("Query:%1 Resultado:%2")
                                                 .arg(Add->query().lastQuery())
                                                 .arg(Add->lastError().text()));
                delete Add;
            }
        }
 //   }

    if( !bHasNew && CanAdd())
    {
        QSqlQueryModel *Add = new QSqlQueryModel;
        Add->setQuery(QString("insert into %1 (%2, tp) values ('%3', 2)")
                                 .arg(Table().toLower())
                                 .arg(Field().toLower())
                                 .arg(strTokenNew),m_currentdb);
        delete Add;
    }



    m_pModelLocal->setQuery(QString("select id, %1 from %2 order by tp, %1")
                            .arg(Field().toLower())
                            .arg(Table().toLower())
                           ,m_currentdb);


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
    ///
    ///
    ///
    debug_message("-->CreateTableIfNoExist()\n");

    if( UserName().trimmed().isEmpty() )
    {
        debug_message("<--CreateTableIfNoExist() L:%d\n", __LINE__);
        return;
    }

    QSqlQueryModel *CrateTable = new QSqlQueryModel();

    ///
    /// create table if no exists
    ///
    ///
    ///
//    select relname, relnamespace from pg_class join pg_catalog.pg_namespace n ON n.oid = pg_class.relnamespace where n.nspname='public' and relname='%1';

    QString strChecExist = QString("SELECT true FROM pg_tables WHERE tablename = '%1'").arg(Table().toLower().toLower());
    CrateTable->setQuery(strChecExist, m_currentdb);
    if( CrateTable->rowCount())
    {
        delete CrateTable;
        debug_message("<--CreateTableIfNoExist() L:%d\n", __LINE__);
        return;
    }

	QString strPrimType = "integer NOT NULL";
	QSqlDatabase db = QSqlDatabase::database();
	debug_message(" drivername: %s\n", db.driverName().toUtf8().data() );
	if( db.driverName()	 == "QSQLITE" )
        strPrimType = "INTEGER PRIMARY KEY autoincrement";
	//INTEGER PRIMARY KEY (SQLITE)
	
    QString strCreate = QString( "CREATE TABLE %1 ("\
                                 "id %3, "\
                                 "%2 character varying ,"\
                                 "tp integer default 1 );")
                                .arg(Table().toLower()).arg(Field().toLower()).arg(strPrimType);

    CrateTable->setQuery( strCreate, m_currentdb );

    QString strOwner = QString("ALTER TABLE %1 OWNER TO %2;").arg(Table().toLower()).arg(UserName());

    CrateTable->setQuery( strOwner, m_currentdb );


    QString strCreateSequence = QString(" CREATE SEQUENCE %1_id_seq "\
                                        " START WITH 1 "\
                                        " INCREMENT BY 1 "\
                                        " NO MINVALUE "\
                                        " NO MAXVALUE "\
                                        "CACHE 1;").arg(Table().toLower());

    CrateTable->setQuery( strCreateSequence, m_currentdb );

    QString strSeqOwner = QString( "ALTER TABLE %1_id_seq OWNER TO %2; ").arg(Table().toLower()).arg(UserName());

    CrateTable->setQuery( strSeqOwner, m_currentdb );

    QString strOwned = QString("ALTER SEQUENCE %1_id_seq OWNED BY %1.id;").arg(Table().toLower());

    CrateTable->setQuery( strOwned, m_currentdb );

    QString strAlterCol = QString("ALTER TABLE ONLY %1 ALTER COLUMN id SET DEFAULT nextval('%1_id_seq'::regclass);")
            .arg(Table().toLower());

    CrateTable->setQuery( strAlterCol, m_currentdb );

    QString strCata = QString("SELECT pg_catalog.setval('%1_id_seq', 1, false);").arg(Table().toLower());

    CrateTable->setQuery( strCata, m_currentdb );

    QString strpKey = QString("ALTER TABLE ONLY %1 ADD CONSTRAINT prmary_key_%1 PRIMARY KEY (id); ").arg(Table().toLower());

    CrateTable->setQuery( strpKey, m_currentdb );

    delete CrateTable;
    debug_message("<--CreateTableIfNoExist() L:%d\n", __LINE__);

}
