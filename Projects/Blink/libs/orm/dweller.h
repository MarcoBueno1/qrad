#ifndef Dweller_H
#define Dweller_H
 
#include "model.h"

#include <QPixmap>

DECLARE_MODEL(Dweller)

class Dweller : public Model
{
     MODEL_DECLARE_ATTRIBUTE(int, id);
     MODEL_DECLARE_ATTRIBUTE(QString, Name);
     MODEL_DECLARE_ATTRIBUTE(int, Type);
     MODEL_DECLARE_ATTRIBUTE(QDate, Since);
     MODEL_DECLARE_ATTRIBUTE(QDate, MoveOut);
     MODEL_DECLARE_ATTRIBUTE(int, ImageId);
	 MODEL_DECLARE_ATTRIBUTE(int, JobTitle );
	 MODEL_DECLARE_ATTRIBUTE(QString, email );
	 MODEL_DECLARE_ATTRIBUTE(QString, Obs );
	 
     MODEL_MATCH_TABLE(Dweller, "dweller")

private:
    static DwellerList* m_allList;
public:
    static DwellerList* findAll(void);
    static Dweller* findByid(int id);
    static Dweller* findByid(int id, QString database);
    static bool saveImage(QString path);
    static QPixmap getImage();
};
 
#endif 
