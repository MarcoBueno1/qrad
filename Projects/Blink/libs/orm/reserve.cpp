#include "reserve.h"


ORM_BEGIN_MAP(reserve)
     ORM_MAP_PRIMARYKEY(id, "id");
     ORM_MAP_FIELD(commona_id, "commona_id");
     ORM_MAP_FIELD(date_start, "date_start");
     ORM_MAP_FIELD(time_start, "time_start");
     ORM_MAP_FIELD(date_end, "date_end");
     ORM_MAP_FIELD(time_end, "time_end");
     ORM_MAP_FIELD(responsable, "responsable");
     ORM_MAP_FIELD(used, "used");
     ORM_MAP_FIELD(obs_before, "obs_before");
     ORM_MAP_FIELD(obs_after, "obs_after");
     ORM_MAP_FIELD(problem, "problem");
     ORM_MAP_FIELD(canceled, "canceled");
ORM_END_MAP()

