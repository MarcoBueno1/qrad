#ifndef login_H
#define login_H
 
#include "model.h"

DECLARE_MODEL(login)

class login : public Model
{

     MODEL_DECLARE_ATTRIBUTE(int, id);
     MODEL_DECLARE_ATTRIBUTE(int, user);
     MODEL_DECLARE_ATTRIBUTE(QDate, date);
     MODEL_DECLARE_ATTRIBUTE(QTime, time);

     MODEL_MATCH_TABLE(login, "login")

private:
    static loginList* m_allList;
public:
    static loginList* findAll(void);
    static login* findByid(int id);
    static login* findByid(int id, QString database);
};
 
#endif 
