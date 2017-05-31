#ifndef metreage_H
#define metreage_H
 
#include "orm.h"

DECLARE_ORM(metreage)

DECLARE_CLASS(metreage)
     ORM_DECLARE_ATTRIBUTE(metreage, int, id);
     ORM_DECLARE_ATTRIBUTE(metreage, QString, Descricao);
     ORM_DECLARE_ATTRIBUTE(metreage, double, m2);
     ORM_MATCH_TABLE(metreage, "metreage")
ORM_END_MAP()
#endif 
