#ifndef PaymentWay_H
#define PaymentWay_H

#include "orm.h"

DECLARE_ORM(PaymentWay)

DECLARE_CLASS(PaymentWay)
    ORM_DECLARE_ATTRIBUTE(PaymentWay,int, Id);
    ORM_DECLARE_ATTRIBUTE(PaymentWay,QString, Description);
    ORM_DECLARE_ATTRIBUTE(PaymentWay,int, CreditSystemId);
    ORM_DECLARE_ATTRIBUTE(PaymentWay,bool, PaybackEnabled);
    ORM_DECLARE_ATTRIBUTE(PaymentWay,bool, Visible);

    ORM_MATCH_TABLE(PaymentWay, "paymentway")
ORM_END_MAP()
 
#endif 
