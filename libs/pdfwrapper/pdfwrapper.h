#ifndef PDFWRAPPER_H
#define PDFWRAPPER_H


#include <QString>
#include <QStringList>


#define ALIGN_LEFT    0
#define ALIGN_CENTER  1
#define ALIGN_RIGHT   2

typedef struct fieldformat
{
  char Name[128];
  double Percent;
  int Align;
}FieldFormat, *PFieldFormat;



class pdfwrapper
{
    pdfwrapper();
	~pdfwrapper();
	
 public:	
    static int Build( QString strFile, 
	                  QStringList ThreeLinesheader,
	                  QString strTitle, 
					  QList<FieldFormat *> ColHeader, 
                      QList<QStringList *> lines,
                      int dwTitleSize = 14,
                      double dTotal=-1 );

	
};
#endif
