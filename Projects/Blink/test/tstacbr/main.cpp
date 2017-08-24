#include <QCoreApplication>

#include "buildtkt.h"
#include "ticket.h"
#include "qradshared.h"
#include "bankticket.h"
#include "qradfile.h"
#include "accounttoreceivemodel.h"
#include "accounttoreceivehistorymodel.h"

BuildTkt *g_tkt;



void Init()
{
   MainCompany *pCompany    = MainCompany::findByPrimaryKey(1);
   ticketconfig *pTktConfig = ticketconfig::findByPrimaryKey(1);
   bankaccount *pAccount    = bankaccount::findByPrimaryKey(1);
   BankModel *pBank         = BankModel::findByPrimaryKey(pAccount->getBanco());

   if( g_tkt->Init(pCompany,pTktConfig,pBank, pAccount ))
   {
       printf( "\nInit Exectado com sucesso!\n");
   }
   else
   {
       printf( "\nFalha no Init\n!");

   }
}


void AppendTicket()
{
    Dweller *pDweller = Dweller::findByPrimaryKey(284); // 8

    bool bRet = g_tkt->AppendTicket(pDweller, "650.00", QDate::currentDate().addDays(15), "14", "4");
    if( bRet )
    {
//        delete pDweller;
        pDweller = Dweller::findByPrimaryKey(9);
        bRet = g_tkt->AppendTicket(pDweller, "651.00", QDate::currentDate().addDays(10), "15", "5");
        if( bRet )
        {
//            delete pDweller;
            pDweller = Dweller::findByPrimaryKey(10);
            bRet = g_tkt->AppendTicket(pDweller, "652.00", QDate::currentDate().addDays(5), "16", "6");
        }

    }
    if( bRet )
    {
        printf("\nAppendTicket executado com sucesso!\n");
    }
    else
    {
        printf("\nFalha no AppendTicket!\n");

    }
}

void AppendEngecoDivida()
{
    Dweller *pDweller = Dweller::findByPrimaryKey(587); // Drogaria Nazaré
// 05/12/2012
// R$ 574,88
    int i;
    printf("Primeiro loop\n");
    QDate date = QDate(2012,12,5);
    for( i = 0; date <= QDate(2013,8,5); i++ )
    {

        g_tkt->AppendTicket(pDweller,"574,88", date,QString("%1").arg(20158353+10+i),QString("04"),QString("TAXA CONDOMINIAL 521,06 + FUNDO DE RESERVA 53,82 \nTOTAL 574,88"), "", date.addDays(-5));
        date = date.addMonths(1);
        //printf("no loop\n");
    }
    printf("Segundo loop\n");
    date =  QDate(2013,9,5);
    for( ; date <= QDate(2014,12,5); i++ )
    {
        g_tkt->AppendTicket(pDweller,"522,62", date,QString("%1").arg(20158353+10+i),QString("04"),"TAXA CONDOMINIAL", "", date.addDays(-5));
        date = date.addMonths(1);
    }
    printf("Terceiro loop\n");
    date =  QDate(2015,1,5);
    for( ; date <= QDate(2017,5,5); i++ )
    {
        g_tkt->AppendTicket(pDweller,"616,69", date,QString("%1").arg(20158353+10+i),"703 G","TAXA CONDOMINIAL",QString("5"),date.addDays(-5));
        date = date.addMonths(1);
    }
    printf("Quarto loop\n");
    date =  QDate(2013,11,28);
    g_tkt->AppendTicket(pDweller,"133,34", date,QString("%1").arg(20158353+10+i),"04","TAXA EXTRA",QString(""),date.addDays(-5));
    date =  QDate(2013,12,30);
    g_tkt->AppendTicket(pDweller,"133,34", date,QString("%1").arg(20158353+10+i),"04","TAXA EXTRA",QString(""),date.addDays(-5));
    date =  QDate(2014,1,20);
    g_tkt->AppendTicket(pDweller,"133,34", date,QString("%1").arg(20158353+10+i),"04","TAXA EXTRA",QString(""),date.addDays(-5));
    date =  QDate(2014,2,28);
    g_tkt->AppendTicket(pDweller,"133,34", date,QString("%1").arg(20158353+10+i),"04","TAXA EXTRA",QString(""),date.addDays(-5));
    date =  QDate(2014,3,27);
    g_tkt->AppendTicket(pDweller,"93,44", date,QString("%1").arg(20158353+10+i),"04","TAXA EXTRA",QString(""),date.addDays(-5));
    date =  QDate(2014,4,28);
    g_tkt->AppendTicket(pDweller,"38,65", date,QString("%1").arg(20158353+10+i),"04","TAXA EXTRA",QString(""),date.addDays(-5));
    date =  QDate(2015,8,5);
    g_tkt->AppendTicket(pDweller,"139,35", date,QString("%1").arg(20158353+10+i),"04","TAXA EXTRA",QString(""),date.addDays(-5));
    date =  QDate(2017,3,15);
    g_tkt->AppendTicket(pDweller,"150,00", date,QString("%1").arg(20158353+10+i),"04","TAXA EXTRA",QString(""),date.addDays(-5));
    date =  QDate(2017,4,15);
    g_tkt->AppendTicket(pDweller,"150,00", date,QString("%1").arg(20158353+10+i),"04","TAXA EXTRA",QString(""),date.addDays(-5));
    date =  QDate(2017,7,15);
    g_tkt->AppendTicket(pDweller,"150,00", date,QString("%1").arg(20158353+10+i),"04","TAXA EXTRA",QString(""),date.addDays(-5));

}


