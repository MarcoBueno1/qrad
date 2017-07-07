#include "qradlistview.h"
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
#include <QMimeData>
#include <QFileDialog>
#include <QDesktopServices>

#define ADD_NEW_TOKEN "::ADICIONAR::"
QRadListView::QRadListView(QWidget *parent) :
    QListView(parent)
{
    m_ForeingKey = -1;
    m_IsFileSelect = true;
    setAcceptDrops(true);
    m_pModelLocal = new QSqlQueryModel;
    connect(this, SIGNAL(OnEdit()), this, SLOT(doEdit()));
    connect(this, SIGNAL(OnUpdate(QString)), this, SLOT(doUpdate(QString)));

}
QRadListView::~QRadListView()
{
    if( m_ForeingKey == -1 )
    {
        for( int i = 0 ; i < m_pModelLocal->rowCount(); i++ )
        {
           QSqlQuery *qDelete = new QSqlQuery;

           QString strDelete = QString("delete from %1 where id = %2").arg(Table()).arg(m_pModelLocal->index(i,0).data().toInt());
           qDelete->exec(strDelete);
           delete qDelete;
        }
    }
    delete m_pModelLocal;
}

void QRadListView::showEvent ( QShowEvent * event )
{
    Q_UNUSED(event );

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
    connect(this,SIGNAL(clicked(QModelIndex)),this,SLOT(Activacted(QModelIndex)));

}
void QRadListView::ConnectDB()
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

QString QRadListView::CreateEditUi()
{
    bool doInsert = false;
    QString New;

    if( m_IsFileSelect )
    {
        New = QFileDialog::getOpenFileName(this,
           tr("Adicionar Arquivo"),QDir::currentPath(), tr("Todos (*.*)"));

        doInsert = !New.trimmed().isEmpty();
    }
    else
    {
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
                doInsert = true;
                New = LineEdit->text().trimmed();
            }
        }
        delete LineEdit;
        delete Button;
        delete Layout;
        delete Label;
        delete Window;
    }
    if( doInsert )
    {
        QSqlQueryModel *AlreadyExist = new QSqlQueryModel;
        AlreadyExist->setQuery(QString("select %1 from %2 where %1 = '%3' order by tp, %1")
                               .arg(Field().toLower())
                               .arg(Table().toLower())
                               .arg(New),
                               m_currentdb);

        if( !AlreadyExist->rowCount() )
        {
            AlreadyExist->setQuery(QString("insert into %1 (%2,tp, fkey) values ('%3', 1,%4)")
                                     .arg(Table().toLower())
                                     .arg(Field().toLower())
                                     .arg(New)
                                     .arg(m_ForeingKey),
                                   m_currentdb);
//            New = LineEdit->text().trimmed();

        }

        delete AlreadyExist;
    }


    return New;
}

void QRadListView::doEdit(void)
{
        QString New = CreateEditUi();
        if( !New.isEmpty())
        {
            doUpdate(New);
        }
        else
        {
            setCurrentIndex(model()->index(0,0));
//            setCurrentIndex(0);
        }
}
void QRadListView::doUpdate(QString New)
{
    Q_UNUSED(New);
    m_pModelLocal->setQuery(QString("select id, %2 from %1 where fkey = %3 or %2 = '%4' order by tp, %2")
                       .arg(Table().toLower()).arg(Field().toLower()).arg(m_ForeingKey).arg(ADD_NEW_TOKEN), m_currentdb);

    setModel(m_pModelLocal);
    setModelColumn(1);
    setCurrentIndex(m_pModelLocal->index(0,1));
//            int Index = findText(New);
//            setCurrentIndex(Index);

}
void QRadListView::Activacted(QModelIndex index )
{
   QString text = this->currentIndex().sibling(this->currentIndex().row(),1).data().toString();

//   QMessageBox::information(this,"Activacted", text);
   if( CanAdd() && (text == ADD_NEW_TOKEN) )
   {
         emit OnEdit();
   }
   else
   {
       QDesktopServices::openUrl(QUrl(text, QUrl::TolerantMode));
   }
}
bool QRadListView::PersistObjects()
{
    bool    bHasNew = false;
    QString strTokenNew  = ADD_NEW_TOKEN;

    ConnectDB();

    debug_message("\n-->PersistObjects()\n");

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

/*
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
*/
    if( !bHasNew && CanAdd())
    {
        QSqlQueryModel *Add = new QSqlQueryModel;
        Add->setQuery(QString("insert into %1 (%2, tp, fkey) values ('%3', 2, %4)")
                                 .arg(Table().toLower())
                                 .arg(Field().toLower())
                                 .arg(strTokenNew).arg(m_ForeingKey) ,m_currentdb);
        delete Add;
    }



//    if( m_ForeingKey > -1 )
    {
        m_pModelLocal->setQuery(QString("select id, %1 from %2 where fkey = %3 or %1 = '%4' order by tp, %1")
                                .arg(Field().toLower())
                                .arg(Table().toLower())
                                .arg(m_ForeingKey).arg(strTokenNew)
                               ,m_currentdb);
    }

    setModel(m_pModelLocal);
    setModelColumn(1);

    return true;
}

