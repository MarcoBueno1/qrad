#ifndef email_H
#define email_H
 
#include "orm.h"

DECLARE_ORM(email)

DECLARE_CLASS(email)
     ORM_DECLARE_ATTRIBUTE(email, int, Id);
     ORM_DECLARE_ATTRIBUTE(email, QString, User);
     ORM_DECLARE_ATTRIBUTE(email, QString, Password);
     ORM_DECLARE_ATTRIBUTE(email, QString, Host);
     ORM_DECLARE_ATTRIBUTE(email, QString, Port);
     ORM_MATCH_TABLE(email, "email")
ORM_END_MAP()
#endif 
