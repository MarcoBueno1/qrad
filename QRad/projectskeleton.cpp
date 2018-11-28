#include "projectskeleton.h"
#include <QDebug>
#include <QFile>

#define PROJ_SKEL "QT += core sql network script widgets \n"\
"\n"\
"TARGET = %1plugin\n"\
"TEMPLATE = lib\n"\
"CONFIG += plugin\n"\
"\n"\
"INCLUDEPATH = ../../../libs/QRadCore \\ \n"\
"              ../../../libs/QRadOrm  \n"\
"\n"\
"DESTDIR = .\n"\
"SOURCES = \\ \n"\
"             %1plugin.cpp \\ \n"\
"%2"\
"\n"\
"HEADERS = \\ \n"\
"             %1plugin.h \\ \n"\
"%3"\
"\n"\
"FORMS = \\ \n"\
"%4"\
"\n"\
"LIBS += -L../../../libs/QRadCore \\ \n"\
"        -L../../../libs/QRadOrm \\ \n"\
"        -lqradcore \\ \n"\
"        -lqradorm  \n"\
"\n"\
"win32:QMAKE_LFLAGS += --enable-auto-import \n"\
"\n"\
"OTHER_FILES += %1.json \n"


#define CPP_SKEL "#include \"%1plugin.h\" \n"\
"\n"\
"%1Plugin::%1Plugin()\n"\
"{\n"\
"    m_manager = 0;\n"\
"    m_edit = 0;\n"\
"}\n"\
"\n"\
"%1Plugin::~%1Plugin()\n"\
"{\n"\
"    if( m_manager )\n"\
"        delete m_manager;\n"\
"\n"\
"    if( m_edit )\n"\
"        delete m_edit;\n"\
"}\n"\
"\n"\
"void %1Plugin::onLoad(QRadPluginContainer* container)\n"\
"{\n"\
"    m_container = container;\n"\
"\n"\
"%2"\
"}\n"\
"\n"\
"void %1Plugin::Process( const QString& action )\n"\
"{\n"\
"%3"\
"}\n"\
"\n"\
"void %1Plugin::setParam(QString str, QVariant v)\n"\
"{\n"\
"    Q_UNUSED(str);\n"\
"    Q_UNUSED(v);\n"\
"}\n"\
"\n"\
"QVariant %1Plugin::getParam(QString str)\n"\
"{\n"\
"    Q_UNUSED(str);\n"\
"    return QVariant();\n"\
"}\n"\
"\n"

#define APPEND_ACTION  "    appendAction(\"%1\");\n"

#define PROCESS_ACTION "    if (action.toLower() == QString(\"%1\").toLower())\n"\
    "    {\n"\
    "        if (!m_%2)\n"\
    "            m_%2 = new %3(m_parent);\n"\
    "\n"\
    "        m_%2->show();\n"\
    "    }\n"


#define H_SKEL "#ifndef %1PLUGIN_H\n"\
"#define %1PLUGIN_H\n"\
"\n"\
"#include <QtCore>\n"\
"\n"\
"#include \"qradplugininterface.h\"\n"\
"#include \"qradplugincontainer.h\"\n"\
"%2"\
"\n"\
"class %1Plugin : public QRadPluginInterface\n"\
"{\n"\
"    Q_OBJECT;\n"\
"    Q_INTERFACES( QRadPluginInterface );\n"\
"    Q_PLUGIN_METADATA(IID \"%1plugin\" FILE \"%1.json\")\n"\
"\n"\
"public:\n"\
"     %1Plugin();\n"\
"    ~%1Plugin();\n"\
"\n"\
"    void onLoad(QRadPluginContainer*);\n"\
"\n"\
"protected:\n"\
"%3\n"\
"\n"\
"\n"\
"private slots:\n"\
"    void Process( const QString& action );\n"\
"    void setParam(QString, QVariant);\n"\
"    QVariant getParam(QString);\n"\
"\n"\
"private:\n"\
"    QRadPluginContainer *m_container;\n"\
"};\n"\
"\n"\
"#endif // %1_H\n"

#define JSON_SKEL "{\n"\
"    \"Keys\": [ \"%1lugin\" ]\n"\
"}\n"

