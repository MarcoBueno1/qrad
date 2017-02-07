#include "Visitante.h"
#include <QImage>
#include "pgsqlasync.h"

VisitanteList* Visitante::m_allList = NULL;

MODEL_BEGIN_MAP(Visitante)
     MODEL_MAP_PRIMARYKEY(id, "id");
     MODEL_MAP_FIELD(Nome, "Nome");
     MODEL_MAP_FIELD(RG, "rg");
     MODEL_MAP_FIELD(CPF, "cpf");
     MODEL_MAP_FIELD(PreAutorizado, "preautorizado");
     MODEL_MAP_FIELD(MoradorAutorizador, "moradorautorizador");
     MODEL_MAP_FIELD(DataAutorizado, "dataautorizado");
     MODEL_MAP_FIELD(AnunciarChegada, "anunciarchegada");
     MODEL_MAP_FIELD(LoId, "loid" );
     MODEL_MAP_FIELD(tp, "tp" );

MODEL_END_MAP()

QList<Visitante*>* Visitante::findAll()
{
    MODEL_INIT_LIST(Visitante, m_allList);

    QString query = QString("select * from Visitante");

    if (!Visitante::fillModelList(m_allList, query))
        return NULL;

    return m_allList;
}

Visitante* Visitante::findByid(int id)
{
    Visitante *VisitanteM = new Visitante();

    QString query = QString("select * from Visitante where id = %3").arg(id);

    if (!VisitanteM->fillModelFromQuery(query))
    {
        delete VisitanteM;
        return NULL;
    }

    return VisitanteM;
}
Visitante* Visitante::findByid(int id, QString database)
{
    return Visitante::findByPrimaryKey(id, database);
}


bool Visitante::saveImage( QString path )
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

QPixmap Visitante::getImage()
{
  return  Model::getImage(getLoId());
}

QString Visitante::getImage(QString path)
{
  return  Model::getImage(getLoId(), path);
}

bool Visitante::Save()
{
  setDateLastOp(QDate::currentDate());
  setTimeLastOp(QTime::currentTime());
  return Model::Save();
}
