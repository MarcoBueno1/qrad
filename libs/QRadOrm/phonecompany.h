#ifndef PHONECOMPANY_H
#define PHONECOMPANY_H


#include "orm.h"

DECLARE_ORM(PhoneCompany)

DECLARE_CLASS(PhoneCompany)
     ORM_DECLARE_ATTRIBUTE(PhoneCompany, int, id)
     ORM_DECLARE_ATTRIBUTE(PhoneCompany, QString, Name)
     ORM_DECLARE_ATTRIBUTE(PhoneCompany, bool, Removed)
     ORM_MATCH_TABLE(PhoneCompany, "operadora")
ORM_END_MAP()

#endif // PHONECOMPANY_H
