#include <QCoreApplication>

#include "buildtkt.h"
#include "ticket.h"
#include "qradshared.h"
#include "bankticket.h"
#include "qradfile.h"

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
  printf( "\n14. Close Program\n" );

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

           default:
                   break;
        }

      }while( cOption != 14 );

    qDebug() << "antes delete g_tkt";
    delete g_tkt;
    qDebug() << "depois delete g_tkt";

//    exit(0);
    //return a.exec();
    return 0;
}
