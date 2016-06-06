#include <QString>
#include <QStringList>

class pdfwrapper
{
    pdfwrapper();
	~pdfwrapper();
	
 public:	
    static int Build( QString strFile, QString strTitle, QString strColTitle, QStringList body );
};
