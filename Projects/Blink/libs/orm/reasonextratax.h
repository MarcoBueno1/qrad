#ifndef reasonextratax_H
#define reasonextratax_H

#include "orm.h"

DECLARE_ORM(ReasonExtraTax)

DECLARE_CLASS(ReasonExtraTax)
     ORM_DECLARE_ATTRIBUTE(ReasonExtraTax, int, id)
     ORM_DECLARE_ATTRIBUTE(ReasonExtraTax, QString, Description)
//     ORM_DECLARE_ATTRIBUTE(shipper, int, User);
     ORM_MATCH_TABLE(ReasonExtraTax, "reasonextratx")
ORM_END_MAP()
#endif
