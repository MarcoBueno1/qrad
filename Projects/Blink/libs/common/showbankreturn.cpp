#include "showbankreturn.h"
#include "ui_showbankreturn.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include "qradshared.h"
#include "qraddebug.h"
#include "qradprogresswindow.h"


#define MEM_TABLE "create table mem_table( seunumero character varying, datavencto character varying, nomepagador character varying, datapagto character varying, valor character varying, valorpago character varying, tipoop int)"
#define INSERT_MEM_TABLE "insert into mem_table( seunumero, datavencto, nomepagador, datapagto, valor, valorpago, tipoop) values( '%1', '%2','%3','%4','%5','%6', %7)"

#define SELECT_MEM_TABLE "select seunumero as \"Nº Sis\", datavencto as \"Vencto\", nomepagador as \"Nome\", datapagto as \"Pg. Em\", valor as \"ValorR$\", valorpago as \"Pago R$ / Situação\", tipoop from mem_table"


ShowBankReturn::ShowBankReturn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowBankReturn)
{
    ui->setupUi(this);

    connect(ui->pushButtonOk, SIGNAL(clicked()),this,SLOT(accept()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()),this,SLOT(reject()));

    connect(ui->radioButtonAlterados, SIGNAL(clicked(bool)),this,SLOT(RadioBtnChanged()));
    connect(ui->radioButtonBaixados, SIGNAL(clicked(bool)),this,SLOT(RadioBtnChanged()));
    connect(ui->radioButtonLiquidados, SIGNAL(clicked(bool)),this,SLOT(RadioBtnChanged()));
    connect(ui->radioButtonOutros, SIGNAL(clicked(bool)),this,SLOT(RadioBtnChanged()));
    connect(ui->radioButtonRegistrados, SIGNAL(clicked(bool)),this,SLOT(RadioBtnChanged()));
    connect(ui->radioButtonTodos, SIGNAL(clicked(bool)),this,SLOT(RadioBtnChanged()));

    setWindowTitle("Conteúdo do(s) arquivo(s) de retorno (.RET)");
    m_ColumnPaid =  new ColumnRetBankAndPaid;
}

ShowBankReturn::~ShowBankReturn()
{
    delete m_ColumnPaid;
    delete ui;
}


QSqlDatabase ShowBankReturn::ConfigTempDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","memory_sqlql");

//    db.setDatabaseName("qraddb");
    db.setDatabaseName(":memory:");
    db.setHostName("localhost");
    db.setPassword("dsmpassword");
    db.setUserName("dsm");


    db.open();
    return db;
}

QString ShowBankReturn::FriendlyOpType(QRAD_BANKTKT_TYPEOP opType)
{
    switch( opType )
    {
      case tpRegistered:
                        return QString("Registered");
      case tpLiquidated:
                        return QString("Liquidated");
      case tpDeed:
                        return QString("Deed");
      case tpChanges:
                        return QString("Changes");
      case tpOther:
                        return QString("Other");
    }
}


void ShowBankReturn::BuildTable(QList<BankTicket*> *list)
{
    db = ConfigTempDatabase();


    QSqlQuery *query = new QSqlQuery(db);

    query->exec(MEM_TABLE);

    for( int i = 0; i < list->count(); i++ )
    {
        BankTicket *tkt = list->at(i);

        debug_message("Entry Type: %s\n", FriendlyOpType(tkt->getTpOp()).toLatin1().data());

        //seunumero, datavencto, nomepagador, datapagto, valor, valorpago

        QString Insert = QString(INSERT_MEM_TABLE).arg(tkt->getSeuNumero()).arg(tkt->getdtVencto().toString(FMT_DATE)).arg(tkt->getNomePagador()).arg(tkt->getdtPagto().toString(FMT_DATE)).arg((tkt->getValor())).arg(tkt->getValorPago()).arg(tkt->getTpOp());
        if( !query->exec(Insert))
        {
            debug_message("Erro na execucao de: %s\n\nErro:%s", Insert.toLatin1().data(), query->lastError().text().toLatin1().data());

        }
    }
    delete query;
}

