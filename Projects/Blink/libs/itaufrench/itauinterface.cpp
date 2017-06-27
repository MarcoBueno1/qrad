#include "itauinterface.h"
#include "pdf2txt.h"

#include <QFile>
#include <QProcess>
#include "itauparser.h"


#ifdef Q_OS_WIN
  #define P2T_FILENAME "p2t.exe"
#else
  #define P2T_FILENAME "pdftotext"
#endif


#ifdef Q_OS_WIN
#define BLINK_SAVE_EXE \
    QFile *file = new QFile(P2T_FILENAME);\
    file->open(QIODevice::WriteOnly);\
    file->write((char *)pdftotext_exe,sizeof(pdftotext_exe));\
    file->flush();\
    file->close();\
    delete file;
#else
  #define BLINK_SAVE_EXE
#endif

#ifdef Q_OS_WIN
#define BLINK_REMOVE_EXE \
    QFile::remove(P2T_FILENAME);
#else
#define BLINK_REMOVE_EXE
#endif

ItauInterface::ItauInterface(QString Path)
{
    m_filenme = Path;
}

bool ItauInterface::Parse(QList<BankTicket*> *tikets)
{
    return Parse( tikets, m_filenme);
}

bool ItauInterface::Parse(QList<BankTicket*> *tikets,QString Path)
{

    BLINK_SAVE_EXE
    
    QProcess d;
    d.start(QString("%1 -layout %2 ").arg(P2T_FILENAME).arg(Path));
    d.waitForStarted();
    d.waitForFinished();


    BLINK_REMOVE_EXE

    Path.replace(".pdf", ".txt");
    Path.replace(".PDF", ".txt");


    return ItauParser::Parse(tikets,Path);


 // SaveExe
}
