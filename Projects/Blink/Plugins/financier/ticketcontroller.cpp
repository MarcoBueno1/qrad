#include "ticketcontroller.h"
#include "ticketconfig.h"
#include "metreage.h"
#include "configmodel.h"
#include "qradshared.h"
#include "qraddebug.h"
#include "qradconfig.h"
#include "accounttoreceivemodel.h"
#include "accounttoreceivehistorymodel.h"
#include <QDesktopServices>
#include <QUrl>

#include <QMessageBox>

/*
#define FIN_AP_WITH_NO_PAYER "select * from dweller d2 "\
                             " where not exists ("\
                             " ( select a, t from (select a.id as a, t.id as t  from dweller d inner join ap a on d.ap = a.id inner join tower t on t.id = d.tower "\
                             " where d.payer = true) as foo where d2.ap = a and d2.tower = t ))"
*/

#define FIN_MISSING_TICKETS_THIS_MONTH "select * from dweller d where payer = true and removed <> true and id not in "\
                             "( select clientid from ticket t where t.removed <> true and t.vencto = '%1' and type = %2 );"

#define FIN_MISSING_TICKETS_THIS_MONTH_VALUE "select * from dweller d where payer = true and removed <> true and id not in "\
                             "( select clientid from ticket t where t.removed <> true and t.valor=%1 and t.vencto = '%2' and type = %3 %4 );"


#define FIN_AP_WITH_NO_PAYER " select ap.numero as \"Ap\", tower.name as \"Torre\" from ap, tower  where not exists( select a2,b2 from ( select d.ap a2 , "\
                             " d.tower b2 from dweller d where d.payer = true and d.removed <> true) as foo where a2=ap.id and b2=tower.id) and tower.tp = 1"


#define FIN_GET_LAST_NUMBER  "select * from ticket order by nossonumero desc limit 1"

#define UPDATE_ALL_TICKETS "update ticket set status = %3 where type = %1 and removed <> true and status = %2"

#define FIN_PRINT_ALL_TICKETS "select * from  ticket where type = %1 and removed <> true and status = %2"


#define DEFAULT_REM_DIR "C:\\DVL\\"
#define DEFAULT_PDF_FILE "C:\\ACBrMonitorPLUS\\BOLETOS.PDF"


#define SELECT_EMAILS "select count(*) from dweller where notifbyemail = true and email <> ''"

#define SELECT_DWELER_FOR_EXTRA_TAX "select * from dweller d where removed = false and payer = true order by d.id"
TicketController::TicketController()
{
  g_tkt = new BuildTkt;
}

TicketController::~TicketController()
{

    delete g_tkt ;
}

bool TicketController::BuildTicket(DwellerList *dlist,
                                  QDate date,
                                  int type,
                                  QString Obs,
                                  int ExtraTxId)
{

    double txValue = 0;
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
    if( ExtraTxId )
    {
        extratx *ext = extratx::findById(ExtraTxId,true);
        if( ext )
        {
            txValue = ext->getValue();
            delete ext;
        }
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
         tkt->setObs(QString("%1 %2").arg(Obs).arg(date.toString(FMT_DATE)));
         tkt->setSeuNumero(QString("%1%2").arg(pDweller->gettower()).arg(pDweller->getAp()->getNumber()).toInt());
         tkt->setType(type);
         if( type == tpTxCond)
             tkt->setValor(pMetr->getMontlyValue());
         else
             tkt->setValor(txValue);

         tkt->setVencto(date);
         tkt->setSendStatus(pDweller->getNotifByEmail()&&!pDweller->getemail().isEmpty()?stPending:stDisabled);
         tkt->setUser(QRadConfig::GetCurrentUserId());
         tkt->setExtraTxId(ExtraTxId);


         if( !tkt->Save() )
         {
             QMessageBox::warning(NULL, "Oops!", QString("Não foi possivel gerar o boleto de %1 - %2 %3!").arg(pDweller->getName()).arg(pTower->getName()));
             delete pMetr;
             delete pTower;
             delete tkt;
             return false;
         }
        // doPrint(tpTxCond,stCreated,tkt);
        // g_tkt->AppendTicket(pDweller,  QString("%1").arg(tkt->getValor()), tkt->getVencto(),QString("%1").arg(tkt->getNossoNumero()),QString("%1").arg(tkt->getSeuNumero()));


         AccountToReceiveModel *account  = new  AccountToReceiveModel;

         account->setClientId(pDweller->getid());
         account->setDescription(QString("%1 AP: %2-%3 (%4)").arg(Obs).arg(ap->getNumber()).arg(pTower->getName()).arg(pDweller->getName()));
         account->setIssueDate(QDate::currentDate());
         account->setVencDate(date);
         account->setValue( tkt->getValor());
         if( account->Save())
         {
             tkt->updateAccountId(account->getId());
         }
         else
         {
             QMessageBox::warning(NULL, "Oops!", QString("Não foi possivel salvar conta: Boleto %1 - %2!").arg(tkt->getid()).arg(account->lastError().text()));

         }
         ///
         /// History
         AccountToReceiveHistoryModel *accountToReceiveHistoryModel = new AccountToReceiveHistoryModel;

         accountToReceiveHistoryModel->setAccountToReceiveId(account->getId());
         accountToReceiveHistoryModel->setTypeOperation(AccountOpCreate);
         accountToReceiveHistoryModel->setUserId(QRadConfig::GetCurrentUserId());
         accountToReceiveHistoryModel->setDate(QDate::currentDate());
         accountToReceiveHistoryModel->setTime(QTime::currentTime());

         accountToReceiveHistoryModel->Save();

         delete accountToReceiveHistoryModel;

         ///


         delete account;
         delete pMetr;
         delete pTower;
         delete tkt;
    }
}

