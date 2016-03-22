#ifndef Ap_H
#define Ap_H
 
#include "model.h"

DECLARE_MODEL(Ap)

class Ap : public Model
{
     MODEL_DECLARE_ATTRIBUTE(int, id);
     MODEL_DECLARE_ATTRIBUTE(QString, Number);
     MODEL_DECLARE_ATTRIBUTE(int, Tower);
     MODEL_DECLARE_ATTRIBUTE(int, m2);

     MODEL_MATCH_TABLE(Ap, "ap")

private:
    static ApList* m_allList;
public:
    static ApList* findAll(void);
    static Ap* findByid(int id);
    static Ap* findByid(int id, QString database);
};
 
#endif 
