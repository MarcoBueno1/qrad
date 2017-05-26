#ifndef respemis_H
#define respemis_H
 
#include "orm.h"

DECLARE_ORM(respemis)

DECLARE_CLASS(respemis)
     ORM_DECLARE_ATTRIBUTE(respemis, int, id);
     ORM_DECLARE_ATTRIBUTE(respemis, QString, Description);
     ORM_DECLARE_ATTRIBUTE(respemis, int, tp);
     ORM_MATCH_TABLE(respemis, "respemis")
ORM_END_MAP()
#endif 
