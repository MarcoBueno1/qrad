#include "ticketcontroller.h"
#include "ticketconfig.h"
#include "dweller.h"
#include "metreage.h"
#include "configmodel.h"
#include "qradshared.h"
#include "qraddebug.h"
#include "accounttoreceivemodel.h"

#include <QMessageBox>

/*
#define FIN_AP_WITH_NO_PAYER "select * from dweller d2 "\
                             " where not exists ("\
                             " ( select a, t from (select a.id as a, t.id as t  from dweller d inner join ap a on d.ap = a.id inner join tower t on t.id = d.tower "\
                             " where d.payer = true) as foo where d2.ap = a and d2.tower = t ))"
*/

#define FIN_MISSING_TICKETS_THIS_MONTH "select * from dweller d where payer = true and removed <> true and id not in "\
                             "( select clientid from ticket t where t.removed <> true and t.vencto = '%1' and type = %2 );"


#define FIN_AP_WITH_NO_PAYER " select ap.numero as \"Ap\", tower.name as \"Torre\" from ap, tower  where not exists( select a2,b2 from ( select d.ap a2 , "\
                             " d.tower b2 from dweller d where d.payer = true and d.removed <> true) as foo where a2=ap.id and b2=tower.id) "


#define FIN_GET_LAST_NUMBER  "select * from ticket order by nossonumero desc limit 1"

#define UPDATE_ALL_TICKETS "update ticket set status = %3 where type = %1 and removed <> true and status = %2"

#define FIN_PRINT_ALL_TICKETS "select * from  ticket where type = %1 and removed <> true and status = %2"


TicketController::TicketController()
{
  g_tkt = new BuildTkt;
}

TicketController::~TicketController()
{

    delete g_tkt ;
}

bool TicketController::BuildTicketCond(int id )
{
    QDate date = QDate::currentDate();
    configList *pConfig = config::findAll();

    if( pConfig )
    {
        date.setDate(date.year(),date.month(),pConfig->at(0)->getDefaultPayDate().day());
    }

    if( !id ) /// for all
    {
        ///
        /// antes de tudo testar consistencia para verificar se falta alguem.
        ///
        DwellerList *dlist = Dweller::findBy(QString(FIN_MISSING_TICKETS_THIS_MONTH).arg(date.toString(FMT_DATE_DB)).arg(tpTxCond));
        if( !dlist )
        {
          QMessageBox::information(NULL, "Oops!", "Todos os boletos de taixa condominial deste mes ja foram emitidos!");
          return false;
        }
        int nLastNumber = 0;
        ticketList *ptList = ticket::findBy(FIN_GET_LAST_NUMBER);
        if ( ptList)
            nLastNumber = ptList->at(0)->getNossoNumero();
        if( !nLastNumber)
        {
            ticketconfigList *ptktConfig = ticketconfig::findAll();
            if(ptktConfig)
                nLastNumber = ptktConfig->at(0)->getNossoNumero();
        }


        for( int i = 0; i < dlist->count(); i++)
        {
             nLastNumber++;
             Dweller *pDweller = dlist->at(i);
             Ap *ap = pDweller->getAp();
             Tower *pTower = Tower::findByid(pDweller->gettower());
             metreage *pMetr = metreage::findByid(ap->getMetreageId(), true);
             if( !pMetr )
             {
                 QMessageBox::warning(NULL, "Oops!", QString("O apartamento %1 não possui metragem configurada, não poderá ser gerado boleto para este apartamento!").arg(ap->getNumber()));
                 continue;
             }

             ticket *tkt = new ticket ;

             tkt->setNossoNumero(nLastNumber);
             tkt->setclientid(pDweller->getid());
             tkt->setidticket(1); // as default
             tkt->setObs(QString("TAXA CONDOMINIAL %1").arg(date.toString(FMT_DATE)));
             tkt->setSeuNumero(QString("%1%2").arg(pDweller->gettower()).arg(pDweller->getAp()->getNumber()).toInt());
             tkt->setType(tpTxCond);
             tkt->setValor(pMetr->getMontlyValue());
             tkt->setVencto(date);
             if( !tkt->Save() )
             {
                 QMessageBox::warning(NULL, "Oops!", QString("Não foi possivel gerar o boleto de %1 - %2 %3!").arg(pDweller->getName()).arg(pTower->getName()));
                 delete pMetr;
                 delete pTower;
                 delete tkt;
                 return false;
             }

             AccountToReceiveModel *account  = new  AccountToReceiveModel;

             account->setClientId(pDweller->getid());
             account->setDescription(QString("TX CONDOMINIAL AP: %1-%2 (%3)").arg(ap->getNumber()).arg(pTower->getName()).arg(pDweller->getName()));
             account->setIssueDate(QDate::currentDate());
             account->setVencDate(date);
             account->setValue(pMetr->getMontlyValue());
             if( account->Save())
             {
                 tkt->updateAccountId(account->getId());
             }


             delete account;
             delete pMetr;
             delete pTower;
             delete tkt;
        }
        return true;
    }
    return false;
}

