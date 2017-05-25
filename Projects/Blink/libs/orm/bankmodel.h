#ifndef BankModel_H
#define BankModel_H

#include "orm.h"

DECLARE_ORM(BankModel)

DECLARE_CLASS(BankModel)
    ORM_DECLARE_ATTRIBUTE(BankModel, int, Id)
    ORM_DECLARE_ATTRIBUTE(BankModel, QString, Description)
    ORM_DECLARE_ATTRIBUTE(BankModel,QString, Code)

    ORM_MATCH_TABLE(BankModel, "fin_bank")
ORM_END_MAP()
 
#endif 
