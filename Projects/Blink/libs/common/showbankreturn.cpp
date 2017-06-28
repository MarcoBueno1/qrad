#include "showbankreturn.h"
#include "ui_showbankreturn.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include "qradshared.h"
#include "qraddebug.h"


#define MEM_TABLE "create table mem_table( seunumero character varying, datavencto character varying, nomepagador character varying, datapagto character varying, valor character varying, valorpago character varying)"
#define INSERT_MEM_TABLE "insert into mem_table( seunumero, datavencto, nomepagador, datapagto, valor, valorpago) values( '%1', '%2','%3','%4','%5','%6')"

#define SELECT_MEM_TABLE "select seunumero as \"Nº Sis\", datavencto as \"Vencto\", nomepagador as \"Nome\", datapagto as \"Pg. Em\", valor as \"ValorR$\", valorpago as \"Vlr. Pago R$\" from mem_table"


ShowBankReturn::ShowBankReturn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowBankReturn)
{
    ui->setupUi(this);

    connect(ui->pushButtonOk, SIGNAL(clicked()),this,SLOT(accept()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()),this,SLOT(reject()));
}

ShowBankReturn::~ShowBankReturn()
{
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

void ShowBankReturn::BuildTable(QList<BankTicket*> *list)
{
    db = ConfigTempDatabase();


    QSqlQuery *query = new QSqlQuery(db);

    query->exec(MEM_TABLE);

    for( int i = 0; i < list->count(); i++ )
    {
        BankTicket *tkt = list->at(i);

        //seunumero, datavencto, nomepagador, datapagto, valor, valorpago

        QString Insert = QString(INSERT_MEM_TABLE).arg(tkt->getSeuNumero()).arg(tkt->getdtVencto().toString(FMT_DATE)).arg(tkt->getNomePagador()).arg(tkt->getdtPagto().toString(FMT_DATE)).arg((tkt->getValor())).arg(tkt->getValorPago());
        if( !query->exec(Insert))
        {
            debug_message("Erro na execucao de: %s\n\nErro:%s", Insert.toLatin1().data(), query->lastError().text().toLatin1().data());

        }
    }
    delete query;
}

bool ShowBankReturn::Exec(QList<BankTicket*> *list, QString Path)
{
    IBankTicketParser *Parser = BankTicketParserFactory::GetParser(Path);

    if(!Parser)
    {
        QMessageBox::warning(NULL,"Oops!", QString("O arquivo %1 é incompativel!").arg(Path));
        return false;
    }

    if( !Parser->Parse(list) || (list->count() == 0 ))
    {
        QMessageBox::warning(NULL,"Oops!", QString("Nenhum pagamento encontrado no arquivo: %1").arg(Path));
        delete Parser;
        return false;
    }
    delete Parser;

    BuildTable(list);

    m_model = new QSqlQueryModel;

    m_model->setQuery(SELECT_MEM_TABLE,db);
    ui->tableView->setModel(m_model);
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
