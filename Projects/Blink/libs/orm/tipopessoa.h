#ifndef tipopessoa_H
#define tipopessoa_H
 
#include "orm.h"

DECLARE_ORM(tipopessoa)

DECLARE_CLASS(tipopessoa)
     ORM_DECLARE_ATTRIBUTE(tipopessoa, int, id);
     ORM_DECLARE_ATTRIBUTE(tipopessoa, QString, Description);
     ORM_DECLARE_ATTRIBUTE(tipopessoa, int, tp);
     ORM_MATCH_TABLE(tipopessoa, "tipopessoa")
ORM_END_MAP()
#endif 
