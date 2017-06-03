#ifndef supplier_H
#define supplier_H
 
#include "orm.h"

DECLARE_ORM(supplier)

DECLARE_CLASS(supplier)
     ORM_DECLARE_ATTRIBUTE(supplier, int, id);
     ORM_DECLARE_ATTRIBUTE(supplier, QString, CNPJ);
     ORM_DECLARE_ATTRIBUTE(supplier, QString, Nome);
     ORM_DECLARE_ATTRIBUTE(supplier, QString, Fantasia);
     ORM_DECLARE_ATTRIBUTE(supplier, QDate, Data);
     ORM_DECLARE_ATTRIBUTE(supplier, QTime, Hora);
     ORM_DECLARE_ATTRIBUTE(supplier, bool, Removed);
     ORM_MATCH_TABLE(supplier, "supplier")
ORM_END_MAP()
#endif 