bool TicketController::BuildTicketExtra( extratx *pTx )
{
//    Dweller *pD;
    DwellerList *dlist ;
    QDate date = pTx->getData();

    /*
    if(pTx->getAll())
    {
        dlist = Dweller::findBy(QString(SELECT_DWELER_FOR_EXTRA_TAX));
        if( !dlist )
        {
          QMessageBox::information(NULL, "Oops!", "Nenhum morador pagador encontrado, por favor ajustar o cadastro!");
          return false;
        }
    }
    else
    {
         dlist = new DwellerList;

         pD = Dweller::findByid(pTx->getDweller());
         dlist->append(pD);
    }
    */

    QString SQL;
    for( int i = 0; i < pTx->getTimes(); i++ )
    {

        if( pTx->getAll() )
        {
           SQL = QString(FIN_MISSING_TICKETS_THIS_MONTH_VALUE).arg(pTx->getValue()).arg(date.toString(FMT_DATE_DB)).arg(tpTxExtr).arg("");
        }
        else
        {
           SQL = QString(FIN_MISSING_TICKETS_THIS_MONTH_VALUE).arg(pTx->getValue()).arg(date.toString(FMT_DATE_DB)).arg(tpTxExtr).arg(QString(" and clientid = %1").arg(pTx->getDweller()));

        }
        dlist = Dweller::findBy(SQL);
        if( !dlist )
            continue;

        BuildTicket( dlist,
                     date,
                     tpTxExtr,
                     "TX EXTRA",
                     pTx->getId());

        date.addMonths(1);
    }

/*    if(!pTx->getAll())
    {
        delete pD;
        delete dlist;
    }*/

    return true;
}

bool TicketController::BuildTicketCond(int id )
{
    QDate date = QDate::currentDate();
    configList *pConfig = config::findAll();

    if( pConfig )
    {
        date.setDate(date.year(),date.month(),pConfig->at(0)->getDefaultPayDate().day());
    }

    if(!InitAcbr())
        return false;


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

        BuildTicket( dlist,
                     date,
                     tpTxCond,
                     "TAXA CONDOMINIAL");


/*        if( !g_tkt->AddTickets() )
        {
            QMessageBox::warning(NULL, "Oops!", QString("Não foi possivel adicionar boletos!"));
            return false;
        }
*/
/*
        if( g_tkt->BuildShipping(DEFAULT_REM_DIR, "rem001.rem"))
        {
            QMessageBox::warning(NULL, "Oops!", QString("Não foi possivel criar o arquivo de remessa!"));
            return false;
        }
*/
        return true;
    }
    return false;
}

bool TicketController::doPrepare(BBO_TYPE type, BBOL_STATUS status)
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

bool TicketController::doPrint(BBO_TYPE type, BBOL_STATUS status, ticket *ptkt)
{
    if(!ptkt)
    {
        if( !doPrepare(type, status) )
        {
            return false;
        }
        if( !g_tkt->Print() )
        {
            return false;
        }
        return true;
    }
    else
    {
        if(!InitAcbr())
        {
            return false;
        }
        Dweller *pDweller = Dweller::findByid(ptkt->getclientid());

        QString value = QString("%1").arg(ptkt->getValor());
        value.replace(".",",");
        g_tkt->AppendTicket(pDweller, value, ptkt->getVencto(),QString("%1").arg(ptkt->getNossoNumero()),QString("%1").arg(ptkt->getSeuNumero()));
        if(g_tkt->AddTickets())
        {
            if(g_tkt->Print())
            {
                ptkt->updateLoId(ptkt->saveFile(DEFAULT_PDF_FILE));
                return true;
            }
        }
    }

    return false;
}

bool TicketController::doShipp(QString dir, QString filename,BBO_TYPE type, BBOL_STATUS status)
{
    if( dir.isEmpty() || filename.isEmpty())
    {
        dir = DEFAULT_REM_DIR;
        filename = "rem001.rem";
    }
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

   if(!pCompany || !pTktConfig || !pAccount)
   {
       debug_message("Company=%x, tktconfig=%x, bAccount=%x\n",pCompany, pTktConfig, pAccount );
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


void TicketController::OpenRemDir()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(DEFAULT_REM_DIR));//, QUrl::TolerantMode);
}
void TicketController::OpenPDF()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(DEFAULT_PDF_FILE));//, QUrl::TolerantMode);

}
void TicketController::SendEmail()
{
/*    DwellerList *pEmails = Dweller::findBy(SELECT_EMAILS);
    if( pEmails )
    {
        for( int i = 0; i < pEmails->count();i++)
        {
            Send(pEmails->at(i));
        }
    }
   */
}
void TicketController::Send(Dweller *pDweller )
{

}
