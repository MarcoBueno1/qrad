#ifndef ticketconfig_H
#define ticketconfig_H
 
#include "orm.h"

DECLARE_ORM(ticketconfig)

DECLARE_CLASS(ticketconfig)
     ORM_DECLARE_ATTRIBUTE(ticketconfig, int, id)
     ORM_DECLARE_ATTRIBUTE(ticketconfig, int, RespEmis)
     ORM_DECLARE_ATTRIBUTE(ticketconfig, int, TipoPessoa)
     ORM_DECLARE_ATTRIBUTE(ticketconfig, QString, CodigoCedente)
     ORM_DECLARE_ATTRIBUTE(ticketconfig, int, LayoutBol)
     ORM_DECLARE_ATTRIBUTE(ticketconfig, int, TipoCobranca)
     ORM_DECLARE_ATTRIBUTE(ticketconfig, int, cnab)
     ORM_DECLARE_ATTRIBUTE(ticketconfig, QString, NossoNumero)
     ORM_DECLARE_ATTRIBUTE(ticketconfig, QString, Carteira )
     ORM_DECLARE_ATTRIBUTE(ticketconfig, QString, Mensagem )
     ORM_DECLARE_ATTRIBUTE(ticketconfig, QString, Juros )
     ORM_DECLARE_ATTRIBUTE(ticketconfig, double, Custo )

     ORM_MATCH_TABLE(ticketconfig, "ticketconfig")
ORM_END_MAP()
#endif 
