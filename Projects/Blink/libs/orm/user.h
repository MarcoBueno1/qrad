#ifndef user_H
#define user_H
 
#include "model.h"

#include <QPixmap>

DECLARE_MODEL(user)

class user : public Model
{


     MODEL_DECLARE_ATTRIBUTE(int, id);
     MODEL_DECLARE_ATTRIBUTE(QString, name);
     MODEL_DECLARE_ATTRIBUTE(QString, email);
     MODEL_DECLARE_ATTRIBUTE(QDate, contractsince);
     MODEL_DECLARE_ATTRIBUTE(QString, password);
     MODEL_DECLARE_ATTRIBUTE(int, profile);
     MODEL_DECLARE_ATTRIBUTE(int, LoId);
     MODEL_DECLARE_ATTRIBUTE(bool, removed);


     MODEL_MATCH_TABLE(user, "user")

private:
    static userList* m_allList;
public:
    static userList* findAll(void);
    static user* findByid(int id);
    static user* findByid(int id, QString database);
    static user* findByPassword(QString email, QString Password);
    bool saveImage(QString path);
    QPixmap getImage();
};
 
#endif 
