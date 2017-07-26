#ifndef CEP_H
#define CEP_H

#include "orm.h"

DECLARE_ORM(Cep)

DECLARE_CLASS(Cep)
     ORM_DECLARE_ATTRIBUTE(Cep, int, Id)
     ORM_DECLARE_ATTRIBUTE(Cep, QString, Number)
     ORM_DECLARE_ATTRIBUTE(Cep, bool, Removed)
     ORM_MATCH_TABLE(Cep, "cep")
ORM_END_MAP()

#endif // CEP_H
