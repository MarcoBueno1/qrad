#include <QString>
#include <QStringList>


#define ALIGN_LEFT    0
#define ALIGN_CENTER  1
#define ALIGN_RIGHT   2

typedef struct fieldformat
{
  QString Name;
  double Percent;
  int Align;
}FieldFormat, *PFieldFormat;



class pdfwrapper
{
    pdfwrapper();
	~pdfwrapper();
	
 public:	
    static int Build( QString strFile, QString strTitle, QString strColTitle, QStringList body );
};
