#ifndef preaut_H
#define preaut_H
 
#include "model.h"

DECLARE_MODEL(preaut)

class preaut : public Model
{


     MODEL_DECLARE_ATTRIBUTE(int, id);
     MODEL_DECLARE_ATTRIBUTE(bool, sunday);
     MODEL_DECLARE_ATTRIBUTE(bool, monday);
     MODEL_DECLARE_ATTRIBUTE(bool, tuesday);
     MODEL_DECLARE_ATTRIBUTE(bool, wednesday);
     MODEL_DECLARE_ATTRIBUTE(bool, thursday);
     MODEL_DECLARE_ATTRIBUTE(bool, friday);
     MODEL_DECLARE_ATTRIBUTE(bool, saturday);
     MODEL_DECLARE_ATTRIBUTE(int, visit);
     MODEL_DECLARE_ATTRIBUTE(QTime, horaini);
     MODEL_DECLARE_ATTRIBUTE(QTime, horafim);
     MODEL_DECLARE_ATTRIBUTE(QDate, validate);
     MODEL_DECLARE_ATTRIBUTE(int, authorizer);
     MODEL_DECLARE_ATTRIBUTE(QString, obs);
     MODEL_DECLARE_ATTRIBUTE(QDate, autsince);
     MODEL_DECLARE_ATTRIBUTE(int, reason);
     MODEL_DECLARE_ATTRIBUTE(int, destination);
     MODEL_DECLARE_ATTRIBUTE(int, tp);
     MODEL_DECLARE_ATTRIBUTE(bool, NoAnnounce);
     MODEL_DECLARE_ATTRIBUTE(bool, removed);


     MODEL_MATCH_TABLE(preaut, "preaut")

private:
    static preautList* m_allList;
public:
    static preautList* findAll(void);
    static preaut* findByid(int id);
    static preaut* findByid(int id, QString database);
    static preaut* findValidByVisitor( int visitor );

};
 
#endif 
