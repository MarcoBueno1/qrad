#ifndef Debt_H
#define Debt_H

#include "orm.h"
#include "paymentwaymodel.h"

DECLARE_ORM(Debt)

DECLARE_CLASS(Debt)
    ORM_DECLARE_ATTRIBUTE(Debt,int, Id);
    ORM_DECLARE_ATTRIBUTE(Debt,int, SaleId);
    ORM_DECLARE_FOREINGKEY(Debt,int, PaymentWayId, PaymentWay, PaymentWay);
    ORM_DECLARE_ATTRIBUTE(Debt,int, CreditId);
    ORM_DECLARE_ATTRIBUTE(Debt,QDate, PaymentDate);
    ORM_DECLARE_ATTRIBUTE(Debt, double,  Value);
    ORM_DECLARE_ATTRIBUTE(Debt,bool, Paid);
    ORM_DECLARE_ATTRIBUTE(Debt,int, Portion);
    ORM_DECLARE_ATTRIBUTE(Debt,int, NumPortions);
    ORM_DECLARE_ATTRIBUTE(Debt,int, Status);
    ORM_DECLARE_ATTRIBUTE(Debt, QString, Description);
    ORM_DECLARE_ATTRIBUTE(Debt, int, PromotionId);
    ORM_DECLARE_ATTRIBUTE(Debt, double, Payback);
    ORM_DECLARE_ATTRIBUTE(Debt,bool, BoletoEmitted);
    ORM_DECLARE_ATTRIBUTE(Debt, double, HandTaken);

    ORM_MATCH_TABLE(Debt, "debt")
ORM_END_MAP()
 
#endif 
