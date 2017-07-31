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
#include "editticket.h"
#include "reasonextratax.h"
#include "editcondtax.h"
#include <QUrl>
#include <QDir>
#include "editextratx.h"
#include <QMessageBox>
#include "qradprogresswindow.h"
#include "emailgui.h"
#include "qrademail.h"
#include "qradreportmanager.h"
#include "shipper.h"

/*
#define FIN_AP_WITH_NO_PAYER "select * from dweller d2 "\
                             " where not exists ("\
                             " ( select a, t from (select a.id as a, t.id as t  from dweller d inner join ap a on d.ap = a.id inner join tower t on t.id = d.tower "\
                             " where d.payer = true) as foo where d2.ap = a and d2.tower = t ))"
*/

#define FIN_MISSING_TICKETS_THIS_MONTH "select * from dweller d where payer = true and removed <> true and id not in "\
                             "( select clientid from ticket t where t.removed <> true and t.vencto = '%1' and type = %2 %3 ) and free <> true and movedout <> true order by d.id;"

#define FIN_MISSING_TICKETS_THIS_MONTH_VALUE "select * from dweller d where payer = true and removed <> true and id not in "\
                             "( select clientid from ticket t where t.removed <> true and t.valor=%1 and t.vencto = '%2' and type = %3 %4 )  and movedout <> true  order by d.id;"


#define FIN_AP_WITH_NO_PAYER " select ap.numero as \"Ap\", tower.name as \"Torre\" from ap, tower  where not exists( select a2,b2 from ( select d.ap a2 , "\
                             " d.tower b2 from dweller d where d.payer = true and d.removed <> true and movedout <> true) as foo where a2=ap.id and b2=tower.id) and tower.tp = 1"


#define FIN_GET_LAST_NUMBER  "select * from ticket order by nossonumero desc limit 1"

#define UPDATE_ALL_TICKETS "update ticket set status = %3 where (type = %1) and removed <> true and status = %2"
#define COUNT_ALL_SHIPP_TICKETS "select count(*) from ticket where (type = %1) and removed <> true and status = %2"

#define FIN_PRINT_ALL_TICKETS "select * from  ticket where (type = %1) and removed <> true and status = %2"


#define DEFAULT_REM_DIR "C:\\DVL\\"
#define DEFAULT_PDF_FILE "C:\\ACBrMonitorPLUS\\BOLETOS.PDF"


#define SELECT_EMAILS "select count(*) from dweller where notifbyemail = true and email <> ''"

#define SELECT_DWELER_FOR_EXTRA_TAX "select * from dweller d where removed = false and payer = true order by d.id"


////  After process .ret file
#define UPDATE_TICKETS_TO_PAID "ticket set pagoem = '%1', valorpago = %2, status=%3, obs='%4', vuser = %5 where nossonumero in (%6) and vencto = '%7'"
#define SET_TICKETS_REGISTERED "ticket set status=%1 where nossonumero in (%2)"
#define UPDATE_ACCOUNT_TO_RECEIVE  "update fin_accounttoreceive set paiddate = '%1', valuepaid = %2, paid = true where id in ( select accountid from ticket where  nossonumero in (%3) and vencto = '%4') "
///




TicketController::TicketController()
{
  g_tkt = new BuildTkt;
  m_pTktConfig =  NULL;
}

TicketController::~TicketController()
{

    delete g_tkt ;
}


//  alter table ticket add column issuedate date ;

