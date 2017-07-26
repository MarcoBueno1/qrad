#ifndef STATE_H
#define STATE_H

#include "orm.h"

DECLARE_ORM(State)

DECLARE_CLASS(State)
     ORM_DECLARE_ATTRIBUTE(State, int, id)
     ORM_DECLARE_ATTRIBUTE(State, QString, Name)
     ORM_DECLARE_ATTRIBUTE(State, bool, Removed)
     ORM_MATCH_TABLE(State, "state")
ORM_END_MAP()

#endif // STATE_H
