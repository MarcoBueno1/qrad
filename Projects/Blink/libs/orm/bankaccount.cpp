#include "bankaccount.h"


ORM_BEGIN_MAP(bankaccount)
     ORM_MAP_PRIMARYKEY(id, "id");
     ORM_MAP_FIELD(Conta, "conta");
     ORM_MAP_FIELD(DigitoConta, "digconta");
     ORM_MAP_FIELD(Agencia, "agencia");
     ORM_MAP_FIELD(DigitoAgencia, "digagencia");
     ORM_MAP_FIELD(Banco, "fin_bank");
ORM_END_MAP()

