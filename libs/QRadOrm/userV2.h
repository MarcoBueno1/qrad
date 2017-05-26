#ifndef User_H
#define User_H

#include "orm.h"

DECLARE_ORM(User)

DECLARE_CLASS(User)
     ORM_DECLARE_ATTRIBUTE(User, int, id);
     ORM_DECLARE_ATTRIBUTE(User, QString, name);
     ORM_DECLARE_ATTRIBUTE(User, QString, email);
     ORM_DECLARE_ATTRIBUTE(User, QDate, contractsince);
     ORM_DECLARE_ATTRIBUTE(User, QString, password);
     ORM_DECLARE_ATTRIBUTE(User, int, profile);
     ORM_DECLARE_ATTRIBUTE(User, int, LoId);
     ORM_DECLARE_ATTRIBUTE(User, bool, removed);

    ORM_MATCH_TABLE(User, "vuser")
ORM_END_MAP()
 
#endif 
