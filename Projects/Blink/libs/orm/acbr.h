#ifndef acbr_H
#define acbr_H
 
#include "orm.h"

DECLARE_ORM(Acbr)

DECLARE_CLASS(Acbr)
     ORM_DECLARE_ATTRIBUTE(Acbr, int, id)
     ORM_DECLARE_ATTRIBUTE(Acbr, QString, SendPath)
     ORM_DECLARE_ATTRIBUTE(Acbr, QString, ReceivePath)
     ORM_DECLARE_ATTRIBUTE(Acbr, QString, SendName)
     ORM_DECLARE_ATTRIBUTE(Acbr, QString, ReceiveName)
     ORM_MATCH_TABLE(Acbr, "acbr")
ORM_END_MAP()
#endif 
