#include "docvalidate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qraddebug.h"
#include <QStringList>


CDoc::CDoc()
{
}

int CDoc::mod11(char *szValue, char tipo)
{
  int soma = 0; // acumulador
  int peso = 2; // peso inicial
  int numdig = strlen(szValue); // número de dígitos
  for (int i = numdig - 1; i >= 0; i--)
  {
    soma = soma + (szValue[i]-0x30) * peso++;
    // se $tipo == 2 o $peso retorna a 2 quando
    // atingir 10
    if (tipo != 1)
        if (peso == 10)
            peso = 2;
  }
  // calcula o resto de $soma dividido por 11
  // subtrai 11 do resultado anterior - este é o dígito
  // se $dígito for 10 ou 11 altera para 0
  int digito = 11 - (soma % 11);

  if (digito > 9) digito = 0;
  return digito;
}

CCPF::CCPF():CDoc()
{
}

bool CCPF::IsValid( QString strCPF )
{
    char szCPF[12];
    QString strDv;

    strCPF = strCPF.remove("-").remove(".").remove("/");


    if( strCPF.trimmed().length() != 11 )
    {
        debug_message("CPF: Tamanho diferente de 11:\n");
        return false;
    }

    strncpy( szCPF, strCPF.toLatin1().data(),9 );

    szCPF[9] = 0;


    char digit = (char) mod11( szCPF, 1);

    szCPF[9] = digit + 0x30;
    szCPF[10] = 0;
    digit  = (char) mod11( szCPF, 1);
    szCPF[10] = digit+0x30;
    szCPF[11] = 0;


    debug_message("CPF: Digito de verificacao: %s\n", &szCPF[9]);
    strDv.sprintf("%s",&szCPF[9]);
    if( strDv == strCPF.mid(9,2))
       return true;
    return false;
}



CCNPJ::CCNPJ():CDoc()
{
}

bool CCNPJ::IsValid( QString strCNPJ )
{
    char szCNPJ[15];
    QString strDv;

    strCNPJ = strCNPJ.remove("-").remove(".").remove("/");


    if( strCNPJ.trimmed().length() != 14 )
    {
        debug_message("CNPJ: Tamanho diferente de 14:\n");
        return false;
    }

    strncpy( szCNPJ, strCNPJ .toLatin1().data(),12 );

    szCNPJ[12] = 0;


    char digit = (char) mod11( szCNPJ, 2);

    szCNPJ[12] = digit + 0x30;
    szCNPJ[13] = 0;
    digit  = (char) mod11( szCNPJ, 2);
    szCNPJ[13] = digit+0x30;
    szCNPJ[14] = 0;


    debug_message("CNPJ: Digito de verificacao: %s\n", &szCNPJ[12]);
    strDv.sprintf("%s",&szCNPJ[12]);
    if( strDv == strCNPJ.mid(12,2))
       return true;
    return false;
}