void AppendTicketFromTicketsDb()
{
    ticketList *tickets = ticket::findBy("select * from ticket where status = 1 or status = 3 order by clientid");
    for( int i = 0; tickets && (i < tickets->count()); i++ )
    {
        ticket *pCurrent= tickets->at(i);
        Dweller *pdw = Dweller::findByid(pCurrent->getclientid());
        if( pdw )
        {
           g_tkt->AppendTicket(pdw,
                               QString("%1").arg(pCurrent->getValor()).replace(".",","),
                               pCurrent->getVencto(),
                               QString("%1").arg(pCurrent->getNossoNumero()).replace(".",","),
                               QString("%1").arg(pCurrent->getSeuNumero()).replace(".",","));
        }
    }
}


void AdjustMissingAccounts()
{
    ticketList *pTickets = ticket::findBy("select * from ticket where accountid = 0");

    if( pTickets )
    {
        for( int i = 0; i < pTickets->count();i++)
        {
            ticket *tkt = pTickets->at(i);
            Dweller *pDweller = Dweller::findByid(tkt->getclientid());
            Ap   *ap = Ap::findByid(pDweller->getap());
            Tower *pTower = Tower::findByid(pDweller->gettower());

            AccountToReceiveModel *account  = new  AccountToReceiveModel;

            account->setClientId(pDweller->getid());
            account->setDescription(QString("%1 AP: %2-%3 (%4)").arg(tkt->getObs()).arg(ap->getNumber()).arg(pTower->getName()).arg(pDweller->getName()));
            account->setIssueDate(tkt->getVencto().addDays(-5));
            account->setVencDate(tkt->getVencto());
            account->setValue( tkt->getValor());
            account->setAccountTypeId(2); // tipo condominio
            if( account->Save())
            {
//                debug_message("account->getId()=%d\n", account->getId());
                tkt->updateAccountId(account->getId());
//                    debug_message("nao consegui fazer updateAccountId=%d\n", account->getId());
            }

            ///
            /// History
            AccountToReceiveHistoryModel *accountToReceiveHistoryModel = new AccountToReceiveHistoryModel;

            accountToReceiveHistoryModel->setAccountToReceiveId(account->getId());
            accountToReceiveHistoryModel->setTypeOperation(AccountOpCreate);
            accountToReceiveHistoryModel->setUserId(tkt->getUser());
            accountToReceiveHistoryModel->setDate(QDate::currentDate());
            accountToReceiveHistoryModel->setTime(QTime::currentTime());

            accountToReceiveHistoryModel->Save();

            delete accountToReceiveHistoryModel;

            ///

            delete account;
            delete ap;
            delete pTower;
            delete pDweller;
        }
    }
}



void AddTickets()
{
    if( g_tkt->AddTickets() )
    {
        printf("\nAddTickets executado com sucesso!\n");
    }
    else
    {
        printf("\nFalha no AddTickets!\n");
    }
}

void BuildShipping()
{
    if( g_tkt->BuildShipping("c:\\dvl\\","remessa.rem"))
    {
        printf("\nBuildShipping executado com Sucesso!\n");
    }
    else
    {
        printf("\nProblema no BuildShipping!\n");
    }
}
#define TYPE_ONLY_PAID 0
#define TYPE_ONLY_REGISTERED 1
#define TYPE_ALL 2

