#ifndef layoutbol_H
#define layoutbol_H
 
#include "orm.h"

DECLARE_ORM(layoutbol)

DECLARE_CLASS(layoutbol)
     ORM_DECLARE_ATTRIBUTE(layoutbol, int, id);
     ORM_DECLARE_ATTRIBUTE(layoutbol, QString, Description);
     ORM_DECLARE_ATTRIBUTE(layoutbol, int, tp);
     ORM_MATCH_TABLE(layoutbol, "layoutbol")
ORM_END_MAP()
#endif 
