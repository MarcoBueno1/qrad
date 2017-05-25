#ifndef AccountTypeModel_H
#define AccountTypeModel_H

#include "orm.h"

typedef enum{
    AccountTypeToPay = 0,
    AccountTypeToReceive,
    AccountTypeBoth
}DSM_AccountType_t;

typedef enum{
    AccountOpCreate = 1,
    AccountOpEdit,
    AccountOpRemove,
    AccountOpPaid,
    AccountOpRestore
}DSM_AccountOperation_t;

DECLARE_ORM(AccountTypeModel)

DECLARE_CLASS(AccountTypeModel)
    ORM_DECLARE_ATTRIBUTE(AccountTypeModel,int, Id)
    ORM_DECLARE_ATTRIBUTE(AccountTypeModel,QString, Description)
    ORM_DECLARE_ATTRIBUTE(AccountTypeModel,int, Type)

    ORM_MATCH_TABLE(AccountTypeModel, "fin_accounttype")
ORM_END_MAP()
 
#endif 