bool TicketController::doPrepare(BBO_TYPE type, BBO_TYPE status)
{
    ticketList *tktList =  ticket::findBy(QString(FIN_PRINT_ALL_TICKETS).arg(type).arg(status));
    if( !tktList)
    {
        return false;
    }
    if(!InitAcbr())
    {
        return false;
    }

    for( int i = 0; i < tktList->count();i++)
    {
        ticket *ptkt = tktList->at(i);
        Dweller *pDweller = Dweller::findByid(ptkt->getclientid());

        QString value = QString("%1").arg(ptkt->getValor());
        value.replace(".",",");
        g_tkt->AppendTicket(pDweller, value, ptkt->getVencto(),QString("%1").arg(ptkt->getNossoNumero()),QString("%1").arg(ptkt->getSeuNumero()));
    }
    return g_tkt->AddTickets();
}

bool TicketController::doPrint(BBO_TYPE type, BBO_TYPE status, int id)
{
    if(!id)
    {
        if( !doPrepare(type, status))
        {
            return false;
        }
        if(!g_tkt->Print())
        {
            return false;
        }
        return true;
    }

    return false;
}

bool TicketController::doShipp(QString dir, QString filename,BBO_TYPE type, BBO_TYPE status)
{
    if( !doPrepare(type, status))
    {
        return false;
    }

    if(g_tkt->BuildShipping(dir,filename))
    {
        QSqlQuery query;

        if( !query.exec(QString(UPDATE_ALL_TICKETS).arg(type).arg(status).arg(stBuiltShipp)))
        {
            debug_message("Warning: Nao foi possivel atualizar o status de tickets para stBuiltShipp(%s)!!\n", query.lastError().text().toLatin1().data());
        }

        /// read and save shipping file to db
        //// abrir pasta
        return true;
    }
    return false;
}


bool TicketController::InitAcbr()
{
   MainCompany *pCompany    = MainCompany::findByPrimaryKey(1);
   ticketconfig *pTktConfig = ticketconfig::findByPrimaryKey(1); // default only one ticket
   bankaccount *pAccount    = bankaccount::findByPrimaryKey(1);   // default only one banckaccount

   if(pCompany || !pTktConfig || !pAccount)
   {
       QMessageBox::warning(NULL, "Oops!", QString("Configurações incompletas, por favor configure Conta, Empresa, Boleto!"));

   }
   BankModel *pBank   = BankModel::findByPrimaryKey(pAccount->getBanco()) ;


   if(!g_tkt->Init(pCompany,pTktConfig,pBank, pAccount ))
   {
       QMessageBox::warning(NULL, "Oops!", QString("Erro na inicialização da biblioteca de geração de boletos!"));
       return false;
   }
   return true;
}
