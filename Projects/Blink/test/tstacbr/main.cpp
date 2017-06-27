#include <QCoreApplication>

#include "buildtkt.h"
#include "ticket.h"
#include "qradshared.h"

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
void ExtractReturn()
{
    QList<Ticket *> tickets;
    if( g_tkt->ExtractReturn(&tickets,"c:\\dvl\\","remessa.ret"))
    {
        printf("\nExtractReturn executado com Sucesso!\n");
        for( int i =0; i < tickets.count();i++)
        {
            Ticket *pTkt =  tickets.at(i);
            printf( "Item.....: %d\n", i+1);
            printf( "BnkNumber: %s\n", pTkt->getNossoNumero().toLatin1().data());
            printf( "SysNumber: %s\n", pTkt->getSeuNumero().toLatin1().data());
            printf( "Value....: %s\n", pTkt->getValue().toLatin1().data());
            printf( "Vencto...: %s\n", pTkt->getDate().toString(FMT_DATE).toLatin1().data());
            printf( "Paid.....: %s\n", pTkt->getPaidValue().toLatin1().data());
            printf( "Dt Paid..: %s\n", pTkt->getDatePagto().toString(FMT_DATE).toLatin1().data());
            printf( "Event....: %s\n", pTkt->getTpOp()==tpLiquidated?"Baixa":pTkt->getTpOp()==tpRegistered?"Registro":"Outro");
        }
    }
    else
    {
        printf("\nProblema no ExtractReturn!\n");
    }

}


void configureDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL7");
///    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("qraddb");
    db.setHostName("localhost");
    db.setPassword("dsmpassword");
    db.setUserName("dsm");

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
  printf( "\n10. ExtractReturn(\n" );
  printf( "\n11. AppendTicketFromTicketsDb\n");
  printf( "\n12. Close Program\n" );

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
                   if(g_tkt->Print())
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
                  ExtractReturn();
                  break;
           case 11:
                  AppendTicketFromTicketsDb();
                  break;

           default:
                   break;
        }

      }while( cOption != 12 );

    qDebug() << "antes delete g_tkt";
    delete g_tkt;
    qDebug() << "depois delete g_tkt";

//    exit(0);
    //return a.exec();
    return 0;
}
