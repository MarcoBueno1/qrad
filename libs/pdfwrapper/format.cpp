#include "format.h"
#include <stdio.h>
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


void Format::FormatColHead( HPDF_Page page, 
                            HPDF_Font def_font,
                            HPDF_REAL pgWidth, 
							HPDF_REAL pgHeight, 
							QList<FieldFormat *> head )
{
   HPDF_REAL tw; 
   HPDF_REAL CurrPos=0;
 
   for( int i = 0; i < head.count(); i++ )
   {
     double dPart1 = pgWidth-(head.count()+1);
     double dPart2 = dPart1 /100 ;
     double FieldMaxLen = dPart2 * head.at(i)->Percent;
	 
	 /// Passo 1:  Assegurar que nao passa do tamanho maximo
	 tw = HPDF_Page_TextWidth(page, head.at(i)->Name);   
	 while( tw > FieldMaxLen )
	 {
		 int len = strlen(head.at(i)->Name);
		 if( len )
			 head.at(i)->Name[len-1] = 0;
	     tw = HPDF_Page_TextWidth(page, head.at(i)->Name);   
	 }
	 //
	 
     HPDF_Page_BeginText(page);
	 if( head.at(i)->Align == ALIGN_CENTER )
           HPDF_Page_MoveTextPos(page, 50 + CurrPos + (/*pgWidth*/FieldMaxLen - tw) / 2, pgHeight);
	 else if( head.at(i)->Align == ALIGN_LEFT )
           HPDF_Page_MoveTextPos(page, 50 + CurrPos, pgHeight);
	 else
 	 {
		 printf( "ALIGN_RIGHT: Nao implementado\n");
	 }
     HPDF_Page_SetFontAndSize(page, def_font, 13);
     HPDF_Page_ShowText(page, head.at(i)->Name);
	 HPDF_Page_EndText(page);
	 CurrPos += FieldMaxLen + 1;
   }
}

QString Format::FormatColHead( QList<FieldFormat *> head )
{
   QString tmp;
   QString Line;

   
   for( int i = 0; i < head.count(); i++ )
   {
     double dPart1 = FORMAT_LINE_LEN-(head.count()+1);
     double dPart2 = dPart1 /100 ;
     double dPart3 = dPart2 * head.at(i)->Percent;

 //    double dValue = ((FORMAT_LINE_LEN-(head.count()+1)) / 100 * head.at(i)->Percent);
     int FieldMaxLen = (int)dPart3; //((FORMAT_LINE_LEN-(head.count()+1)) / 100 * head.at(i)->Percent);
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


void Format::FormatLine( HPDF_Page page, 
                         HPDF_Font def_font,
                         HPDF_REAL pgWidth, 
						 HPDF_REAL pgHeight, 
						 QList<FieldFormat *> head,
                         QList<QString> fields )
{
   HPDF_REAL tw; 
   HPDF_REAL CurrPos=0;
   HPDF_REAL PosHi = pgHeight;
   char szText[1024];
 
   for( int i = 0; i < head.count(); i++ )
   {
     double dPart1 = pgWidth-(head.count()+1);
     double dPart2 = dPart1 /100 ;
     double FieldMaxLen = dPart2 * head.at(i)->Percent;
	 
	 /// Passo 1:  Assegurar que nao passa do tamanho maximo
	 fields.at(i);
	 strcpy(szText, fields.at(i).toLatin1().data());
	 
	 tw = HPDF_Page_TextWidth(page, szText);   
	 while( tw > FieldMaxLen )
	 {
		 int len = strlen(szText);
		 if( len )
			 szText[len-1] = 0;
	     tw = HPDF_Page_TextWidth(page, szText);   
	 }
	 //
	 
     HPDF_Page_BeginText(page);
	 if( head.at(i)->Align == ALIGN_CENTER )
           HPDF_Page_MoveTextPos(page, 50 + CurrPos + (/*pgWidth*/FieldMaxLen - tw) / 2, PosHi);
	 else if( head.at(i)->Align == ALIGN_LEFT )
           HPDF_Page_MoveTextPos(page, 50 + CurrPos, PosHi);
	 else
 	 {
		 printf( "ALIGN_RIGHT: Nao implementado\n");
	 }
     HPDF_Page_SetFontAndSize(page, def_font, 9);
     HPDF_Page_ShowText(page, szText);
	 HPDF_Page_EndText(page);
	 CurrPos += FieldMaxLen + 1;
   }
  
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

