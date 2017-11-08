#ifndef Invited_H
#define Invited_H
 
#include "orm.h"

DECLARE_ORM(Invited)

DECLARE_CLASS(Invited)
     ORM_DECLARE_ATTRIBUTE(Invited, int, Id)
     ORM_DECLARE_ATTRIBUTE(Invited, int, Reserve)
     ORM_DECLARE_ATTRIBUTE(Invited, QString, Name)
     ORM_DECLARE_ATTRIBUTE(Invited, QString, Documment)
     ORM_DECLARE_ATTRIBUTE(Invited, bool, Removed)
     ORM_MATCH_TABLE(Invited, "invited")
ORM_END_MAP()
#endif 
