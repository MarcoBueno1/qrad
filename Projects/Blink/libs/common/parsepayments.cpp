#include "parsepayments.h"
#include "itauinterface.h"
#include "qradshared.h"
#include "qraddebug.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

#define MEM_TABLE "create table mem_table( seunumero character varying, datavencto character varying, nomepagador character varying, datapagto character varying, valor character varying, valorpago character varying)"
#define INSERT_MEM_TABLE "insert into mem_table( seunumero, datavencto, nomepagador, datapagto, valor, valorpago) values( '%1', '%2','%3','%4','%5','%6')"

#define UPDATE_TICKETS "ticket set pagoem = '%1', valorpago = %2, status=4, obs='%3', vuser = 1 where nossonumero in (%4) and seunumero in (%5) and type = 0 and status = 2 and vencto = '%6' and removed <> true "
#define UPDATE_ACCOUNT_TO_RECEIVE  "update fin_accounttoreceive set paiddate = '%1', valuepaid = %2, paid = true where id in ( select accountid from ticket where  nossonumero in (%3) and seunumero in (%4) and type = 0 and status = 4 and vencto = '%5' and removed <> true ) "


ParsePayments::ParsePayments()
{

}

ParsePayments::~ParsePayments()
{

}

QSqlDatabase ParsePayments::ConfigTempDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","memory_sqlql");

//    db.setDatabaseName("qraddb");
    db.setDatabaseName(":memory:");
    db.setHostName("localhost");
    db.setPassword("dsmpassword");
    db.setUserName("dsm");


    return db;
}



bool ParsePayments::BuildFrom(QString Path)
{
    QList<itauticket*> list;
    if( !ItauInterface::Load(&list,Path))
    {
        QMessageBox::warning(NULL,"Oops!", QString("Nenhum pagamento encontrado no arquivo: %1").arg(Path));
        return false;
    }


    QSqlDatabase db = ConfigTempDatabase();
    
    
    QSqlQuery *query = new QSqlQuery(db);
    
    query->exec(MEM_TABLE);

    for( int i = 0; i < list.count(); i++ )
    {
        itauticket *tkt = list.at(i);

        //seunumero, datavencto, nomepagador, datapagto, valor, valorpago

        QString Insert = QString(INSERT_MEM_TABLE).arg(tkt->getSeuNumero()).arg(tkt->getdtVencto().toString(FMT_DATE)).arg(tkt->getNomePagador()).arg(tkt->getdtPagto().toString(FMT_DATE)).arg((tkt->getValor())).arg(tkt->getValorPago());
        if( !query->exec(Insert))
        {
            debug_message("Erro na execucao de: %s\n\nErro:%s", Insert.toLatin1().data(), query->lastError().text().toLatin1().data());

        }
    }
    delete query;

     /// create view for user confirm or abort informations.
     if( 1)
     {
         QSqlDatabase::database().transaction();

         query = new QSqlQuery; // default database
         for( int i = 0; i < list.count(); i++ )
         {
             itauticket *tkt = list.at(i);
             QString Update  =  QString(UPDATE_TICKETS)
                     .arg(tkt->getdtPagto().toString(FMT_DATE_DB))
                     .arg(tkt->getValorPago().replace(",",".").toDouble())
                     .arg(QString("AUTOMATIZADO EM %1").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy-hh:mm:ss")))
                     .arg(tkt->getNossoNumero())
                     .arg(tkt->getSeuNumero())
                     .arg(tkt->getdtVencto().toString(FMT_DATE_DB));

             if( !query->exec(Update))
             {
                 debug_message("Erro na execucao de: %s\n\nErro:%s", Update.toLatin1().data(), query->lastError().text().toLatin1().data());
             }
             else
             {
                // "update fin_accounttoreceive set paiddate = '%1', valuepaid = %2, paid = true where id in ( select accountid from ticket where  nossonumero in (%3) and seunumero in (%4) and type = 0 and status = 4 and vencto = '%5' and removed <> true ) "
                 Update = QString(UPDATE_ACCOUNT_TO_RECEIVE)
                         .arg(tkt->getdtPagto().toString(FMT_DATE_DB))
                         .arg(tkt->getValorPago().replace(",",".").toDouble())
                         .arg(tkt->getNossoNumero())
                         .arg(tkt->getSeuNumero())
                         .arg(tkt->getdtVencto().toString(FMT_DATE_DB));

                 if( !query->exec(Update))
                 {
                     debug_message("Erro na execucao de: %s\n\nErro:%s", Update.toLatin1().data(), query->lastError().text().toLatin1().data());
                 }
             }
         }
         QSqlDatabase::database().commit();
         delete query;
         return true;
     }
  return false;
}
