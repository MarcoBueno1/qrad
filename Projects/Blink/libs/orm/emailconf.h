#ifndef emailconf_H
#define emailconf_H
 
#include "model.h"

DECLARE_MODEL(emailconf)

class emailconf : public Model
{


     MODEL_DECLARE_ATTRIBUTE(int, id);
     MODEL_DECLARE_ATTRIBUTE(QString, username);
     MODEL_DECLARE_ATTRIBUTE(QString, password);
     MODEL_DECLARE_ATTRIBUTE(QString, server);
     MODEL_DECLARE_ATTRIBUTE(QString, port);


     MODEL_MATCH_TABLE(emailconf, "emailconf")

private:
    static emailconfList* m_allList;
public:
    static emailconfList* findAll(void);
    static emailconf* findByid(int id);
    static emailconf* findByid(int id, QString database);
};
 
#endif 
