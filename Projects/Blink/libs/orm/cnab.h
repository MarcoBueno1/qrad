#ifndef cnab_H
#define cnab_H
 
#include "orm.h"

DECLARE_ORM(cnab)

DECLARE_CLASS(cnab)
     ORM_DECLARE_ATTRIBUTE(cnab, int, id);
     ORM_DECLARE_ATTRIBUTE(cnab, QString, Description);
     ORM_DECLARE_ATTRIBUTE(cnab, int, tp);
     ORM_MATCH_TABLE(cnab, "cnab")
ORM_END_MAP()
#endif 
