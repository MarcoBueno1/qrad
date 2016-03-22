#include "Dweller.h"
#include <QImage>

DwellerList* Dweller::m_allList = NULL;

MODEL_BEGIN_MAP(Dweller)
     MODEL_MAP_PRIMARYKEY(id, "id");
	 
     MODEL_MAP_FIELD(Name, "Name");
     MODEL_MAP_FIELD(Type, "type");
     MODEL_MAP_FIELD(Since, "since");
	 MODEL_MAP_FIELD(MoveOut, "moveout");
     MODEL_MAP_FIELD(ImageId, "imageid" );
	 MODEL_MAP_FIELD(JobTitle, "jobtitle");
	 MODEL_MAP_FIELD(email, "email");
	 MODEL_MAP_FIELD(Obs, "obs");
	 
MODEL_END_MAP()

QList<Dweller*>* Dweller::findAll()
{
    MODEL_INIT_LIST(Dweller, m_allList);

    QString query = QString("select * from dweller");

    if (!Dweller::fillModelList(m_allList, query))
        return NULL;

    return m_allList;
}

Dweller* Dweller::findByid(int id)
{
    Dweller *DwellerM = new Dweller();

    QString query = QString("select * from dweller where id = %3").arg(id);

    if (!DwellerM->fillModelFromQuery(query))
    {
        delete DwellerM;
        return NULL;
    }

    return DwellerM;
}
Dweller* Dweller::findByid(int id, QString database)
{
    return Dweller::findByPrimaryKey(id, database);
}

bool Dweller::saveImage( QString path)
{

 return true;
}

QPixmap Dweller::getImage()
{
    QString strFoto;

    ///
    /// \brief codigo para ler a imagem do banco de dados
    ///

    QImage myImage;
    myImage.load(strFoto);

    return QPixmap::fromImage(myImage);
}
