#include <QFile>
#include <QDebug>

#include "skeleton.h"
#include "parser.h"


skeleton::skeleton(QString strName)
{
    m_Name = strName;
    m_IgnoreIfExist = false;
//    m_Table = parser::parseTable(strName);
    m_type = tpEditSet;
//    m_fields = NULL;
    m_tables = NULL;
}
skeleton::skeleton()
{
    m_IgnoreIfExist = false;
    m_tables = NULL;
    m_type = tpEditSet;
//    m_Table  = new CTable;
//    m_fields = NULL;
}

void skeleton::IgnoreIfExist(bool bIgnore)
{
    m_IgnoreIfExist = bIgnore;
}

void skeleton::SetName(QString name)
{
//    if( m_Table )
  //      delete m_Table;
//    m_Table = parser::parseTable(name);
    m_Name =  name;
}

QString skeleton::GetName()
{
    if( m_Name.isEmpty() && m_tables)
        m_Name = m_tables->at(0)->getName();
    return m_Name;
}


skeleton::~skeleton()
{
    if( m_tables )
    {
        for( int i = 0; i< m_tables->count();i++)  /// skip primary key
        {
            delete m_tables->at(i);
        }
        delete m_tables;
    }
}

void skeleton::AddTable(CTable *tbl)
{
    if( !m_tables )
        m_tables = new QList<CTable*>;

    m_tables->append(tbl);
}
void skeleton::AddTable(QString Name)
{
    if( !m_tables )
        m_tables = new QList<CTable*>;

    CTable *tbl = parser::parseTable(Name);

    if(tbl)
        m_tables->append(tbl);
}


//void skeleton::AddField(QString strName, QString type)
//{
//    CField *cfield = parser::ParseField(strName);
//    CType *ctype = parser::ParseType(type);

//    m_fields.append(qMakePair(cfield,ctype));
//}

//void skeleton::AddField(CField* field, CType* type)
//{
//    m_fields.append(qMakePair(field,type));
//}


QList<CTable *> *skeleton::GetTables()
{
    return m_tables;
}

void skeleton::CreateFile()
{
    CreateH();
    CreateCpp();
}

void skeleton::CreateH()
{
    for( int i =0; m_tables && (i < m_tables->count()); i++ )
    {
        QFile *file = new QFile(PathMaker::Path()+m_tables->at(i)->getName()+ ".h");

        if( m_IgnoreIfExist && file->exists() )
        {
            qDebug() << "Already exist, Ignoring " << m_tables->at(i)->getName()+".h";
            return;
        }

        if(!file->open(QIODevice::WriteOnly))
        {
            qDebug() << "Impossivel abrir:"<<m_tables->at(i)->getName()+".h"<<" para escrita" ;
            delete file;
            return;
        }


        QString strBody = QString("#ifndef %1_H\n"\
                                    "#define %1_H\n"\
                                    " \n"\
                                    "#include \"orm.h\"\n"\
                                    "\n"\
                                    "DECLARE_ORM(%1)\n"\
                                    "\n"\
                                    "DECLARE_CLASS(%1)\n").arg(m_tables->at(i)->getName());


//        QList<QPair<QString, QString> > ::iterator i;
        QList<CField*> *pFields = m_tables->at(i)->getFields();

        qDebug() << "Field count:" << pFields->count();
        for( int j = 0; pFields && (j< pFields->count());j++)
        {
            if( pFields->at(j)->getMode() == fNoModel)
			{
                qDebug() << "CONTINUE PARA:" << pFields->at(j)->getField();
                continue;
			}

            strBody += QString("     ORM_DECLARE_ATTRIBUTE(%1, %2, %3);\n")
                    .arg(m_tables->at(i)->getName()).arg(pFields->at(j)->getType()->getType()).arg(pFields->at(j)->getCaption());
        }


        strBody += QString("     ORM_MATCH_TABLE(%1, \"%1\")\n"\
                            "ORM_END_MAP()\n"\
                            "#endif \n").arg(m_tables->at(i)->getName());


        file->write(strBody.toUtf8());
        file->flush();
        file->close();
        delete file;
    }
}


void skeleton::CreateCpp()
{
    for( int i =0; m_tables && (i < m_tables->count()); i++ )
    {

        QFile *file = new QFile(PathMaker::Path()+m_tables->at(i)->getName() + ".cpp");

        if( m_IgnoreIfExist && file->exists() )
        {
            qDebug() << "Already exist, Ignoring " << m_tables->at(i)->getName()+".cpp";
            return;
        }

        if(!file->open(QIODevice::WriteOnly))
        {
            qDebug() << "Impossivel abrir:"<<m_tables->at(i)->getName()+ ".cpp" <<" para escrita" ;
            delete file;
            return;
        }


        QString strBody = QString( "#include \"%1.h\""\
    "\n\n"\
    "\n"\
    "ORM_BEGIN_MAP(%1)\n").arg(m_tables->at(i)->getName());

        QList<CField *> *pFields=  m_tables->at(i)->getFields();
		
        strBody += QString( "     ORM_MAP_PRIMARYKEY(%1, \"%2\");\n").arg(pFields->at(0)->getCaption()).arg(pFields->at(0)->getField());


        for( int j = 1; pFields && (j< pFields->count()) ;j++)
        {
            if( pFields->at(j)->getMode() == fNoModel )
                continue;

            strBody += QString("     ORM_MAP_FIELD(%1, \"%2\");\n")
                    .arg(pFields->at(j)->getCaption()).arg(pFields->at(j)->getField());
        }


        strBody += "ORM_END_MAP()\n\n";

        file->write(strBody.toUtf8());
        file->flush();
        file->close();
        delete file;
    }

}



QString skeleton::KindOf(ModelAttrType attr)
{
    switch (attr)
    {
     case ATTR_QString:
                         return "QString";
     case ATTR_int:
                         return "int";
     case ATTR_double:
                         return "double";
     case ATTR_bool:
                         return "bool";
     case ATTR_QDate:
                         return "QDate";
     case ATTR_QTime:
                         return "QTime";
     case ATTR_QChar:
     case ATTR_char:
     case ATTR_Serial:
                        return "QString";
    }
    return "QString";
}

void skeleton::setType(QRAD_SKEL_TYPE type)
{
	m_type = type;
}

QRAD_SKEL_TYPE skeleton::getType()
{
	return m_type;
}
