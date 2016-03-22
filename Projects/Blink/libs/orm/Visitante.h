#ifndef Visitante_H
#define Visitante_H
 
#include "model.h"

#include <QPixmap>

DECLARE_MODEL(Visitante)

class Visitante : public Model
{

     MODEL_DECLARE_ATTRIBUTE(int, id);
     MODEL_DECLARE_ATTRIBUTE(QString, Nome);
     MODEL_DECLARE_ATTRIBUTE(QString, RG);
     MODEL_DECLARE_ATTRIBUTE(QString, CPF);
     MODEL_DECLARE_ATTRIBUTE(bool, PreAutorizado);
     MODEL_DECLARE_ATTRIBUTE(int,  MoradorAutorizador);
     MODEL_DECLARE_ATTRIBUTE(QDate, DataAutorizado);
     MODEL_DECLARE_ATTRIBUTE(bool, AnunciarChegada);
     MODEL_DECLARE_ATTRIBUTE(int, ImagemId);

     MODEL_MATCH_TABLE(Visitante, "Visitante")

private:
    static VisitanteList* m_allList;
public:
    static VisitanteList* findAll(void);
    static Visitante* findByid(int id);
    static Visitante* findByid(int id, QString database);
    static bool saveImage(QString path);
    static QPixmap getImage();
};
 
#endif 
