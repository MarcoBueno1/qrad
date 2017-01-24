#include "Brand.h"

BrandList* Brand::m_allList = NULL;

MODEL_BEGIN_MAP(Brand)
     MODEL_MAP_PRIMARYKEY(id, "id");
     MODEL_MAP_FIELD(Name, "Name");
     MODEL_MAP_FIELD(tp, "tp");
     MODEL_MAP_FIELD(Removed, "removed");
MODEL_END_MAP()

QList<Brand*>* Brand::findAll()
{
    MODEL_INIT_LIST(Brand, m_allList);

    QString query = QString("select * from Brand");

    if (!Brand::fillModelList(m_allList, query))
        return NULL;

    return m_allList;
}

Brand* Brand::findByid(int id)
{
    Brand *BrandM = new Brand();

    QString query = QString("select * from Brand where id = %3").arg(id);

    if (!BrandM->fillModelFromQuery(query))
    {
        delete BrandM;
        return NULL;
    }

    return BrandM;
}
Brand* Brand::findByid(int id, QString database)
{
    return Brand::findByPrimaryKey(id, database);
}
