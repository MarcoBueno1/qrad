#include "format.h"
/// Max chars per line for helvetica ( size 9 )

#define FORMAT_LINE_LEN          100
#define FORMAT_MAX_LINE_PER_PAGE  39

/// 


Format::Format()
{

}

Format::~Format()
{

}

QString Format::FormatColHead( QList<FieldFormat *> head )
{
   QString tmp;
   QString Line;

   
   for( int i = 0; i < head.count(); i++ )
   {
     int FieldMaxLen = (int)((FORMAT_LINE_LEN-head.count()+1) / 100 * head.at(i)->Percent);
	 if( head.at(i)->Align == ALIGN_CENTER )
         tmp.sprintf("%*s", FieldMaxLen / 2 + strlen(head.at(i)->Name) / 2,head.at(i)->Name);
	 else if( head.at(i)->Align == ALIGN_LEFT )
         tmp.sprintf ("%-*s",FieldMaxLen,head.at(i)->Name);
	 else
         tmp.sprintf ("%*s",FieldMaxLen,head.at(i)->Name);
     if( Line.length() )
		 Line += " ";
	 Line += tmp;
   }
   return Line;
}

QString Format::FormatLine( QList<FieldFormat *> head, QList<QString> fields )
{
   QString tmp;
   QString Line;

   for( int i = 0; i < fields.count(); i++ )
   {
     int FieldMaxLen = (int)((FORMAT_LINE_LEN-fields.count()+1) / 100 * head.at(i)->Percent);
	 if( head.at(i)->Align == ALIGN_CENTER )
		 tmp.sprintf("%*s", FieldMaxLen / 2 + fields.at(i).length() / 2,fields.at(i).toLatin1().data());
	 else if( head.at(i)->Align == ALIGN_LEFT )
		 tmp.sprintf ("%-*s",FieldMaxLen,fields.at(i).toLatin1().data());
	 else
		 tmp.sprintf ("%*s",FieldMaxLen,fields.at(i).toLatin1().data());
     if( Line.length() )
		 Line += " ";
	 Line += tmp;
   }
   return Line;   
}

