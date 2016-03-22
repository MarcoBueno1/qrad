#include "address.h"

AddressList* Address::m_allList = NULL;

MODEL_BEGIN_MAP(Address)
     MODEL_MAP_PRIMARYKEY(id, "id");
	 MODEL_MAP_FIELD(Street, "street");
	 MODEL_MAP_FIELD(Number, "number");
	 MODEL_MAP_FIELD(cep, "cep");
	 MODEL_MAP_FIELD(Neighborhood, "neighborhood" );

	 MODEL_END_MAP()

QList<Address*>* Address::findAll()
{
    MODEL_INIT_LIST(Address, m_allList);

    QString query = QString("select * from address");

    if (!Address::fillModelList(m_allList, query))
        return NULL;

    return m_allList;
}

Address* Address::findByid(int id)
{
    Address *AddressM = new Address();

    QString query = QString("select * from address where id = %3").arg(id);

    if (!AddressM->fillModelFromQuery(query))
    {
        delete AddressM;
        return NULL;
    }

    return AddressM;
}
Address* Address::findByid(int id, QString database)
{
    return Address::findByPrimaryKey(id, database);
}
