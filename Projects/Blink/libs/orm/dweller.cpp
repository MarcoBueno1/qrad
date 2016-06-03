#include "dweller.h"

DwellerList* Dweller::m_allList = NULL;

MODEL_BEGIN_MAP(Dweller)
     MODEL_MAP_PRIMARYKEY(id, "id");
	 
     MODEL_MAP_FIELD(Name, "Name");
     MODEL_MAP_FIELD(CPF, "cpf");
     MODEL_MAP_FIELD(RG, "rg");
     MODEL_MAP_FIELD(Type, "type");
     MODEL_MAP_FIELD(Ap, "ap");
     MODEL_MAP_FIELD(Tower, "tower");
     MODEL_MAP_FIELD(Since, "since");
     MODEL_MAP_FIELD(MoveOut, "moveout");
     MODEL_MAP_FIELD(ImageId, "imageid" );
     MODEL_MAP_FIELD(JobTitle, "jobtitle");
     MODEL_MAP_FIELD(email, "email");
     MODEL_MAP_FIELD(Obs, "obs");
     MODEL_MAP_FIELD(LoId, "loid");
     MODEL_MAP_FIELD(Ramal, "ramal");
     MODEL_MAP_FIELD(Removed, "removed");

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

Dweller* Dweller::findByCPF(QString strCPF )
{
    Dweller *DwellerM = new Dweller();

    QString query = QString("select * from dweller where cpf = '%1'").arg(strCPF);

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

bool Dweller::saveImage( QString path )
{
  int nLoId  = Model::saveImage( path );

  if( nLoId )
  {
    setLoId( nLoId );
    updateLoId( nLoId );
    return true;
  }

  return false;
}

QPixmap Dweller::getImage()
{
  return  Model::getImage(getLoId());

}
