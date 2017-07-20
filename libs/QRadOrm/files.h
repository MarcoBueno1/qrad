#ifndef FILES_H
#define FILES_H

#include "filedescription.h"

DECLARE_ORM(Files)

DECLARE_CLASS(Files)
     ORM_DECLARE_ATTRIBUTE(Files, int, id)
     ORM_DECLARE_ATTRIBUTE(Files, QDate, Created)
     ORM_DECLARE_ATTRIBUTE(Files, QString, Name)
     ORM_DECLARE_ATTRIBUTE(Files, QString, Description)
     ORM_DECLARE_ATTRIBUTE(Files, QDate, LastAccess)
     ORM_DECLARE_FOREINGKEY(Files, int, TypeId, FileDescription, FileDescription)
     ORM_DECLARE_ATTRIBUTE(Files, int, LoId)
     ORM_DECLARE_ATTRIBUTE(Files, bool, Removed)
     ORM_MATCH_TABLE(Files, "files")
public:
     int saveFile(QString file);
ORM_END_MAP()

#endif // FILES_H
