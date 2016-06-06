#include "profile.h"

profileList* profile::m_allList = NULL;

MODEL_BEGIN_MAP(profile)
     MODEL_MAP_PRIMARYKEY(id, "id");
     MODEL_MAP_FIELD(name, "name");
     MODEL_MAP_FIELD(usepreaut, "usepreaut");
     MODEL_MAP_FIELD(usevisit, "usevisit");
     MODEL_MAP_FIELD(usedweller, "usedweller");
     MODEL_MAP_FIELD(usevisitant, "usevisitant");
     MODEL_MAP_FIELD(useuser, "useuser");
     MODEL_MAP_FIELD(useprofile, "useprofile");
     MODEL_MAP_FIELD(tp, "tp");
     MODEL_MAP_FIELD(removed, "removed");
MODEL_END_MAP()

QList<profile*>* profile::findAll()
{
    MODEL_INIT_LIST(profile, m_allList);

    QString query = QString("select * from profile");

    if (!profile::fillModelList(m_allList, query))
        return NULL;

    return m_allList;
}

profile* profile::findByid(int id)
{
    profile *profileM = new profile();

    QString query = QString("select * from profile where id = %3").arg(id);

    if (!profileM->fillModelFromQuery(query))
    {
        delete profileM;
        return NULL;
    }

    return profileM;
}
profile* profile::findByid(int id, QString database)
{
    return profile::findByPrimaryKey(id, database);
}