#define XML_SKEL "<?xml version=\"1.0\"?>\n"\
"\n"\
"<pluginlist>\n"\
"    <!-- Pre-sell Plugin -->\n"\
"\n"\
"    <plugin name=\"%1plugin\" library=\"./%1plugin\" enabled=\"true\">\n"\
"        <menu path=\"Cadastro\" title=\"Consulta de %2\" action=\"Manage\" hotkey=\"CTRL+M\" permission=\"%1\" />\n"\
"        <menu path=\"Cadastro\" title=\"Cadastrar %2\" action=\"Edit\" hotkey=\"CTRL+E\" permission=\"%1\" />\n"\
"    </plugin>\n"\
"\n"\
"</pluginlist>"


projectskeleton::projectskeleton()
{

}

projectskeleton::~projectskeleton()
{

}

void projectskeleton::Build(skeleton *skel)
{
    qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;
    BuildPro( skel);
    qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;
    BuildH( skel);
    qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;
    BuildCPP( skel);
    qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;
    BuildJson( skel);
    qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;
    BuildXML(skel);
}

void projectskeleton::BuildPro(skeleton *skel)
{
    QString Name = skel->getName();

    QFile *file = new QFile(PathMaker::Path()+ Name + ".pro");

    if(!file->open(QIODevice::WriteOnly))
    {
        qDebug() << "Impossivel abrir:"<< Name << ".pro "<<" para escrita" ;
        delete file;
        return;
    }

    QList<CTable *>*pTables = skel->getTables();

    QString strSources; //manager  edit
    QString strHeaders; //manager  edit
    QString strUI; //manager  edit

    for( int i = 0; pTables && (i < pTables->count()); i++   )
    {

        strSources += QString("             manager%1.cpp \\ \n").arg(pTables->at(i)->getName());
        strSources += QString("             edit%1.cpp \\ \n").arg(pTables->at(i)->getName());
        strSources += QString("             %1.cpp \n").arg(pTables->at(i)->getName());


        strHeaders += QString("             manager%1.h \\ \n").arg(pTables->at(i)->getName());
        strHeaders += QString("             edit%1.h \\ \n").arg(pTables->at(i)->getName());
        strHeaders += QString("             %1.h \n").arg(pTables->at(i)->getName());


        strUI += QString("             manager%1.ui \\ \n").arg(pTables->at(i)->getName());
        strUI += QString("             edit%1.ui \n").arg(pTables->at(i)->getName());


        //// cria subtabelas para cada item multi
        QList<CField*> *fields = pTables->at(i)->getFields();
        for( int j = 0; fields && (j < fields->count()); j++ )
        {
            if( !fields->at(j)->getType()->getTable().isEmpty() &&
                    ((fields->at(j)->getType()->geTpMode() == tpMulti)
                    ||(fields->at(j)->getType()->geTpMode() == tpMultiEdtFull)))
            {
                if( strSources.contains(QString("manager%1.cpp").arg(fields->at(j)->getType()->getTable())))
                        continue;

                if( !strSources.endsWith("\\"))
                    strSources.insert(strSources.lastIndexOf("\n")-1,"\\");

                strSources += QString("             manager%1.cpp \\ \n").arg(fields->at(j)->getType()->getTable());
                strSources += QString("             edit%1.cpp \\ \n").arg(fields->at(j)->getType()->getTable());
                strSources += QString("             %1.cpp \n").arg(fields->at(j)->getType()->getTable());

                if( !strHeaders.endsWith("\\"))
                    strHeaders.insert(strHeaders.lastIndexOf("\n")-1,"\\");

                strHeaders += QString("             manager%1.h \\ \n").arg(fields->at(j)->getType()->getTable());
                strHeaders += QString("             edit%1.h \\ \n").arg(fields->at(j)->getType()->getTable());
                strHeaders += QString("             %1.h \n").arg(fields->at(j)->getType()->getTable());

                if( !strUI.endsWith("\\"))
                    strUI.insert(strUI.lastIndexOf("\n")-1,"\\");

                strUI += QString("             manager%1.ui \\ \n").arg(fields->at(j)->getType()->getTable());
                strUI += QString("             edit%1.ui \n").arg(fields->at(j)->getType()->getTable());
            }
        }
    }
    QString strHead = QString(PROJ_SKEL).arg(Name).arg(strSources).arg(strHeaders).arg(strUI);

    file->write(strHead.toUtf8());

    file->flush();
    file->close();

	delete file;

}

