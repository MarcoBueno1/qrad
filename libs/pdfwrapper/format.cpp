#include "format.h"
/// Max chars per line for helvetica ( size 9 )

#define FORMAT_LINE_LEN 100

/// 


Format::Format()
{

}

Format::~Format()
{

}


QString Format::FormatLine( QList<FieldFormat> head, QList<QString> fields )
{
   QString tmp;
   QString Line;

   for( int i = 0; i < fields.count(); i++ )
   {
     int FieldMaxLen = (int)((FORMAT_LINE_LEN-fields.count()+1) / 100 * head.at(i).Percent);
	 if( head.at(i).Align == ALIGN_CENTER )
		 tmp.sprintf("%*s", FieldMaxLen / 2 + fields.at(i).length() / 2,fields.at(i).toLatin1().data());
	 else if( head.at(i).Align == ALIGN_LEFT )
		 tmp.sprintf ("%-*s",FieldMaxLen,fields.at(i).toLatin1().data());
	 else
		 tmp.sprintf ("%*s",FieldMaxLen,fields.at(i).toLatin1().data());
     if( Line.length() )
		 Line += " ";
	 Line += tmp;
   }
   return Line;   
}

