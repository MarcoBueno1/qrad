#include "files.h"

ORM_BEGIN_MAP(Files)
     ORM_MAP_PRIMARYKEY(id, "id");
     ORM_MAP_FIELD(Created, "created");
     ORM_MAP_FIELD(Name, "name");
     ORM_MAP_FIELD(Description, "description");
     ORM_MAP_FIELD(LastAccess, "lastaccess" );
     ORM_MAP_FIELD(TypeId, "typeid" );
     ORM_MAP_FIELD(LoId, "loid" );
     ORM_MAP_FIELD(AssociationTypeId, "associationtypeid" );
     ORM_MAP_FIELD(AssociationId, "associationid" );
     ORM_MAP_FIELD(Removed, "removed");
ORM_END_MAP()

int Files::saveFile(QString file)
{
    return ORM::saveFile(file);
}
void Files::getFile(QString file, int nLoId)
{
    ORM::getFile(file,nLoId);
}
