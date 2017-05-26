#ifndef bankaccount_H
#define bankaccount_H
 
#include "orm.h"

DECLARE_ORM(bankaccount)

DECLARE_CLASS(bankaccount)
     ORM_DECLARE_ATTRIBUTE(bankaccount, int, id);
     ORM_DECLARE_ATTRIBUTE(bankaccount, QString, Conta);
     ORM_DECLARE_ATTRIBUTE(bankaccount, QString, DigitoConta);
     ORM_DECLARE_ATTRIBUTE(bankaccount, QString, Agencia);
     ORM_DECLARE_ATTRIBUTE(bankaccount, QString, DigitoAgencia);
     ORM_DECLARE_ATTRIBUTE(bankaccount, int, Banco);
     ORM_MATCH_TABLE(bankaccount, "fin_bankaccount")
ORM_END_MAP()
#endif 
