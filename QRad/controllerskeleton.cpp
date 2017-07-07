#include "controllerskeleton.h"
#include <QFile>

#define CONTROLLER_BODY "#include \"%1controller.h\"\n"\
"\n"\
"%1Controller::%1Controller()\n"\
"{\n"\
"   m_mod = NULL;\n"\
"}\n"\
"\n"\
"%1Controller::~%1Controller()\n"\
"{\n"\
"\n"\
"}\n"\
"\n"\
"void %1Controller::SetModel(%1* mod)\n"\
"{\n"\
"   m_mod = mod;\n"\
"   Load();\n"\
"}\n"\
"\n"\
"void %1Controller::SetWindow(Edit%1 *window)\n"\
"{\n"\
"    m_window = window;\n"\
"\n"\
"}\n"\
"\n"\
"\n"\
"\n"\
"void %1Controller::Save()\n"\
"{\n"\
"\n"\
"%2"\
"    m_window->accept();\n"\
"}\n"\
"\n"\
"void %1Controller::Load()\n"\
"{\n"\
"%3\n"\
"}\n\n"\
"void %1Controller::Cancel()\n"\
"{\n"\
"   m_window->reject();\n"\
"}\n"\


#define CONTROLLER_H_BODY "#ifndef %1CONTROLLER_H\n"\
"#define %1CONTROLLER_H\n"\
"\n"\
"#include <QObject>\n"\
"#include \"edit%1.h\"\n"\
"#include \"%1.h\"\n"\
"\n"\
"class %1Controller\n"\
"{\n"\
"public:\n"\
"    %1Controller();\n"\
"    ~%1Controller();\n"\
"\n"\
"    void SetWindow(Edit%1 *window);\n"\
"    void SetModel(%1* mod);\n"\
"\n"\
"public slots:\n"\
"    void Save();\n"\
"    void Cancel();\n"\
"\n"\
"private:\n"\
"     Edit%1 *m_window;\n"\
"     %1* m_mod;\n"\
"     void Load();\n"\
"};\n"\
"\n"\
"#endif // %1CONTROLLER_H\n"



controllerskeleton::controllerskeleton()
{

}

controllerskeleton::~controllerskeleton()
{

}


void controllerskeleton::Build( skeleton *Skel )
{
    BuildCpp( Skel );
    BuildH( Skel );
}

void controllerskeleton::BuildH(skeleton *Skel  )
{
    QFile *file = new QFile(PathMaker::Path()+Skel->GetName() + "controller.h");

    if(!file->open(QIODevice::WriteOnly))
    {
        qDebug() << "Impossivel abrir:"<< Skel->GetName() << "Controller.h" <<" para escrita" ;
        delete file;
        return;
    }

    QString strModel = QString(CONTROLLER_H_BODY).arg(Skel->GetName());

    file->write(strModel.toUtf8());
    file->flush();
    file->close();
    delete file;

}

void controllerskeleton::BuildCpp( skeleton *Skel  )
{
    QFile *file = new QFile(PathMaker::Path()+Skel->GetName() + "controller.cpp");

    if(!file->open(QIODevice::WriteOnly))
    {
        qDebug() << "Impossivel abrir:"<< Skel->GetName() << "Controller.cpp" <<" para escrita" ;
        delete file;
        return;
    }
    QList<QPair<QString, QString> > fields = Skel->GetFields();

    QString  strModel = "    " + Skel->GetName() +"* mod =  m_mod;\n";
    strModel += "    if( m_mod == NULL)\n";
    strModel += QString("        ") + QString("mod = new ") + Skel->GetName() + QString(";\n\n");

    for( int i = 1; i< fields.count();i++)  /// skip primary key
    {
        if( fields.at(i).second == "QString" )
        {
            strModel += "    mod->set" + fields.at(i).first + "(m_window->LnEdt" +  fields.at(i).first +"->text());\n";
        }
        if( fields.at(i).second == "int" )
        {
            strModel += "    mod->set" + fields.at(i).first + "(m_window->SpnBx" +  fields.at(i).first +"->value());\n";
        }
        if( fields.at(i).second == "double" )
        {
            strModel += "    mod->set" + fields.at(i).first + "(m_window->DblSpnBx" +  fields.at(i).first +"->value());\n";
        }
        if( fields.at(i).second == "bool" )
        {
            strModel += "    mod->set" + fields.at(i).first + "(m_window->ChkBx" +  fields.at(i).first +"->isChecked());\n";
        }
        if( fields.at(i).second == "QDate" )
        {
            strModel += "    mod->set" + fields.at(i).first + "(m_window->DtEdt" +  fields.at(i).first +"->date());\n";
        }
        if( fields.at(i).second == "QTime" )
        {
            strModel += "    mod->set" + fields.at(i).first + "(m_window->TmEdt" +  fields.at(i).first +"->time());\n";
        }

    }

    strModel += "    mod->Save();\n";
    strModel += "    if( m_mod == NULL)\n";
    strModel += "       delete mod;\n";

    QString strLoad  = "    if( m_mod == NULL)\n";
    strLoad         += "      return;\n";

    for( int i = 1; i< fields.count();i++)  /// skip primary key
    {
        if( fields.at(i).second == "QString" )
        {
            strLoad += "    m_window->LnEdt" +  fields.at(i).first +"->setText(mod->get" + fields.at(i).first + "());\n";
        }
        if( fields.at(i).second == "int" )
        {
            strLoad += "    m_window->SpnBx" +  fields.at(i).first +"->setValue(mod->get" + fields.at(i).first + "());\n";
        }
        if( fields.at(i).second == "double" )
        {
            strLoad += "    m_window->DblSpnBx" +  fields.at(i).first +"->setValue(mod->get" + fields.at(i).first + "());\n";
        }
        if( fields.at(i).second == "bool" )
        {
            strLoad += "    m_window->ChkBx" +  fields.at(i).first +"->setChecked(mod->get" + fields.at(i).first + "());\n";
        }
        if( fields.at(i).second == "QDate" )
        {
            strLoad += "    m_window->DtEdt" +  fields.at(i).first +"->setDate(mod->get" + fields.at(i).first + "());\n";
        }
        if( fields.at(i).second == "QTime" )
        {
            strLoad += "    m_window->TmEdt" +  fields.at(i).first +"->setTime(mod->get" + fields.at(i).first + "());\n";
        }

    }


    QString strBody = QString(CONTROLLER_BODY).arg(Skel->GetName()).arg(strModel).arg(strLoad);

    file->write(strBody.toUtf8());
    file->flush();
    file->close();
    delete file;
}