bool ShowBankReturn::Exec(QList<BankTicket*> *list, QString Path)
{
    QStringList PathList(Path);

    return Exec(list, PathList);
}

bool ShowBankReturn::Exec(QList<BankTicket*> *list, QStringList Paths)
{
    QRAD_SHOW_PRPGRESS("Lendo informações...");
    for( int i = 0; i < Paths.count(); i++ )
    {
        QString Path = Paths.at(i);
        QRAD_SHOW_PRPGRESS_STEP(QString("Processando: %1").arg(Path));
        QCoreApplication::processEvents();

        IBankTicketParser *Parser = BankTicketParserFactory::GetParser(Path);

        if(!Parser)
        {
            QRAD_HIDE_PRPGRESS();
            QMessageBox::warning(NULL,"Oops!", QString("O arquivo %1 é incompativel!").arg(Path));
        }

        if( !Parser->Parse(list) || (list->count() == 0 ))
        {
            QRAD_HIDE_PRPGRESS();
            QMessageBox::warning(NULL,"Oops!", QString("Nenhum pagamento encontrado no arquivo: %1").arg(Path));
        }
        delete Parser;
    }

    QRAD_SHOW_PRPGRESS_STEP("Construindo tabela...");
    QCoreApplication::processEvents();
    BuildTable(list);

    ui->groupBox->setTitle(QString("Total Itens:%1").arg(list->count()));

    m_model = new QSqlQueryModel;

///select seunumero as \"Nº Sis\",       0
///       datavencto as \"Vencto\",      1
///       nomepagador as \"Nome\",       2
///       datapagto as \"Pg. Em\",       3
///       valor as \"ValorR$\",          4
///       valorpago as \"Vlr. Pago R$\", 5
///       tipoop                         6
/// from mem_table

    m_model->setQuery(SELECT_MEM_TABLE,db);
    ui->tableView->setModel(m_model);
    ui->tableView->hideColumn(6);
    ui->tableView->setItemDelegateForColumn(5, m_ColumnPaid);

    QRAD_HIDE_PRPGRESS();
    if( exec() == QDialog::Accepted)
    {
        delete m_model;
        db.close();
        return true;
    }
    delete m_model;
    db.close();
    return false;
}
void ShowBankReturn::RadioBtnChanged()
{
   QString Sql = SELECT_MEM_TABLE;
   QString where;

   QString count = "select count(*) from mem_table ";

    if( ui->radioButtonAlterados->isChecked())
    {
        where = QString(" where tipoop = %1").arg(tpChanges);
    }
    else if( ui->radioButtonBaixados->isChecked())
    {
        where = QString(" where tipoop = %1").arg(tpDeed);
    }
    else if( ui->radioButtonLiquidados->isChecked())
    {
        where = QString(" where tipoop = %1").arg(tpLiquidated);
    }
    else if( ui->radioButtonOutros->isChecked())
    {
        where = QString(" where tipoop = %1").arg(tpOther);
    }
    else if( ui->radioButtonRegistrados->isChecked())
    {
        where = QString(" where tipoop = %1").arg(tpRegistered);

    }
    else if( ui->radioButtonTodos->isChecked())
    {

    }
    Sql += where;
    count += where;

    QSqlQuery *q = new QSqlQuery(db);
    if( q->exec(count))
    {
        q->first();
        int count = q->value(0).toInt();
        ui->groupBox->setTitle(QString("Total Itens:%1").arg(count));
    }
    delete q;


    m_model->setQuery(Sql,db);
    ui->tableView->setModel(m_model);
    ui->tableView->hideColumn(6);
    ui->tableView->setItemDelegateForColumn(5, m_ColumnPaid);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

}
