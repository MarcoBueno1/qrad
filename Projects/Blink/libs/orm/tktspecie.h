#ifndef TktSpecie_H
#define TktSpecie_H
 
#include "orm.h"

DECLARE_ORM(TktSpecie)

DECLARE_CLASS(TktSpecie)
     ORM_DECLARE_ATTRIBUTE(TktSpecie, int, id);
     ORM_DECLARE_ATTRIBUTE(TktSpecie, QString, Description);
     ORM_DECLARE_ATTRIBUTE(TktSpecie, int, tp);
     ORM_MATCH_TABLE(TktSpecie, "tktspecie")
ORM_END_MAP()
#endif 
