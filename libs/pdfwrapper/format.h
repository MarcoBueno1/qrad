#include "pdfwrapper.h"
#include <QString>
#include <QList>

class Format
{
   Format();
   ~Format();
   
 public:
 
   static QString FormatLine( QList<FieldFormat *> head, QList<QString> fields );
   static QString FormatColHead( QList<FieldFormat *> head );
};
