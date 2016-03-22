#ifndef Preaut_H
#define Preaut_H
 
#include "model.h"

DECLARE_MODEL(Preaut)

class Preaut : public Model
{
     MODEL_DECLARE_ATTRIBUTE(int, id);
     MODEL_DECLARE_ATTRIBUTE(int, Visit);
     MODEL_DECLARE_ATTRIBUTE(int, Reason);
     MODEL_DECLARE_ATTRIBUTE(QDate, AutSince);
     MODEL_DECLARE_ATTRIBUTE(QDate, Validate);
     MODEL_DECLARE_ATTRIBUTE(QTime, EnterTime);
     MODEL_DECLARE_ATTRIBUTE(bool, Sunday);
     MODEL_DECLARE_ATTRIBUTE(bool, Tuesday);
     MODEL_DECLARE_ATTRIBUTE(bool, Wednesday);
     MODEL_DECLARE_ATTRIBUTE(bool, Thursday);
     MODEL_DECLARE_ATTRIBUTE(bool, Friday);
     MODEL_DECLARE_ATTRIBUTE(bool, Saturday);

     MODEL_MATCH_TABLE(Preaut, "preaut")

private:
    static PreautList* m_allList;
public:
    static PreautList* findAll(void);
    static Preaut* findByid(int id);
    static Preaut* findByid(int id, QString database);
};
 
#endif 
