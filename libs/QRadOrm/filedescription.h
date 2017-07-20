#ifndef FILEDESCRIPTION_H
#define FILEDESCRIPTION_H

#include "orm.h"

DECLARE_ORM(FileDescription)

DECLARE_CLASS(FileDescription)
     ORM_DECLARE_ATTRIBUTE(FileDescription, int, id)
     ORM_DECLARE_ATTRIBUTE(FileDescription, QString, Description)
     ORM_DECLARE_ATTRIBUTE(FileDescription, int, Tp)
     ORM_DECLARE_ATTRIBUTE(FileDescription, bool, Removed)
     ORM_MATCH_TABLE(FileDescription, "filedescription")
ORM_END_MAP()

#endif // FILEDESCRIPTION_H
