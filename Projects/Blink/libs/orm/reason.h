#ifndef reason_H
#define reason_H
 
#include "model.h"

DECLARE_MODEL(reason)

class reason : public Model
{


     MODEL_DECLARE_ATTRIBUTE(int, id);
     MODEL_DECLARE_ATTRIBUTE(QString, description);
     MODEL_DECLARE_ATTRIBUTE(int, tp);
     MODEL_DECLARE_ATTRIBUTE(bool, removed);


     MODEL_MATCH_TABLE(reason, "reason")

private:
    static reasonList* m_allList;
public:
    static reasonList* findAll(void);
    static reason* findByid(int id);
    static reason* findByid(int id, QString database);
};
 
#endif 
