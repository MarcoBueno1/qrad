#ifndef AccountToPayModel_H
#define AccountToPayModel_H

#include "orm.h"
#include "accounttypemodel.h"

DECLARE_ORM(AccountToPayModel)

DECLARE_CLASS(AccountToPayModel)
    ORM_DECLARE_ATTRIBUTE(AccountToPayModel,int, Id)
    ORM_DECLARE_ATTRIBUTE(AccountToPayModel,QDate, IssueDate)
    ORM_DECLARE_ATTRIBUTE(AccountToPayModel,QDate, VencDate)
    ORM_DECLARE_ATTRIBUTE(AccountToPayModel,QDate, PaidDate)
    ORM_DECLARE_ATTRIBUTE(AccountToPayModel,QString, Obs)
    ORM_DECLARE_ATTRIBUTE(AccountToPayModel,double, Value)
    ORM_DECLARE_ATTRIBUTE(AccountToPayModel,double, ValuePaid)
    ORM_DECLARE_FOREINGKEY(AccountToPayModel,int, AccountTypeId, AccountTypeModel, AccountTypeModel);
    ORM_DECLARE_ATTRIBUTE(AccountToPayModel,QString, Description)
    ORM_DECLARE_ATTRIBUTE(AccountToPayModel,int, SupplierId)
    ORM_DECLARE_ATTRIBUTE(AccountToPayModel,bool, Paid)
    ORM_DECLARE_ATTRIBUTE(AccountToPayModel,int, BankId)
    ORM_DECLARE_ATTRIBUTE(AccountToPayModel,bool, Removed)
    ORM_DECLARE_ATTRIBUTE(AccountToPayModel,QString, DocNumber)
   
    ORM_MATCH_TABLE(AccountToPayModel, "fin_accounttopaymodel")
ORM_END_MAP()

#endif 