void ExtractReturn(int nType, QString Vencto)
{
    char szFileName[200];
    char szPath[200];
    char szDataVencto[200];
    QList<BankTicket *> tickets;

    printf("\nData de Vencimento( vazio para não filtrar):");
    scanf("%s", szDataVencto);

    Vencto = QString(szDataVencto);
    if(Vencto == "0")
        Vencto = "";



    printf("\nDigite o caminho do arquivo (com \\ no final):");
    scanf("%s", szPath);

    printf("\nDigite o nome do arquivo:");
    scanf("%s", szFileName);
    if( g_tkt->ExtractReturn(&tickets, QString(szPath),QString(szFileName)))
    {
        printf("\nExtractReturn executado com Sucesso!\n");
        for( int i =0; i < tickets.count();i++)
        {
            BankTicket *pTkt =  tickets.at(i);
            if(( TYPE_ONLY_PAID == nType ) && ( pTkt->getTpOp()!=tpLiquidated))
            {
              continue;
            }
            if(( TYPE_ONLY_REGISTERED == nType ) && ( pTkt->getTpOp()!=tpRegistered))
            {
              continue;
            }
            if( !Vencto.isEmpty() && (pTkt->getdtVencto().toString(FMT_DATE) != Vencto))
            {
              continue;
            }
            printf("%5d %10s %10s %6s %8s %6s %8s %s\n"
                   ,i+1,
                   pTkt->getNossoNumero().toLatin1().data(),
                   pTkt->getSeuNumero().toLatin1().data(),
                   pTkt->getValor().toLatin1().data(),
                   pTkt->getdtVencto().toString(FMT_DATE).toLatin1().data(),
                   pTkt->getValorPago().toLatin1().data(),
                   pTkt->getdtPagto().toString(FMT_DATE).toLatin1().data(),
                   pTkt->getTpOp()==tpLiquidated?"Baixa":pTkt->getTpOp()==tpRegistered?"Registro":"Outro");
            //printf( "-----------------------------------------------------------");
            //printf( "Item.....: %d\n", i+1);
            //printf( "BnkNumber: %s\n", pTkt->getNossoNumero().toLatin1().data());
            //printf( "SysNumber: %s\n", pTkt->getSeuNumero().toLatin1().data());
            //printf( "Value....: %s\n", pTkt->getValor().toLatin1().data());
            //printf( "Vencto...: %s\n", pTkt->getdtVencto().toString(FMT_DATE).toLatin1().data());
            //printf( "Paid.....: %s\n", pTkt->getValorPago().toLatin1().data());
            //printf( "Dt Paid..: %s\n", pTkt->getdtPagto().toString(FMT_DATE).toLatin1().data());
            //printf( "Event....: %s\n", pTkt->getTpOp()==tpLiquidated?"Baixa":pTkt->getTpOp()==tpRegistered?"Registro":"Outro");
        }
    }
    else
    {
        printf("\nProblema no ExtractReturn!\n");
    }
}


void configureDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase(QRadFile::GetParamValue("banco/database"));
///    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(QRadFile::GetParamValue("banco/databasename"));
    db.setHostName(QRadFile::GetParamValue("banco/hostname"));
    db.setUserName(QRadFile::GetParamValue("banco/username"));
    db.setPassword(QRadFile::GetParamValue("banco/password"));

    if(!db.open())
    {
        qDebug() <<"Impossivel conectar no banco";
    }
    else
    {
        qDebug() << "Conexão Ok";
    }

}

void AppendAgenc()
{
    char  szBuffer[200];

    bankaccount *Ba = new bankaccount;

    printf("\nAgencia:") ;
    scanf("%s", szBuffer);
    Ba->setAgencia(szBuffer);

    printf("\nDigito Agencia:") ;
    scanf("%s", szBuffer);
    Ba->setDigitoAgencia(szBuffer);

    printf("\nConta:") ;
    scanf("%s", szBuffer);
    Ba->setConta(szBuffer);

    printf("\nDigito Conta:") ;
    scanf("%s", szBuffer);
    Ba->setDigitoConta(szBuffer);

    printf("\nCodigo Banco:") ;
    scanf("%s", szBuffer);


    BankModelList *Bm = BankModel::findByCode(szBuffer);
    if( !Bm )
    {
        delete Ba;
        printf( "\nO banco informado não foi encontrado!\n");
        return;
    }

    Ba->setBanco(Bm->at(0)->getId());

    if(Ba->Save())
    {
        printf("\nDados Salvos!");
    }
    else
    {
        printf("\nProblema ao salvar dados!");

    }
    delete Ba;
    delete Bm;
}

void AppendBanco()
{
    char  szBuffer[200];

    BankModel *Ba = new BankModel;

    printf("\nCod.Banco:") ;
    scanf("%s", szBuffer);
    Ba->setCode(szBuffer);

    printf("\nNome.Banco:") ;
    scanf("%s", szBuffer);

    Ba->setDescription(szBuffer);
    if(Ba->Save())
    {
       printf("\nDados Salvos!");
    }
    else
    {
       printf("\nProblema ao salvar dados!");
    }
    delete Ba;
}

