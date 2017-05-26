#ifndef tipocobranca_H
#define tipocobranca_H
 
#include "orm.h"

DECLARE_ORM(tipocobranca)

DECLARE_CLASS(tipocobranca)
     ORM_DECLARE_ATTRIBUTE(tipocobranca, int, id);
     ORM_DECLARE_ATTRIBUTE(tipocobranca, QString, Description);
     ORM_DECLARE_ATTRIBUTE(tipocobranca, int, tp);
     ORM_MATCH_TABLE(tipocobranca, "tipocobranca")
ORM_END_MAP()
#endif 
