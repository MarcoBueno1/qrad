#ifndef PHONETYPE_H
#define PHONETYPE_H

#include "orm.h"

DECLARE_ORM(PhoneType)

DECLARE_CLASS(PhoneType)
     ORM_DECLARE_ATTRIBUTE(PhoneType, int, id)
     ORM_DECLARE_ATTRIBUTE(PhoneType, QString, Type)
     ORM_DECLARE_ATTRIBUTE(PhoneType, bool, Removed)
     ORM_MATCH_TABLE(PhoneType, "phonetype")
ORM_END_MAP()
#endif // PHONETYPE_H