void projectskeleton::BuildH(skeleton *skel)
{
    QString Name = skel->getName();

    QFile *file = new QFile(PathMaker::Path()+ Name + "plugin" +  ".h");

    if(!file->open(QIODevice::WriteOnly))
    {
        qDebug() << "Impossivel abrir:"<< Name << "plugin" <<  ".h"<<" para escrita" ;
        delete file;
        return;
    }
    QList<CTable *>*pTables = skel->getTables();

    QString strIncludes; //manager  edit
    QString strProtected;
    QString strHead;

    for( int i = 0; pTables && ( i < pTables->count()); i++   )
    {

        strIncludes += QString("#include \"manager%1.h\"\n").arg(pTables->at(i)->getName());
        strIncludes += QString("#include \"edit%1.h\"\n").arg(pTables->at(i)->getName());


        strProtected += QString("    Manager%1 *m_manager;\n").arg(pTables->at(i)->getName());
        strProtected += QString("    Edit%1 *m_edit;\n").arg(pTables->at(i)->getName());

//        strHead += QString(H_SKEL).arg(pTables->at(i)->getName()).arg(strIncludes).arg(strProtected);
        strHead += QString(H_SKEL).arg(Name).arg(strIncludes).arg(strProtected);
    }


    file->write(strHead.toUtf8());

    file->flush();
    file->close();
	
	delete file;
	
}

void projectskeleton::BuildCPP(skeleton *skel)
{
    QString Name = skel->getName();

    QFile *file = new QFile(PathMaker::Path()+ Name + "plugin" +  ".cpp");

    if(!file->open(QIODevice::WriteOnly))
    {
        qDebug() << "Impossivel abrir:"<< Name << "plugin" <<  ".cpp"<<" para escrita" ;
        delete file;
        return;
    }

    QString strActions; 
    QString strProcess;
	QString strHead ;

    QList<CTable *>*pTables = skel->getTables();
    for( int i = 0; pTables && (i<pTables->count()); i++   )
    {
        strActions += QString(APPEND_ACTION).arg("Manage");
        strActions += QString(APPEND_ACTION).arg("Edit");


        strProcess += QString(PROCESS_ACTION).arg("Manage").arg("manager").arg("Manager"+pTables->at(i)->getName());
        strProcess += QString(PROCESS_ACTION).arg("Edit").arg("edit").arg("Edit"+pTables->at(i)->getName());
//		strHead += QString(CPP_SKEL).arg(pTables->at(i)->getName()).arg(strActions).arg(strProcess);
        strHead += QString(CPP_SKEL).arg(Name).arg(strActions).arg(strProcess);
    }


    file->write(strHead.toUtf8());

    file->flush();
    file->close();
	
	delete file;
}

void projectskeleton::BuildJson(skeleton *skel)
{
    QString Name = skel->getName();

    QFile *file = new QFile(PathMaker::Path()+ Name  +  ".json");

    if(!file->open(QIODevice::WriteOnly))
    {
        qDebug() << "Impossivel abrir:"<< Name << ".json"<<" para escrita" ;
        delete file;
        return;
    }

    QString strHead = QString(JSON_SKEL).arg(Name);

    file->write(strHead.toUtf8());

    file->flush();
    file->close();
	
	delete file;
}

void projectskeleton::BuildXML(skeleton *skel)
{
    QString Name = skel->getName();

    QFile *file = new QFile(PathMaker::Path()+ Name  +  "plugin.xml");

    if(!file->open(QIODevice::WriteOnly))
    {
        qDebug() << "Impossivel abrir:"<< Name << "plugin.xml" <<" para escrita" ;
        delete file;
        return;
    }

    QString strHead ;

    QList<CTable *>*pTables = skel->getTables();
    for( int i = 0; pTables && (i < pTables->count()); i++   )
    {
        strHead += QString(XML_SKEL).arg(pTables->at(i)->getName()).arg(pTables->at(i)->getCaption());
    }

    file->write(strHead.toUtf8());

    file->flush();
    file->close();
	
	delete file;
}
