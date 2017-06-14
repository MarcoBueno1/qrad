#ifndef emailconf_H
#define emailconf_H
 
#include "orm.h"

DECLARE_ORM(emailconf)

DECLARE_CLASS(emailconf)
     ORM_DECLARE_ATTRIBUTE(emailconf, int, Id);
     ORM_DECLARE_ATTRIBUTE(emailconf, QString, User);
     ORM_DECLARE_ATTRIBUTE(emailconf, QString, Password);
     ORM_DECLARE_ATTRIBUTE(emailconf, QString, Host);
     ORM_DECLARE_ATTRIBUTE(emailconf, QString, Port);
     ORM_MATCH_TABLE(email, "emailconf")
ORM_END_MAP()
#endif 
