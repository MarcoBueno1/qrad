#ifndef extratx_H
#define extratx_H
 
#include "orm.h"

DECLARE_ORM(extratx)

DECLARE_CLASS(extratx)
     ORM_DECLARE_ATTRIBUTE(extratx, int, Id)
     ORM_DECLARE_ATTRIBUTE(extratx, double, Value)
     ORM_DECLARE_ATTRIBUTE(extratx, int, Times)
     ORM_DECLARE_ATTRIBUTE(extratx, int, Motivo)
     ORM_DECLARE_ATTRIBUTE(extratx, QDate, Data)
     ORM_DECLARE_ATTRIBUTE(extratx, bool, All)
     ORM_DECLARE_ATTRIBUTE(extratx, int, Dweller)
     ORM_DECLARE_ATTRIBUTE(extratx, QString, Obs)
     ORM_DECLARE_ATTRIBUTE(extratx, double, PercentAdv)
     ORM_DECLARE_ATTRIBUTE(extratx, double, ValueAdv)
     ORM_MATCH_TABLE(extratx, "extratx")
ORM_END_MAP()
#endif 
