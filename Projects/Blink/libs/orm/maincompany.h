#ifndef maincompany_H
#define maincompany_H
 
#include "model.h"

DECLARE_MODEL(maincompany)

class maincompany : public Model
{


     MODEL_DECLARE_ATTRIBUTE(int, id);
     MODEL_DECLARE_ATTRIBUTE(QString, name);
     MODEL_DECLARE_ATTRIBUTE(QString, fantasyname);
     MODEL_DECLARE_ATTRIBUTE(QString, cnpj);
     MODEL_DECLARE_ATTRIBUTE(QString, ie);
     MODEL_DECLARE_ATTRIBUTE(QString, phone);
     MODEL_DECLARE_ATTRIBUTE(QString, email);
     MODEL_DECLARE_ATTRIBUTE(QString, license);
     MODEL_DECLARE_ATTRIBUTE(int, city);
     MODEL_DECLARE_ATTRIBUTE(int, state);


     MODEL_MATCH_TABLE(maincompany, "maincompany")

private:
    static maincompanyList* m_allList;
public:
    static maincompanyList* findAll(void);
    static maincompany* findByid(int id);
    static maincompany* findByid(int id, QString database);
};
 
#endif 
