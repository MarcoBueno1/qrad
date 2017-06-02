#include <QCoreApplication>

#include "itauinterface.h"
#include <stdio.h>


void Parse(QString Path)
{
    QList<itauticket*> list;
    if( !ItauInterface::Load(&list,Path))
    {
        printf("\nNenhum dado encontrado no arquivo: %s" ,Path.toLatin1().data());
        return;
    }
    printf("\nEncontrados os seguintes itens");
    for( int i = 0; i < list.count(); i++ )
    {
        itauticket *tkt = list.at(i);
        printf("Carteira...: %s\n", tkt->getCart().toLatin1().data());
        printf("NossoNumero: %s\n", tkt->getNossoNumero().toLatin1().data());
        printf("SeuNumero..: %s\n", tkt->getSeuNumero().toLatin1().data());
        printf("Pagador....: %s\n", tkt->getNomePagador().toLatin1().data());
        printf("Data.Vencto: %s\n", tkt->getdtVencto().toString("dd/MM/yyyy").toLatin1().data());
        printf("Data Pagto.: %s\n", tkt->getdtPagto().toString("dd/MM/yyyy").toLatin1().data());
        printf("Agencia....: %s\n", tkt->getAgencia().toLatin1().data());
        printf("Valor......: %s\n", tkt->getValor().toLatin1().data());
        printf("Valor Pago.: %s\n", tkt->getValorPago().toLatin1().data());
        printf("Pago?......: %s\n", tkt->getpago()?"Sim":"Nao");
    }
}



void PrintMenu()
{
  printf( "\nProgrma de teste itaufrench\n" );
  printf( "\n---------------------\n" );
  printf( "\n1. Parse( francesinha.pdf )\n" );
  printf( "\n10. Close Program\n" );

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int cOption;

    char szPath[512];

    do
      {
        PrintMenu();
        scanf( "%d", &cOption ) ;
        switch( cOption )
        {
           case 1: 
                   printf("Informe o path completo do arquivo .pdf:");
                   scanf("%s", szPath);
                   Parse(QString(szPath));
                   break;
           default:
                   break;
        }

      }while( cOption != 10 );

    return 0;
}
