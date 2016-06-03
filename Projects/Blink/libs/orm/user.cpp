#include "user.h"

userList* user::m_allList = NULL;

MODEL_BEGIN_MAP(user)
     MODEL_MAP_PRIMARYKEY(id, "id");
     MODEL_MAP_FIELD(name, "name");
     MODEL_MAP_FIELD(email, "email");
     MODEL_MAP_FIELD(contractsince, "contractsince");
     MODEL_MAP_FIELD(password, "password");
     MODEL_MAP_FIELD(profile, "profile");
     MODEL_MAP_FIELD(removed, "removed");
MODEL_END_MAP()

QList<user*>* user::findAll()
{
    MODEL_INIT_LIST(user, m_allList);

    QString query = QString("select * from user");

    if (!user::fillModelList(m_allList, query))
        return NULL;

    return m_allList;
}

user* user::findByid(int id)
{
    user *userM = new user();

    QString query = QString("select * from user where id = %3").arg(id);

    if (!userM->fillModelFromQuery(query))
    {
        delete userM;
        return NULL;
    }

    return userM;
}
user* user::findByid(int id, QString database)
{
    return user::findByPrimaryKey(id, database);
}
