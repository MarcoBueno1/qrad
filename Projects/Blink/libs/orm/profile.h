#ifndef profile_H
#define profile_H
 
#include "model.h"

DECLARE_MODEL(profile)

class profile : public Model
{


     MODEL_DECLARE_ATTRIBUTE(int, id);
     MODEL_DECLARE_ATTRIBUTE(QString, name);
     MODEL_DECLARE_ATTRIBUTE(bool, usepreaut);
     MODEL_DECLARE_ATTRIBUTE(bool, usevisit);
     MODEL_DECLARE_ATTRIBUTE(bool, usedweller);
     MODEL_DECLARE_ATTRIBUTE(bool, usevisitant);
     MODEL_DECLARE_ATTRIBUTE(bool, useuser);
     MODEL_DECLARE_ATTRIBUTE(bool, useprofile);
     MODEL_DECLARE_ATTRIBUTE(bool, ConfigureEmail);
     MODEL_DECLARE_ATTRIBUTE(bool, GerarRelatorios);
     MODEL_DECLARE_ATTRIBUTE(bool, GeenciarBoletos);
     MODEL_DECLARE_ATTRIBUTE(int, tp);
     MODEL_DECLARE_ATTRIBUTE(bool, removed);


     MODEL_MATCH_TABLE(profile, "profile")

private:
    static profileList* m_allList;
public:
    static profileList* findAll(void);
    static profile* findByid(int id);
    static profile* findByid(int id, QString database);
};
 
#endif 
