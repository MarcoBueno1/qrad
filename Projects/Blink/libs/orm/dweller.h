#ifndef Dweller_H
#define Dweller_H
 
#include "model.h"
#include "ap.h"
#include "tower.h"

#include <QPixmap>

DECLARE_MODEL(Dweller)

class Dweller : public Model
{
     MODEL_DECLARE_ATTRIBUTE(int, id)
     MODEL_DECLARE_ATTRIBUTE(QString, Name)
     MODEL_DECLARE_ATTRIBUTE(QString, CPF)
     MODEL_DECLARE_ATTRIBUTE(QString, RG)
     MODEL_DECLARE_ATTRIBUTE(int, Type)
     MODEL_DECLARE_FOREINGKEY(int, ap, Ap, Ap)
     MODEL_DECLARE_FOREINGKEY(int, tower, Tower, Tower)
     MODEL_DECLARE_ATTRIBUTE(QDate, Since)
     MODEL_DECLARE_ATTRIBUTE(QDate, MoveOut)
     MODEL_DECLARE_ATTRIBUTE(int, ImageId)
     MODEL_DECLARE_ATTRIBUTE(int, JobTitle )
     MODEL_DECLARE_ATTRIBUTE(QString, email )
     MODEL_DECLARE_ATTRIBUTE(QString, Obs )
     MODEL_DECLARE_ATTRIBUTE(int, LoId )
     MODEL_DECLARE_ATTRIBUTE(QString,Ramal)
     MODEL_DECLARE_ATTRIBUTE(bool, NotifByEmail)
     MODEL_DECLARE_ATTRIBUTE(int,tp)
     MODEL_DECLARE_ATTRIBUTE(bool, Removed )
     MODEL_DECLARE_ATTRIBUTE(bool, Payer )
     MODEL_DECLARE_ATTRIBUTE(bool, Free )
     MODEL_DECLARE_ATTRIBUTE(bool, MovedOut )

     MODEL_MATCH_TABLE(Dweller, "dweller")

private:
    static DwellerList* m_allList;
public:
    static DwellerList* findAll(void);
    static Dweller* findByid(int id);
    static Dweller* findByid(int id, QString database);
    static Dweller* findByCPF(QString strCPF );
    bool saveImage(QString path);
    QPixmap getImage();
};
 
#endif 
