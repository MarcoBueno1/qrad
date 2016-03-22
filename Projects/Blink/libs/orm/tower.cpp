#include "tower.h"

TowerList* Tower::m_allList = NULL;

MODEL_BEGIN_MAP(Tower)
     MODEL_MAP_PRIMARYKEY(id, "id");
	 MODEL_MAP_FIELD(Name, "name");

	 MODEL_END_MAP()

QList<Tower*>* Tower::findAll()
{
    MODEL_INIT_LIST(Tower, m_allList);

    QString query = QString("select * from tower");

    if (!Tower::fillModelList(m_allList, query))
        return NULL;

    return m_allList;
}

Tower* Tower::findByid(int id)
{
    Tower *TowerM = new Tower();

    QString query = QString("select * from tower where id = %3").arg(id);

    if (!TowerM->fillModelFromQuery(query))
    {
        delete TowerM;
        return NULL;
    }

    return TowerM;
}
Tower* Tower::findByid(int id, QString database)
{
    return Tower::findByPrimaryKey(id, database);
}
