#include <QCoreApplication>

#include "buildtkt.h"

BuildTkt *g_tkt;

void PrintMenu()
{
  printf( "\nProgrma de teste acbr\n" );
  printf( "\n---------------------\n" );
  printf( "\n1. Init\n" );
  printf( "\n2. Print\n" );
  printf( "\n3. AddTicket\n" );
  printf( "\n4. BuildShipping\n" );

/*

       bool Init(MainCompany *pCompany, ticketconfig *pTktConfig, BankModel *pBank, bankaccount *pAccount);
       bool Print(bool bPrinter=false, QString strPath=TKT_DEFAULT_PDF_PATH );
       bool BuildShipping(QString strDir, int nFileNumber, QString FileName );
       bool AddTicket(Dweller *pDweller, QString strValue, QDate dtVencto);
*/


}

void Init()
{
   MainCompany *pCompany    = MainCompany::findByPrimaryKey(1);
   ticketconfig *pTktConfig = ticketconfig::findByPrimaryKey(1);
   BankModel *pBank         = BankModel::findByPrimaryKey(1);
   bankaccount *pAccount    = bankaccount::findByPrimaryKey(1);

   if( g_tkt->Init(pCompany,pTktConfig,pBank, pAccount ))
   {
       printf( "\nInit Exectado com sucesso!\n");
   }
   else
   {
       printf( "\nFalha no Init\n!");

   }
}

void AddTicket()
{
    Dweller *pDweller = Dweller::findByPrimaryKey(1);

    bool bRet = g_tkt->AddTicket(pDweller, "10.00", QDate::currentDate().addDays(15));
    if( bRet )
    {
        delete pDweller;
        pDweller = Dweller::findByPrimaryKey(2);
        bRet = g_tkt->AddTicket(pDweller, "11.50", QDate::currentDate().addDays(10));
        if( bRet )
        {
            delete pDweller;
            pDweller = Dweller::findByPrimaryKey(3);
            bRet = g_tkt->AddTicket(pDweller, "15.50", QDate::currentDate().addDays(5));
        }
    }
    if( bRet )
    {
        printf("\nAddTicket executado com sucesso!\n");
    }
    else
    {
        printf("\nFalha no AddTicket!\n");

    }
}

void BuildShipping()
{
    if( g_tkt->BuildShipping("/media/sf_Dvl/acbr/","remessa.rem"))
    {
        printf("\nBuildShipping executado com Sucesso!\n");
    }
    else
    {
        printf("\nProblema no BuildShipping!\n");
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
                   if(g_tkt->Print())
                   {
                       printf( "\nPrint executado com sucesso\n");
                   }
                   else
                   {
                       printf( "\nErro na execucao do Print\n");
                   }
                   break;
           case 3: 
                   AddTicket();
                   break;
           case 4:
                   BuildShipping();
                   break;
           default:
                   break;
        }

      }while( cOption != 0x35 );

    delete g_tkt;
    exit(0);
    //return a.exec();
}