void AppendTkt()
{

    ticketconfig *tk = new ticketconfig ;

    tk->setCarteira(0);;
    tk->setcnab(2);;
    tk->setCodigoCedente("0001");;
    tk->setJuros("1.50");
    tk->setLayoutBol(2);
    tk->setMensagem("Pagavel em qualquer banco ate o vencimento!");
    tk->setNossoNumero(001);
    tk->setRespEmis(0);

    if(tk->Save())
    {
       printf("\nDados Salvos!");
    }
    else
    {
       printf("\nProblema ao salvar dados!");
    }
    delete tk;
}
void AddCompany()
{
   City *city = new  City ;
   State *state = new State ;

   city->setName("Manaus");
   city->Save();

   state->setSign("AM");
   state->Save();

   MainCompany * pCompany = new MainCompany;

   pCompany->setAddrees("Av. Grande Otelo");
   pCompany->setCEP("69055-021");
   pCompany->setcity(city->getid());
   pCompany->setstate(state->getid());
   pCompany->setcnpj("091.009.0005/90");
   pCompany->setComplement("Garden Club");
   pCompany->setfantasyname("Residencial Garden Club");
   pCompany->setHouseNumber("270");
   pCompany->setNeighborhood("Parque 10");
   pCompany->Save();

}


void PrintMenu()
{
  printf( "\nProgrma de teste acbr\n" );
  printf( "\n---------------------\n" );
  printf( "\n1. Init\n" );
  printf( "\n2. AppendTicket( append 3 tkts..)\n" );
  printf( "\n3. AddTickets( add to acbr )\n" );
  printf( "\n4. BuildShipping\n" );
  printf( "\n5. Print\n" );

//  printf( "\n6. DB_AppendAgenc\n" );
//  printf( "\n7. DB_AppendBanco\n" );
//  printf( "\n8. DB_AppendTkt\n" );
//  printf( "\n9. DB_AppendCompany\n" );
  printf( "\n10. ExtractReturn()\n" );
  printf( "\n11. ExtractReturn(ONLY_PAID)\n" );
  printf( "\n12. ExtractReturn(ONLY_REGISTERED)\n" );
  printf( "\n13. AppendTicketFromTicketsDb\n");
  printf( "\n14. AdjustMissingAccounts()\n");
  printf( "\n15. AppendEngecoDivida()\n");
  printf( "\n16. Close Program\n" );

/*

       bool Init(MainCompany *pCompany, ticketconfig *pTktConfig, BankModel *pBank, bankaccount *pAccount);
       bool Print(bool bPrinter=false, QString strPath=TKT_DEFAULT_PDF_PATH );
       bool BuildShipping(QString strDir, int nFileNumber, QString FileName );
       bool AddTicket(Dweller *pDweller, QString strValue, QDate dtVencto);
*/


}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    configureDatabase();

    g_tkt =  new BuildTkt ;
    int cOption;


    do
      {
        PrintMenu();
        scanf( "%d", &cOption ) ;
        switch( cOption )
        {
           case 1: 
                   Init();
                   break;
           case 2:
                   AppendTicket();
                   break;
           case 3:
                   AddTickets();
                   break;
           case 4:
                   BuildShipping();
                   break;
           case 5:
                   if(g_tkt->Print(false))
                   {
                       printf( "\nPrint executado com sucesso\n");
                   }
                   else
                   {
                       printf( "\nErro na execucao do Print\n");
                   }
                   break;
           case 6:
                  AppendAgenc();
                  break;
           case 7:
                  AppendBanco();
                  break;
           case 8:
                  AppendTkt();
                  break;
           case 9:
                  AddCompany();
                  break;
           case 10:
                  ExtractReturn(TYPE_ALL, "");
                  break;
           case 11:
                  ExtractReturn(TYPE_ONLY_PAID, "");
                  break;
           case 12:
                   ExtractReturn(TYPE_ONLY_REGISTERED, "");
                   break;
           case 13:
                  AppendTicketFromTicketsDb();
                  break;
           case 14:
                  AdjustMissingAccounts();
                  break;
           case 15:
                  AppendEngecoDivida();
                  break;

           default:
                   break;
        }

      }while( cOption != 16 );

    qDebug() << "antes delete g_tkt";
    delete g_tkt;
    qDebug() << "depois delete g_tkt";

//    exit(0);
    //return a.exec();
    return 0;
}
