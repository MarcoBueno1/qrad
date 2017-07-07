#ifndef visit_H
#define visit_H
 
#include "model.h"

DECLARE_MODEL(visit)

class visit : public Model
{
     MODEL_DECLARE_ATTRIBUTE(int, id);
     MODEL_DECLARE_ATTRIBUTE(QDate, Data);
     MODEL_DECLARE_ATTRIBUTE(QTime, Hora);
     MODEL_DECLARE_ATTRIBUTE(int, Visitante);
     MODEL_DECLARE_ATTRIBUTE(int, Reason);
     MODEL_DECLARE_ATTRIBUTE(QDate, Saida);
     MODEL_DECLARE_ATTRIBUTE(QTime, saida_hora);
     MODEL_DECLARE_ATTRIBUTE(int, autorizador);
     MODEL_DECLARE_ATTRIBUTE(bool, Notified);

     MODEL_MATCH_TABLE(visit, "visit")

private:
    static visitList* m_allList;
public:
    static visitList* findAll(void);
    static visit* findByid(int id);
    static visit* findByid(int id, QString database);
    static visitList * findNotNotified();
};
 
#endif 
