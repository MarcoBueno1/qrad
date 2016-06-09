#include "pdfwrapper.h"
#include <QString>
#include <QList>
#include "hpdf.h"

class Format
{
   Format();
   ~Format();
   
 public:
 
   static void FormatColHead( HPDF_Page page, 
                              HPDF_Font def_font,
                              HPDF_REAL pgWidth, 
							  HPDF_REAL pgHeight, 
							  QList<FieldFormat *> head );
   static void FormatLine( HPDF_Page page, 
							 HPDF_Font def_font,
							 HPDF_REAL pgWidth, 
							 HPDF_REAL pgHeight, 
							 QList<FieldFormat *> head,
							 QList<QString> fields );							  
							  
   static QString FormatLine( QList<FieldFormat *> head, QList<QString> fields );
   static QString FormatColHead( QList<FieldFormat *> head );
};
