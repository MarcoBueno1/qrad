#ifndef FILEASSOCIATIONTYPE_H
#define FILEASSOCIATIONTYPE_H

#include "orm.h"

DECLARE_ORM(FileAssociationType)

DECLARE_CLASS(FileAssociationType)
     ORM_DECLARE_ATTRIBUTE(FileAssociationType, int, id)
     ORM_DECLARE_ATTRIBUTE(FileAssociationType, QString, Description)
     ORM_DECLARE_ATTRIBUTE(FileAssociationType, bool, Removed)
     ORM_MATCH_TABLE(FileAssociationType, "fileassociationtype")
ORM_END_MAP()

#endif // FILEASSOCIATIONTYPE_H