///
/// \brief QRadListView::CreateTableIfNoExist
///
void QRadListView::CreateTableIfNoExist()
{
    debug_message("-->CreateTableIfNoExist()\n");

    if( UserName().trimmed().isEmpty() )
    {
        debug_message("<--CreateTableIfNoExist() L:%d\n", __LINE__);
        return;
    }

    QSqlQueryModel *CrateTable = new QSqlQueryModel();

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


    QString strCreate = QString( "CREATE TABLE %1 ("\
                                 "id %3, "\
                                 "%2 character varying ,"\
                                 "tp integer default 1, " \
                                 "fkey integer default 0 );")
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
void QRadListView::dropEvent(QDropEvent *event)
{
    QString strPath = event->mimeData()->text();

    QSqlQueryModel *AlreadyExist = new QSqlQueryModel;
    AlreadyExist->setQuery(QString("select %1 from %2 where %1 = '%3' and fkey = %4 order by tp, %1")
                           .arg(Field().toLower())
                           .arg(Table().toLower())
                           .arg(strPath)
                           .arg(m_ForeingKey),
                           m_currentdb);

    if( !AlreadyExist->rowCount() )
    {
        AlreadyExist->setQuery(QString("insert into %1 (%2,tp, fkey) values ('%3', 1,%4)")
                                 .arg(Table().toLower())
                                 .arg(Field().toLower())
                                 .arg(strPath)
                                 .arg(m_ForeingKey),
                               m_currentdb);
//        New = LineEdit->text().trimmed();

    }

    delete AlreadyExist;

    emit OnUpdate("");


    /*
    QMessageBox::information(this, "Drop1:", event->mimeData()->text());


    if(event->mimeData()->hasFormat("text/plain"))
    {
        QMessageBox::information(this, "Drop:", event->mimeData()->text());
//        debug_message("QDropEvent: %s L:%d\n", , __LINE__);
    }
    */
}

void QRadListView::dragEnterEvent(QDragEnterEvent *event)
{
//    setText(tr("<drop content>"));
//    setBackgroundRole(QPalette::Highlight);

    event->acceptProposedAction();
//    emit changed(event->mimeData());
}

void QRadListView::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

bool QRadListView::Save(int fkey )
{
	for( int i = 0 ; i < m_pModelLocal->rowCount(); i++ )
	{
	   QSqlQuery *qUpdate = new QSqlQuery;

       QString strUpdate = QString("update %1 set fkey=%2 where id = %3").arg(Table()).arg(fkey)
	                                                                 .arg(m_pModelLocal->index(i,0).data().toInt());
       qUpdate->exec(strUpdate);
	   delete qUpdate;
	}
    return true;
}

void QRadListView::Load( int fkey )
{
    m_ForeingKey = fkey;
    doUpdate("");
}

void QRadListView::keyPressEvent(QKeyEvent *event)
{
    if (NULL == model() || (0 == model()->rowCount()))
    {
        return;
    }

    switch(event->key())
    {
        case Qt::Key_Delete:
        {
          QString text = this->currentIndex().sibling(this->currentIndex().row(),1).data().toString();
          if(( text != ADD_NEW_TOKEN) &&
                  (QMessageBox::Yes == QMessageBox::question(this, "Remover?", "Deseja remover este item?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes)))
          {

              QSqlQuery *qDelete = new QSqlQuery;
              qDelete->exec(QString("delete from %1 where %2 = '%3' and fkey = %4").arg(Table()).arg(Field()).arg(text).arg(m_ForeingKey));
              delete qDelete;
              doUpdate("");
          }
          break;
        }
        case Qt::Key_Insert:
                doEdit();
                doUpdate("");
                break;
    }
    QListView::keyPressEvent(event);
}
