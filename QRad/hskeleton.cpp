#include "hskeleton.h"
#include <QFile>
#define H_BODY "#ifndef EDIT%1_H\n"\
"#define EDIT%1_H\n"\
"\n"\
"#include <QDialog>\n"\
"#include <QKeyEvent>\n"\
"#include \"%1.h\"\n"\
"\n"\
"namespace Ui {\n"\
"class Edit%1;\n"\
"}\n"\
"\n"\
"class Edit%1 : public QDialog\n"\
"{\n"\
"    Q_OBJECT\n"\
"\n"\
"public:\n"\
"    explicit Edit%1(QWidget *parent = 0);\n"\
"    ~Edit%1();\n"\
"    void SetModel(%1* mod);\n"\
"    %1* GetSaved();\n"\
"\n"\
"public slots:\n"\
"    void Save();\n"\
"    void Cancel();\n"\
"    %2\n"\
"private:\n"\
"    Ui::Edit%1 *ui;\n"\
"    %1* m_mod;\n"\
"    %1* m_lastMod;\n"\
"    void Load();\n"\
"    void showEvent(QShowEvent *event);\n"\
"    void keyPressEvent(QKeyEvent *e);\n"\
"};\n"\
"\n"\
"#endif // EDIT%1_H\n"\


#define BN_SLOT_CMB_EDT "void doEditCmbBx%1();\n"\

hskeleton::hskeleton()
{
  m_IgnoreIfExist = false;
}

hskeleton::~hskeleton()
{

}
void hskeleton::Build( skeleton *Skel )
{
    QList<CTable *>*pTables = Skel->GetTables();

    for( int i = 0; pTables && (i < pTables->count()); i++ )
    {

	qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;

        QFile *file = new QFile(PathMaker::Path()+"edit" + pTables->at(i)->getName() + ".h");

        if( m_IgnoreIfExist && file->exists() )
        {
            qDebug() << "Already exist, Ignoring " << "Edit" << pTables->at(i)->getName() <<".h" ;
            return;
        }

        if(!file->open(QIODevice::WriteOnly))
        {
            qDebug() << "Impossivel abrir:"<<"Edit" << pTables->at(i)->getName() << ".h" <<" para escrita" ;
            delete file;
            return;
        }

        QList<CField*>  *fields = pTables->at(i)->getFields();
        QString strSlot;
        for( int j = 1;  fields && (j< fields->count());j++)  /// skip primary key
        {
            if( fields->at(j)->getType()->getType() == "int" )
            {
                switch(fields->at(j)->getType()->geTpMode())
                {
                    case tpMultiEdtFull:
                        strSlot += QString(BN_SLOT_CMB_EDT).arg(fields->at(j)->getCaption());
                     break;
                    default:
                     break;
                }
            }
        }



        QString strBody = QString(H_BODY).arg(pTables->at(i)->getName()).arg(strSlot);

        file->write(strBody.toUtf8());
        file->flush();
        file->close();
        delete file;
    }
		qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;

}

void hskeleton::IgnoreIfExist(bool bIgnore)
{
    m_IgnoreIfExist = bIgnore;
}

