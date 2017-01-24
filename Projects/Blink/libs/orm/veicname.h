#ifndef veicname_H
#define veicname_H
 
#include "model.h"

DECLARE_MODEL(veicname)

class veicname : public Model
{


     MODEL_DECLARE_ATTRIBUTE(int, id);
     MODEL_DECLARE_ATTRIBUTE(QString, type);
     MODEL_DECLARE_ATTRIBUTE(int, tp);
     MODEL_DECLARE_ATTRIBUTE(bool, Removed);


     MODEL_MATCH_TABLE(veicname, "veicname")

private:
    static veicnameList* m_allList;
public:
    static veicnameList* findAll(void);
    static veicname* findByid(int id);
    static veicname* findByid(int id, QString database);
};
 
#endif 