bool TicketController::BuildTicket( DwellerList *dlist,
                                   QDate date,
                                   int type,
                                   QString Obs,
                                   int ExtraTxId,
                                   double dValue,
                                   double dDiscount)
{

    QRadProgressWindow *pW = QRadProgressWindow::getInstance();
    pW->setDetail(QString("Construindo boletos ..."));

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
            Obs = ext->getObs();
//            ReasonExtraTax *pReason = ReasonExtraTax::findByid(ext->getMotivo(), true);
//            if(pReason)
//            {
//                Obs = "Tx. Extra " + pReason->getDescription() + "("+date.toString(FMT_DATE)+")";
//                delete pReason;
//            }
            delete ext;
        }
    }


    for( int i = 0; i < dlist->count(); i++)
    {
         QCoreApplication::processEvents();
         nLastNumber++;
         Dweller *pDweller = dlist->at(i);
         Ap *ap = pDweller->getAp();
         Tower *pTower = Tower::findByid(pDweller->gettower());
         metreage *pMetr = metreage::findByid(ap->getMetreageId(), true);
         if( !pMetr )
         {
             pW->hide();
             QMessageBox::warning(NULL, "Oops!", QString("O apartamento %1 não possui metragem configurada, não poderá ser gerado boleto para este apartamento!").arg(ap->getNumber()));
             pW->setDetail(QString("Construindo boletos ..."));
             continue;
         }

         ticket *tkt = new ticket ;

         tkt->setNossoNumero(nLastNumber);
         tkt->setclientid(pDweller->getid());
         tkt->setidticket(1); // as default
         tkt->setObs(Obs);//.arg(date.toString(FMT_DATE)));
         tkt->setSeuNumero(QString("%1%2").arg(pDweller->gettower()).arg(pDweller->getAp()->getNumber()).toInt());
         tkt->setType(type);

         if( type == tpTxCond )
         {
             debug_message("dValue=%02.02f dDiscount=%02.02f dlist->count()=%d\n", dValue,dDiscount,dlist->count() );
             if( (dValue>0) && (dlist->count() ==1) ) /// aplciavel apenas se for uma unidade ..
             {
                debug_message("Setando valor...\n");
                tkt->setValor(dValue);
             }
             else
             {
                 debug_message("Setando valor...getMontlyValue \n");
                 tkt->setValor(pMetr->getMontlyValue());
             }
             if(( dDiscount >= 0 ) && (dlist->count() ==1) ) /// aplciavel apenas se for uma unidade ..
             {
                   debug_message("Setando desconto \n");
                  tkt->setDiscount(dDiscount);
             }
             else
             {
                 debug_message("Setando desconto m_pTktConfig->getDiscount\n");
                 tkt->setDiscount(m_pTktConfig->getDiscount());
             }

         }
         else
             tkt->setValor(txValue);

         tkt->setVencto(date);
         tkt->setSendStatus(pDweller->getNotifByEmail()&&!pDweller->getemail().isEmpty()?stPending:stDisabled);
         tkt->setUser(QRadConfig::GetCurrentUserId());
         tkt->setExtraTxId(ExtraTxId);
         tkt->setIssueDate(QDate::currentDate());



         if( !tkt->Save() )
         {
             pW->hide();
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
         account->setDescription(QString("%1 AP: %2-%3").arg(Obs).arg(ap->getNumber()).arg(pTower->getName()));
         account->setIssueDate(QDate::currentDate());
         account->setVencDate(date);
         account->setValue( tkt->getValor());
         account->setAccountTypeId(2); // tipo condominio
         if( account->Save())
         {
             debug_message("account->getId()=%d\n", account->getId());
             if( !tkt->updateAccountId(account->getId()))
                 debug_message("nao consegui fazer updateAccountId=%d\n", account->getId());

         }
         else
         {
             pW->hide();
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
    pW->hide();
    return true;
}

bool TicketController::BuildTicketExtra( extratx *pTx )
{
    QRAD_SHOW_PRPGRESS("Criando Taxa Extra...");

    DwellerList *dlist ;

    QRAD_HIDE_PRPGRESS();
    Editextratx *edt = new Editextratx;
    if( QDialog::Accepted != edt->exec())
    {
        QMessageBox::warning(NULL,
                             QString("Cancelado!"),
                             QString("Operação cancelada!"));
        delete edt;
        return false;
    }
    QRAD_SHOW_PRPGRESS_STEP(QString("Criando Taxa Extra..."));
    pTx = edt->GetSaved();

    QDate date = pTx->getData();


    QString SQL;
    for( int i = 0; i < pTx->getTimes(); i++ )
    {
        debug_message("getTimes()=%d\n",pTx->getTimes());

        if( pTx->getAll() )
        {
           SQL = QString(FIN_MISSING_TICKETS_THIS_MONTH_VALUE).arg(pTx->getValue()).arg(date.toString(FMT_DATE_DB)).arg(tpTxExtr).arg("");
           debug_message("pTx->getAll()=true\n");
           dlist = Dweller::findBy(SQL);
           if( !dlist )
           {
               QRAD_HIDE_PRPGRESS();
               QMessageBox::information(NULL, "Oops!", QString("Todos os boletos de taixa condominial com vencimento %1 já foram emitidos!").arg(date.toString(FMT_DATE)));
               return false;
           }

        }
        else
        {
            QList<int> list = edt->GetSavedIds();
            QList<int> OkList = list;
            for( int i = 0; i < list.count();i++ )
            {
               SQL = QString(FIN_MISSING_TICKETS_THIS_MONTH_VALUE).arg(pTx->getValue()).arg(date.toString(FMT_DATE_DB)).arg(tpTxExtr).arg(QString(" and clientid = %1").arg(pTx->getDweller()));
               dlist = Dweller::findBy(SQL);
               if( !dlist )
               {
                 QString dwdata;
                 int nId = list.at(i);
                 Dweller *dName = Dweller::findByid(nId);
                 if( dName )
                 {
                     Ap *ap = Ap::findByid(dName->getap());
                     Tower *tw = Tower::findByid(dName->gettower());
                     if( tw )
                     {
                         dwdata += tw->getName() + " - ";
                         delete tw;
                     }
                     if( ap )
                     {
                         dwdata += ap->getNumber() + " - ";
                         delete ap;
                     }
                     dwdata += dName->getName();
                     delete dName;
                 }
                 QRAD_HIDE_PRPGRESS();
                 if( QMessageBox::No == QMessageBox::question(NULL, "Atenção",
                                                              QString("Já foi emitido boleto para %1 este morador neste mês, deseja realmente emitir um novo boleto?").arg(dwdata),
                                                              QMessageBox::Yes|QMessageBox::No,
                                                              QMessageBox::No ))
                     OkList.removeOne(nId);
               }
           }
           if( !OkList.count())
           {
               QRAD_HIDE_PRPGRESS();
               return false;
           }
           QString Where;
           //debug_message("OkList()=%s\n",OkList.join().data());
           for( int i = 0; i < OkList.count();i++ )
           {
               if( !Where.length() )
                   Where = " where id = ";
               else
                   Where += " or id = ";

               Where += QString("%1 ").arg(OkList.at(i));
           }
           //debug_message("where=%s\n",Where.toLatin1().data());
           dlist = Dweller::findBy(QString("select * from dweller %1").arg(Where));
           //debug_message("dlist->count()=%s\n",Where.toLatin1().data());


        }

        debug_message("Tamanho da lista:%d\n", dlist->count());


        BuildTicket( dlist,
                     date,
                     tpTxExtr,
                     "TX EXTRA",
                     pTx->getId());

        date.addMonths(1);
    }

    delete edt;
    QRAD_HIDE_PRPGRESS();
    return true;
}

bool TicketController::BuildTicketCond(int id )
{
    if(!InitAcbr())
        return false;

    QRadProgressWindow *pW = QRadProgressWindow::getInstance();
    pW->setDetail(QString("Criando Taxa Condominial..."));


    QDate date = QDate::currentDate();
    configList *pConfig = config::findAll();

    if( pConfig )
    {
        date.setDate(date.year(),date.month(),pConfig->at(0)->getDefaultPayDate().day());

        if( QDate::currentDate() > date )
            date.setDate(date.year(),date.month(),QDate::currentDate().day());
    }

    Editcondtx *pCondTx = new Editcondtx ;
    pCondTx->setVencto(date);
    pCondTx->setObs(QString("TAXA CONDOMINIAL (%1)").arg(date.toString(FMT_DATE)));
    pW->hide();
    if( QDialog::Rejected == pCondTx->exec())
    {
        delete pCondTx;
        return false;
    }
    pW->setDetail(QString("Criando Taxa Condominial..."));


    date = pCondTx->getVencto();
    double dValue = pCondTx->getValue();
    double dDiscount = pCondTx->getDisocunt();

    debug_message("dValue=%02.02f dDiscount=%02.02f logo apos os gets\n", dValue,dDiscount );


    DwellerList *dlist;

    if( pCondTx->ToAll() ) /// for all
    {
        ///
        /// antes de tudo testar consistencia para verificar se falta alguem.
        ///
        dlist = Dweller::findBy(QString(FIN_MISSING_TICKETS_THIS_MONTH).arg(date.toString(FMT_DATE_DB)).arg(tpTxCond).arg(""));
        if( !dlist )
        {
          pW->hide();
          QMessageBox::information(NULL, "Oops!", "Todos os boletos de taixa condominial deste mes ja foram emitidos!");
          return false;
        }
    }
    else
    {

        QList<int> list = pCondTx->GetSaved();
        QList<int> OkList = list;
        for( int i = 0; i < list.count();i++ )
        {
            int nId = list.at(i);
            dlist = Dweller::findBy(QString(FIN_MISSING_TICKETS_THIS_MONTH).arg(date.toString(FMT_DATE_DB)).arg(tpTxCond).arg(QString(" and clientid = %1 ").arg(nId)));
            if( !dlist )
            {
              QString dwdata;
              Dweller *dName = Dweller::findByid(nId);
              if( dName )
              {
                  Ap *ap = Ap::findByid(dName->getap());
                  Tower *tw = Tower::findByid(dName->gettower());
                  if( tw )
                  {
                      dwdata += tw->getName() + " - ";
                      delete tw;
                  }
                  if( ap )
                  {
                      dwdata += ap->getNumber() + " - ";
                      delete ap;
                  }
                  dwdata += dName->getName();
                  delete dName;
              }
              pW->hide();
              if( QMessageBox::No == QMessageBox::question(NULL, "Atenção",
                                                           QString("Já foi emitido boleto para %1 este morador neste mês, deseja realmente emitir um novo boleto?").arg(dwdata),
                                                           QMessageBox::Yes|QMessageBox::No,
                                                           QMessageBox::No ))
                  OkList.removeOne(nId);
            }
   //     dlist = Dweller::findBy(QString("select * from dweller where id = %1").arg(id));
        }
        if( !OkList.count())
        {
            pW->hide();
            return false;
        }
        QString Where;
        for( int i = 0; i < OkList.count();i++ )
        {
            if( !Where.length() )
                Where = " where id = ";
            else
                Where += " or id = ";

            Where += QString("%1 ").arg(OkList.at(i));
        }
        dlist = Dweller::findBy(QString("select * from dweller %1").arg(Where));
    }


    debug_message("dValue=%02.02f dDiscount=%02.02f antes do BuildTicket\n", dValue,dDiscount );

    bool bRet = BuildTicket( dlist,
                             date,
                             tpTxCond,
                             pCondTx->getObs(),
                             0,
                             dValue,
                             dDiscount);

    pW->hide();
    delete pCondTx;
    return bRet;


}

bool TicketController::doPrepare(BBO_TYPE type, BBOL_STATUS status)
{
    QString Discount = "0,00";
    QString Type;

    QRadProgressWindow *pW = QRadProgressWindow::getInstance();
    pW->setDetail(QString("Preparando itens..."));


    if( type == tpAll)
    {
        Type = QString("%1 or type = %2 ").arg(tpTxCond).arg(tpTxExtr);
    }
    else
    {
        Type = QString("%1 ").arg(type);
    }
    ticketList *tktList =  ticket::findBy(QString(FIN_PRINT_ALL_TICKETS).arg(Type).arg(status));
    if( !tktList)
    {
        pW->hide();
        return false;
    }
    if(!InitAcbr())
    {
        pW->hide();
        return false;
    }

    debug_message("SQL: %s\n",QString(FIN_PRINT_ALL_TICKETS).arg(Type).arg(status).toLatin1().data());
    debug_message("tktList->count()=%d\n", tktList->count());
    for( int i = 0; i < tktList->count();i++)
    {
        QCoreApplication::processEvents();
        ticket *ptkt = tktList->at(i);
        Dweller *pDweller = Dweller::findByid(ptkt->getclientid());

        QString value = QString("%1").arg(ptkt->getValor());
      //  value.replace(".",",");

       /// regra garden
       /// type== txExtra= not discount
/*       if( tpTxExtr == ptkt->getType())
       {
           Discount = "0,00";
       }
       else
       {
#warning "Ajsutar para permitir editar também desconto na ediçao de boleto."
#if(0)
           double valor       = value.replace(",", ".").toDouble();
           Discount = QString("%1").arg(QRadRound::PowerRound(QRadRound::PowerRound(valor/100)*(m_pTktConfig->getDiscount())));
           Discount.replace(".", ",");
#endif

       }
*/
     double valor       = value.replace(",", ".").toDouble();
     Discount = QString("%1").arg(QRadRound::PowerRound(QRadRound::PowerRound(valor/100)*(ptkt->getDiscount())));
     Discount.replace(".", ",");


      g_tkt->AppendTicket( pDweller,
                           value,
                           ptkt->getVencto(),
                           QString("%1").arg(ptkt->getNossoNumero()),
                           QString("%1").arg(ptkt->getSeuNumero()),
                           //ptkt->getType()==tpTxExtr?ptkt->getObs():"",
                           ptkt->getObs(),
                           Discount );
    }
    pW->hide();
    return g_tkt->AddTickets();
}

bool TicketController::doPrint(BBO_TYPE type, BBOL_STATUS status, ticket *ptkt)
{
    QRadProgressWindow *pW = QRadProgressWindow::getInstance();
    pW->setDetail(QString("Preparando impressão..."));

    if(!ptkt)
    {
        if( !doPrepare(type, status) )
        {
            pW->hide();
            return false;
        }
        if( !g_tkt->Print(false) ) // use default name ( same as remessa acbrlib ... )
        {
            pW->hide();
            return false;
        }

        QSqlQuery query;
        QString Type;

        if( type == tpAll)
        {
            Type = QString("%1 or type = %2 ").arg(tpTxCond).arg(tpTxExtr);
        }
        else
        {
            Type = QString("%1 ").arg(type);
        }

//        shipper *pShip = new shipper;
//        pShip->setAmount();

        if( !query.exec(QString(UPDATE_ALL_TICKETS).arg(Type).arg(status).arg(stBuiltShipp)))
        {
            debug_message("Warning: Nao foi possivel atualizar o status de tickets para stBuiltShipp(%s)!!\n", query.lastError().text().toLatin1().data());
        }

        pW->hide();
        return true;
    }
    else
    {
        Dweller *pDweller = Dweller::findByid(ptkt->getclientid());

        QString Path = QString("c:\\dvl\\acbr\\%1").arg(QDate::currentDate().toString("dd_MM_yyyy"));
        QDir  dir;
        dir.mkpath(Path);

        QString value = QString("%1").arg(ptkt->getValor());
        value.replace(".",",");
        debug_message("Antes do Add Ticket\n");

        if(!InitAcbr())
        {
            pW->hide();
            return false;
        }

        double valor       = ptkt->getValor();
        QString Discount = QString("%1").arg(QRadRound::PowerRound(QRadRound::PowerRound(valor/100)*(ptkt->getDiscount())));
        Discount.replace(".", ",");

        g_tkt->AppendTicket(pDweller, value, ptkt->getVencto(),QString("%1").arg(ptkt->getNossoNumero()),QString("%1").arg(ptkt->getSeuNumero()),ptkt->getObs(),Discount);
        debug_message("Antes AddTickets\n");
        if(g_tkt->AddTickets())
        {
            Ap * ap = Ap::findByid(pDweller->getap());
            Tower *tw = Tower::findByid(pDweller->gettower());


            QString FullFileName = QString("%1\\boleto_%2-%3-%4-%5.pdf").arg(Path)
                    .arg(ap->getNumber())
                    .arg(tw->getName())
                    .arg(pDweller->getName())
                    .arg(ptkt->getNossoNumero());

            delete ap;
            delete tw;

            if(g_tkt->Print(false,FullFileName))
            {
//                ptkt->updateLoId(ptkt->saveFile(FullFileName));
                pW->hide();
                delete pDweller;
                return true;
            }
        }
        delete pDweller;
    }

    pW->hide();
    return false;
}

bool TicketController::doShipp(QString dir, QString filename,BBO_TYPE type, BBOL_STATUS status)
{
    QRadProgressWindow *pW = QRadProgressWindow::getInstance();
    pW->setDetail(QString("Preparando remessa..."));

    if( dir.isEmpty() || filename.isEmpty())
    {
        dir = QString("c:\\dvl\\acbr\\%1").arg(QDate::currentDate().toString("dd_MM_yyyy"));
 //DEFAULT_REM_DIR;
        QDir  d;
        d.mkpath(dir );

        filename = QString("%1").arg(QDate::currentDate().toString("ddMMyy"));
    }
    if( !doPrepare(type, status))
    {
        pW->hide();
        debug_message("Saindo antes do shipp, prepare com erro...");
        return false;
    }

    if(g_tkt->BuildShipping(dir,filename))
    {
        /// read and save shipping file to db
        //// abrir pasta
        pW->hide();
        return true;
    }
    pW->hide();
    return false;
}


bool TicketController::InitAcbr()
{
   MainCompany *pCompany    = MainCompany::findByPrimaryKey(1);

   if( m_pTktConfig )
       delete m_pTktConfig;
   m_pTktConfig = ticketconfig::findByPrimaryKey(1); // default only one ticket
   bankaccount *pAccount    = bankaccount::findByPrimaryKey(1);   // default only one banckaccount

   if(!pCompany || !m_pTktConfig || !pAccount)
   {
       debug_message("Company=%x, tktconfig=%x, bAccount=%x\n",pCompany, m_pTktConfig, pAccount );
       QMessageBox::warning(NULL, "Oops!", QString("Configurações incompletas, por favor configure Conta, Empresa, Boleto!"));

   }
   BankModel *pBank   = BankModel::findByPrimaryKey(pAccount->getBanco()) ;


   if(!g_tkt->Init(pCompany,m_pTktConfig,pBank, pAccount ))
   {
       QMessageBox::warning(NULL, "Oops!", QString("Erro na inicialização da biblioteca de geração de boletos!"));
       return false;
   }
   return true;
}


void TicketController::OpenRemDir()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(QString("c:\\dvl\\acbr\\%1").arg(QDate::currentDate().toString("dd_MM_yyyy"))));//, QUrl::TolerantMode);
}
void TicketController::OpenPDF()
{
//    QString toName = QString("%1Boletos%2.pdf").arg(DEFAULT_REM_DIR).arg(QDate::currentDate().toString("ddMMyyyy"));
//    QFile::rename(DEFAULT_PDF_FILE,toName);
//QDesktopServices::openUrl(QUrl::fromLocalFile(toName));//, QUrl::TolerantMode);

    OpenRemDir();

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

bool TicketController::Edit(int id)
{
    bool bRet = false;
    ticket *tkt = ticket::findByid(id,true);
    if( !tkt)
    {
        QMessageBox::warning(NULL, "Oops!","Selecione um boleto para poder Editar!");
        return bRet;
    }
    if( tkt->getStatus() == stPaid)
    {
        QMessageBox::warning(NULL, "Oops!","Este boleto NÃO pode ser editado, Ele já foi pago!");
        return bRet;
    }

    Editticket *pTkt =  new Editticket;
    pTkt->SetModel(tkt);
    if( QDialog::Accepted == pTkt->exec())
    {
        bRet = true;

    }

    delete tkt;
    delete pTkt;
    return bRet;
}

bool TicketController::Remove(int id, QString strMsgText)
{
    bool bRet = false ;

    ticket *tkt = ticket::findByid(id,true);
    if( !tkt)
    {
        QMessageBox::warning(NULL, "Oops!","Selecione um boleto para poder Remover!");
        return bRet;
    }

    if( tkt->getStatus() == stPaid)
    {
        QMessageBox::warning(NULL, "Oops!","Este boleto NÃO pode ser editado, Ele já foi pago!");
        return bRet;
    }


    if( QMessageBox::Yes == QMessageBox::question(NULL,
                                              QString("Atenção!"),
                                              QString("Tem certeza que deseja remover este boleto? \n%1").arg(strMsgText),
                                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes ))
    {
        if(tkt->updateRemoved(true))
        {
            bRet = true;
            AccountToReceiveModel *account = AccountToReceiveModel::findById(tkt->getAccountId(),true);
            if( account )
            {
                account->setRemoved(true);
                if(account->Save())
                {
                 AccountToReceiveHistoryModel *his = new AccountToReceiveHistoryModel;
                 his->setDate(QDate::currentDate());
                 his->setTime(QTime::currentTime());
                 his->setTypeOperation(AccountOpRemove);
                 his->setUserId(QRadConfig::GetCurrentUserId());
                 his->Save();
                 delete his;
                }
                delete account;
            }
        }
    }

   delete tkt;
   return bRet;
}



bool TicketController::ProcessRetList(QList<BankTicket*> *list)
{
    QRadProgressWindow *pW = QRadProgressWindow::getInstance();
    pW->setDetail(QString("Processando lista de retorno..."));

    QSqlDatabase::database().transaction();

    QSqlQuery *query = new QSqlQuery; // default database
    QString Update;
    bool bRet = true;

    for( int i = 0; i < list->count(); i++ )
    {
        QCoreApplication::processEvents();
        BankTicket *tkt = list->at(i);

#define UPDATE_TICKETS_TO_PAID "ticket set pagoem = '%1', valorpago = %2, status=%3, obs='%4', vuser = %5 where nossonumero in (%6) and vencto = '%7'"

        if( tkt->getpago() )
        {
            Update  =  QString(UPDATE_TICKETS_TO_PAID)
                    .arg(tkt->getdtPagto().toString(FMT_DATE_DB))
                    .arg(tkt->getValorPago().replace(",",".").toDouble())
                    .arg(stPaid)
                    .arg(QString("AUTOMATIZADO EM %1").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy-hh:mm:ss")))
                    .arg(QRadConfig::GetCurrentUserId())
                    .arg(tkt->getNossoNumero())
                   // .arg(tkt->getSeuNumero())
                    .arg(tkt->getdtVencto().toString(FMT_DATE_DB));

            if( !query->exec(Update))
            {
                bRet = false;
                debug_message("Erro na execucao de: %s\n\nErro:%s", Update.toLatin1().data(), query->lastError().text().toLatin1().data());
            }
            else
            {
#define UPDATE_ACCOUNT_TO_RECEIVE  "update fin_accounttoreceive set paiddate = '%1', valuepaid = %2, paid = true where id in ( select accountid from ticket where  nossonumero in (%3) and vencto = '%4') "
                Update = QString(UPDATE_ACCOUNT_TO_RECEIVE)
                        .arg(tkt->getdtPagto().toString(FMT_DATE_DB))
                        .arg(tkt->getValorPago().replace(",",".").toDouble())
                        .arg(tkt->getNossoNumero())
//                        .arg(tkt->getSeuNumero())
                        .arg(tkt->getdtVencto().toString(FMT_DATE_DB));

                if( !query->exec(Update))
                {
                    bRet = false;
                    debug_message("Erro na execucao de: %s\n\nErro:%s", Update.toLatin1().data(), query->lastError().text().toLatin1().data());
                }
            }
        }
        else if(tkt->getTpOp()==tpRegistered)
        {
//#define SET_TICKETS_REGISTERED "ticket set status=%1 where nossonumero in (%2) and seunumero in (%3) and valor = '%4' and status = %5 and vencto = '%7' and removed <> true"
            Update  =  QString(SET_TICKETS_REGISTERED)
                       .arg(stRegistered)
                       .arg(tkt->getNossoNumero());
//                    .arg(tkt->getNossoNumero())

            if( !query->exec(Update))
            {
                bRet = false;
                debug_message("Erro na execucao de: %s\n\nErro:%s", Update.toLatin1().data(), query->lastError().text().toLatin1().data());
            }
        }

    }
    QSqlDatabase::database().commit();
    delete query;
    pW->hide();
    return bRet;
}

bool TicketController::UpdateTickets(QList<BankTicket*> *list)
{
    bool bRet = true;
    BankTicket *pCurrent;
    ////
    //// Por enquanto persistindo apenas estado de registrado...
    //// ( aguardando para verificar como ficará o erro operacional do pessoal )
    QRAD_SHOW_PRPGRESS("Atualizando estado dos boletos..");
    for( int i = 0; (i< list->count()) && bRet ; i++ )
    {
         pCurrent = list->at(i);
         QRAD_SHOW_PRPGRESS_STEP(QString("Processando Item:%1 Nosso Número:%2 Valor R$:%3").arg(i).arg(pCurrent->getNossoNumero()).arg(QRadMoney::MoneyHumanForm4(pCurrent->getValor())));
         QCoreApplication::processEvents();
         if( pCurrent->getTpOp() == tpRegistered)
         {
             ticket *ptkt = ticket::findByNossoNumero(pCurrent->getNossoNumero().toInt(),true);
             if( ptkt && ((ptkt->getStatus() != stPaid) && (ptkt->getStatus() != stRegistered)))
             {
                 bRet = ptkt->updateStatus(stRegistered);
                 delete ptkt;
             }
         }
         else if( pCurrent->getTpOp() == tpLiquidated) // pagos apenas com vencimento acima de 05/07/2017
         {
             ticket *ptkt = ticket::findByNossoNumero(pCurrent->getNossoNumero().toInt(),true);
             if(( ptkt && (ptkt->getStatus() != tpLiquidated)) && ptkt->getVencto() > QDate::fromString("05072017","ddMMyyyy"))
             {
                 bRet = ptkt->UpdateToPaid(pCurrent->getdtPagto(),pCurrent->getValorPago().replace(",",".").toDouble());
                 delete ptkt;
             }
         }
    }
    QRAD_HIDE_PRPGRESS();
    if( !bRet)
        QMessageBox::information(NULL,QString("Oops!"), QString("Erro na operacao, nao foi possivel atualizar o item cujo NossoNúmero é %1!!").arg(pCurrent->getNossoNumero()));
    else
        QMessageBox::information(NULL,QString("Ok!"), QString("Operação concluída!!"));
    return bRet;
}

bool TicketController::SendEmail(int id, bool bSilent )
{
  QRAD_SHOW_PRPGRESS("Perparando infraestrutura ...")

  debug_message("ticket antes\n");

  ticket *tkt = ticket::findByid(id,true);
  if(!tkt)
  {
      if( !bSilent )
      {
          QRAD_HIDE_PRPGRESS();
          QMessageBox::warning(NULL,QString("Oops!"), QString("Não foi possivel encontrar o boleto!"));
      }
      return false;
  }

  debug_message("Dweller antes\n");

  Dweller *pdw = Dweller::findByid(tkt->getclientid());
  if(!pdw)
  {
      if( !bSilent )
      {
          QRAD_HIDE_PRPGRESS();
          QMessageBox::warning(NULL,QString("Oops!"), QString("Não foi possivel encontrar o Morador!"));
      }
      delete tkt;
      delete pdw;
      return false;
  }
  if(pdw->getemail().isEmpty())
  {
      if( !bSilent )
      {
          QRAD_HIDE_PRPGRESS();
          QMessageBox::warning(NULL,QString("Oops!"), QString("Este morador não possui e-mail cadastrado!"));
      }
      delete tkt;
      delete pdw;
      return false;
  }

  debug_message("ap antes\n");

  Ap *ap = Ap::findByid(pdw->getap());
  Tower *tw = Tower::findByid(pdw->gettower());

  debug_message("FullFileName antes\n");


  QString Path = QString("c:\\dvl\\acbr\\%1").arg(QDate::currentDate().toString("dd_MM_yyyy"));
  QString FullFileName = QString("%1\\boleto_%2-%3-%4-%5.pdf").arg(Path)
          .arg(ap->getNumber())
          .arg(tw->getName())
          .arg(pdw->getName())
          .arg(tkt->getNossoNumero());

  QRAD_HIDE_PRPGRESS();
  debug_message("doPrint antes\n");

  if( !doPrint((BBO_TYPE)tkt->getType(),(BBOL_STATUS)tkt->getStatus(),tkt))
  {
      if( !bSilent )
      {
          QMessageBox::warning(NULL,QString("Oops!"), QString("Erro ao gerar arquivo %1!").arg(FullFileName));
      }
      delete tkt;
      delete pdw;
      delete ap;
      delete tw;
      return false;
  }

  debug_message("Print ok\n");


  EmailGui *gui = new EmailGui;

  gui->setFile(FullFileName);
  gui->setTo(pdw->getemail());
  if( QDate::currentDate() == QDate(2017,07,27)) /// apenas para corrigir erro de envio sem desconto
  {
      gui->setSubject(tkt->getObs()+ " COM DESCONTO",true);
      gui->setText(QString("Prezado Morador,<br/><br/>        Segue em anexo boleto para pagamento( POR FAVOR DESCONSIDERAR E-MAIL ANTERIOR).<br/><br/><br/><br/>Atenciosamente\n"));
  }
  else
  {
      gui->setSubject(tkt->getObs());
      gui->setText(QString("Prezado Morador,<br/><br/>        Segue em anexo boleto para pagamento.<br/><br/><br/><br/>Atenciosamente\n"));
  }

  if( !bSilent )
  {
      if( QDialog::Accepted != gui->exec())
      {
          QMessageBox::warning(NULL,QString("Cancelado!"), QString("Operação Cancelada!"));
          delete tkt;
          delete pdw;
          delete gui;
          delete ap;
          delete tw;
          return false;
      }
  }

  debug_message("QRadEmail antes\n");


  QRAD_SHOW_PRPGRESS_STEP("Enviando...");
  QRadEmail *mail = QRadEmail::getInstance();

  debug_message("QRadEmail depois\n");
  if(!mail)
  {
      if( !bSilent )
      {
          QRAD_HIDE_PRPGRESS();
          QMessageBox::warning(NULL,QString("Oops!"), QString("Não foi possível inicializar infraestrutura de e-mail!"));
      }
      delete tkt;
      delete pdw;
      delete gui;
      delete ap;
      delete tw;
      return false;
  }

  debug_message("Enviando... %s %s %s %s %s\n",
                pdw->getName().toLatin1().data(),
                pdw->getemail().toLatin1().data(),
                tkt->getObs().toLatin1().data(),
                FullFileName.toLatin1().data());

  QStringList files;
  files.append(FullFileName);
  bool bRet = mail->Send(pdw->getName(),pdw->getemail(),tkt->getObs(),gui->getText(),files);
  if(bRet)
  {
      bRet = tkt->updateSendStatus(stSent);
  }
  else
  {
      tkt->updateSendStatus(stErrSent);
  }
  debug_message("delete 1\n");

  delete tkt;
  debug_message("delete 2\n");
  delete pdw;
  debug_message("delete 3\n");
  delete gui;
  debug_message("delete 4\n");
  delete ap;
  debug_message("delete 5\n");
  delete tw;

  if( !bSilent )
      QRAD_HIDE_PRPGRESS();

  debug_message("retornando\n");
  return bRet;
}
bool TicketController::ReportExaro( QSqlQueryModel *model, QString reportTitle)
{
    if( model->rowCount() <= 0 )
    {
        QMessageBox::information(NULL,"Oops!", "Não existem dados para serem exibidos!");
        return false;
    }
    QRAD_SHOW_PRPGRESS("Preparando dados...");

    QRadReportManager   *report = new QRadReportManager();
    int total = 0;

    if ( !report->load( "report-tickets" ) )
    {
       QRAD_HIDE_PRPGRESS();
       QMessageBox::critical( NULL, "Erro", "Falha ao carregar arquivo modelo." );
       delete report;
       return false;
    }
    QRAD_SHOW_PRPGRESS_STEP("Carregando Informações...");
    ExaroReport *pReport = dynamic_cast<ExaroReport*>(report->getReport());
    pReport->setReportTitle(reportTitle);
/*
//select t.nossonumero as \"Nº.Banco\", t.seunumero as \"Nº.Sis\", a.numero as \"Ap\", tw.name as \"Torre\", d.name as \"Morador\", t.vencto as \"Vencto\", t.pagoem \"Pago em\", "\
//    " t.valor as \"Valor R$\", t.valorpago as \"Pago R$\"
*/
    QRAD_SHOW_PRPGRESS_STEP("Gerando Relatório...");
    pReport->setFields("Nº.Banco","Ap", "Torre", "Morador","obs", "Vencto",  "Valor R$"," "," "," ");
    pReport->setTitles("Nº.Banco","Ap", "Torre", "Morador","Obs", "Vencto",  "Valor R$"," "," "," ");

    for (int index = 0; index < model->rowCount(); index++)
    {
        total += model->record(index).value("Valor R$").toFloat();
    }

    pReport->setQueryName("tickets");
    QString strAux = model->query().lastQuery().replace("t.valor", "to_char(t.valor, 'FM9G999G990D00')").replace(", t.vencto ",", to_char(t.vencto, 'dd-mm-yyyy') ");
    debug_message("QRY.....................................:%s\n", strAux.toLatin1().data());
    report->setQuery("tickets", model->query().lastQuery().replace("  t.valor", "to_char(t.valor, 'FM9G999G990D00')")
                     .replace(", t.vencto ",", to_char(t.vencto, 'dd-mm-yyyy') "));


    report->replace("{Itens}", QString("%1").arg(model->rowCount()));
    report->replace("TOTAL1", "");
    report->setAttributeMoneyValue("TOTAL2", total);
    debug_message("TOTAL...........................:%s\n", QRadMoney::MoneyHumanForm4(QString::number(total)).toLatin1().data());

    QRAD_HIDE_PRPGRESS();
    if ( !report->show() )
    {
        QMessageBox::critical( NULL, "Oops!", QString::fromUtf8( "Problema ao exibir o relatório." ) );
    }

    delete report;

    return true;
}

#define SQL_EMAIL_ITEMS "select t.* from "\
                        " ticket t inner join dweller d on d.id = t.clientid inner join vuser u on u.id = t.vuser "\
                        " inner join ap a on a.id = d.ap inner join tower tw on tw.id = d.tower "\
                        " where (t.status = %1 or t.status =%2 or t.status = %3 ) and  t.sendstatus <> %4 and t.vencto >= '%5' order by t.id desc; "

//>=

bool TicketController::SendToAll()
{
  QDate *date = new QDate(2017,07,26); // hardcoded ..
  QString SQL = QString(SQL_EMAIL_ITEMS).arg(stCreated).arg(stBuiltShipp).arg(stRegistered).arg(stSent).arg(date->toString(FMT_DATE_DB));

  delete date;

  debug_message("antes initacbr \n");

  QRAD_SHOW_PRPGRESS("Iniciando envio...");


  if(!InitAcbr())
  {
      QMessageBox::information(NULL, "Oops!", "Não foi possivel inicializar a infraestrutura!");
      return false;
  }

  debug_message("SQL: %s\n",SQL.toLatin1().data());
  ticketList *tktlst = ticket::findBy(SQL);
  if( !tktlst )
  {
      QMessageBox::information(NULL, "Oops!", "Não existem e-mails pendentes para envio!");
      return true;
  }
  for( int i = 0; tktlst && (i < tktlst->count()); i++ )
  {
      ticket *tkt = tktlst->at(i);
      if( tkt->getSendStatus() == stDisabled) // double check, perhaps user changed configuration abaut that....
      {
          Dweller *pDw = Dweller::findByid(tkt->getclientid());
          if(pDw && pDw->getNotifByEmail() && !pDw->getemail().trimmed().isEmpty())
          {
              tkt->setSendStatus(stPending);
              tkt->updateStatus(stPending);
          }
      }
      if( tkt->getSendStatus() == stPending)
      {
          SendEmail(tkt->getid(),true);
      }
  }
  QRAD_HIDE_PRPGRESS();

  QMessageBox::information(NULL, "Ok!", "Todos e-mails foram enviados!");
  return true;

  return true;
}
