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

/*
#define FIN_AP_WITH_NO_PAYER "select * from dweller d2 "\
                             " where not exists ("\
                             " ( select a, t from (select a.id as a, t.id as t  from dweller d inner join ap a on d.ap = a.id inner join tower t on t.id = d.tower "\
                             " where d.payer = true) as foo where d2.ap = a and d2.tower = t ))"
*/

#define FIN_MISSING_TICKETS_THIS_MONTH "select * from dweller d where payer = true and removed <> true and id not in "\
                             "( select clientid from ticket t where t.removed <> true and t.vencto = '%1' and type = %2 %3 ) and free <> true order by d.id;"

#define FIN_MISSING_TICKETS_THIS_MONTH_VALUE "select * from dweller d where payer = true and removed <> true and id not in "\
                             "( select clientid from ticket t where t.removed <> true and t.valor=%1 and t.vencto = '%2' and type = %3 %4 ) order by d.id;"


#define FIN_AP_WITH_NO_PAYER " select ap.numero as \"Ap\", tower.name as \"Torre\" from ap, tower  where not exists( select a2,b2 from ( select d.ap a2 , "\
                             " d.tower b2 from dweller d where d.payer = true and d.removed <> true) as foo where a2=ap.id and b2=tower.id) and tower.tp = 1"


#define FIN_GET_LAST_NUMBER  "select * from ticket order by nossonumero desc limit 1"

#define UPDATE_ALL_TICKETS "update ticket set status = %3 where (type = %1) and removed <> true and status = %2"

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
            ReasonExtraTax *pReason = ReasonExtraTax::findByid(ext->getMotivo(), true);
            if(pReason)
            {
                Obs = "Tx. Extra " + pReason->getDescription() + "("+date.toString(FMT_DATE)+")";
                delete pReason;
            }
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
             if( (dValue>0) && (dlist->count() ==1) ) /// aplciavel apenas se for uma unidade ..
             {
                tkt->setValor(dValue);
             }
             else
             {
                 tkt->setValor(pMetr->getMontlyValue());
             }
             if(( dDiscount > 0 ) && (dlist->count() ==1) ) /// aplciavel apenas se for uma unidade ..
             {
                  tkt->setDiscount(dDiscount);
             }
             else
                 tkt->setDiscount(m_pTktConfig->getDiscount());

         }
         else
             tkt->setValor(txValue);

         tkt->setVencto(date);
         tkt->setSendStatus(pDweller->getNotifByEmail()&&!pDweller->getemail().isEmpty()?stPending:stDisabled);
         tkt->setUser(QRadConfig::GetCurrentUserId());
         tkt->setExtraTxId(ExtraTxId);


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
         account->setDescription(QString("%1 AP: %2-%3 (%4)").arg(Obs).arg(ap->getNumber()).arg(pTower->getName()).arg(pDweller->getName()));
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
    QRadProgressWindow *pW = QRadProgressWindow::getInstance();
    pW->setDetail(QString("Criando Taxa Extra..."));

    DwellerList *dlist ;

    pW->hide();
    Editextratx *edt = new Editextratx;
    if( QDialog::Accepted != edt->exec())
    {
        QMessageBox::warning(NULL,
                             QString("Cancelado!"),
                             QString("Operação cancelada!"));
        delete edt;
        return false;
    }
    pW->setDetail(QString("Criando Taxa Extra..."));
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
               pW->hide();
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
                 pW->hide();
                 if( QMessageBox::No == QMessageBox::question(NULL, "Atenção",
                                                              QString("Já foi emitido boleto para %1 este morador neste mês, deseja realmente emitir um novo boleto?").arg(dwdata),
                                                              QMessageBox::Yes|QMessageBox::No,
                                                              QMessageBox::No ))
                     OkList.removeOne(nId);
               }
           }
           if( !OkList.count())
           {
               pW->hide();
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
    pW->hide();
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


    bool bRet = BuildTicket( dlist,
                             date,
                             tpTxCond,
                             pCondTx->getObs(),
                             0,
                             dValue,
                             dDiscount);;

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

        g_tkt->AppendTicket(pDweller, value, ptkt->getVencto(),QString("%1").arg(ptkt->getNossoNumero()),QString("%1").arg(ptkt->getSeuNumero()));
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
                ptkt->updateLoId(ptkt->saveFile(FullFileName));
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
    QString toName = QString("%1Boletos%2.pdf").arg(DEFAULT_REM_DIR).arg(QDate::currentDate().toString("ddMMyyyy"));
    QFile::rename(DEFAULT_PDF_FILE,toName);
    //QDesktopServices::openUrl(QUrl::fromLocalFile(toName));//, QUrl::TolerantMode);

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
        QCoreApplication::processEvents();
         pCurrent = list->at(i);
         if( pCurrent->getTpOp() == tpRegistered)
         {
             ticket *ptkt = ticket::findByNossoNumero(pCurrent->getNossoNumero().toInt(),true);
             if( ptkt && (ptkt->getStatus() != stRegistered))
             {
                 bRet = ptkt->updateStatus(stRegistered);
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
