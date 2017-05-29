#ifndef State_H
#define State_H
 
#include "orm.h"

DECLARE_ORM(State)

DECLARE_CLASS(State)
     ORM_DECLARE_ATTRIBUTE(State, int, id)
     ORM_DECLARE_ATTRIBUTE(State, QString, Name)
     ORM_DECLARE_ATTRIBUTE(State, QString, Sign)
     ORM_MATCH_TABLE(State, "state")
ORM_END_MAP()
#endif 
