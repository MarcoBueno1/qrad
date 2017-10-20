#ifndef reserve_H
#define reserve_H
 
#include "orm.h"

DECLARE_ORM(reserve)

DECLARE_CLASS(reserve)
     ORM_DECLARE_ATTRIBUTE(reserve, int, id)
     ORM_DECLARE_ATTRIBUTE(reserve, int, commona_id)
     ORM_DECLARE_ATTRIBUTE(reserve, QDate, date_start)
     ORM_DECLARE_ATTRIBUTE(reserve, QTime, time_start)
     ORM_DECLARE_ATTRIBUTE(reserve, QDate, date_end)
     ORM_DECLARE_ATTRIBUTE(reserve, QTime, time_end)
     ORM_DECLARE_ATTRIBUTE(reserve, int, responsable)
     ORM_DECLARE_ATTRIBUTE(reserve, bool, used)
     ORM_DECLARE_ATTRIBUTE(reserve, QString, obs_before)
     ORM_DECLARE_ATTRIBUTE(reserve, QString, obs_after)
     ORM_DECLARE_ATTRIBUTE(reserve, bool, problem)
     ORM_DECLARE_ATTRIBUTE(reserve, bool, canceled)
     ORM_MATCH_TABLE(reserve, "reserve")
ORM_END_MAP()
#endif 
