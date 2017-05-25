#ifndef AccountTypeModel_H
#define AccountTypeModel_H

#include "orm.h"

DECLARE_ORM(AccountTypeModel)

DECLARE_CLASS(AccountTypeModel)
    ORM_DECLARE_ATTRIBUTE(AccountTypeModel,int, Id)
    ORM_DECLARE_ATTRIBUTE(AccountTypeModel,QString, Description)
    ORM_DECLARE_ATTRIBUTE(AccountTypeModel,int, Type)

    ORM_MATCH_TABLE(AccountTypeModel, "fin_accounttype")
ORM_END_MAP()
 
#endif 
