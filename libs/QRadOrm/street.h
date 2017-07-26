#ifndef STREET_H
#define STREET_H

#include "orm.h"

DECLARE_ORM(Street)

DECLARE_CLASS(Street)
     ORM_DECLARE_ATTRIBUTE(Street, int, id)
     ORM_DECLARE_ATTRIBUTE(Street, QString, Name)
     ORM_DECLARE_ATTRIBUTE(Street, bool, Removed)
     ORM_MATCH_TABLE(Street, "street")
ORM_END_MAP()
#endif // STREET_H
