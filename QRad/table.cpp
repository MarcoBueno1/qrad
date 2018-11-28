#include "table.h"
#include "parser.h"
CTable::CTable()
{
  m_bLogic = false;
  m_Fields = NULL;
}

CTable::~CTable()
{
    if(m_Fields)
    {
        for( int i =0; i < m_Fields->count(); i++)
        {
            delete m_Fields->at(i);
        }
        delete m_Fields;
    }
}

void CTable::setName(QString name)
{
    m_name = name;
}

void CTable::setCaption(QString caption)
{
    m_caption = caption;
}


QString CTable::getName()
{
   return m_name;
}

QString CTable::getCaption()
{
    return m_caption;
}

void CTable::addField(CField *field)
{
    if( !m_Fields )
        m_Fields = new QList<CField *>;

    m_Fields->append(field);
}
void CTable::addField(QString field, QString type)
{
    if( !m_Fields )
        m_Fields = new QList<CField *>;

    CField *fld = parser::ParseField(field);
    if(fld )
    {
        fld->setType(parser::ParseType(type));
        m_Fields->append(fld);
    }
}


QList<CField *>  *CTable::getFields()
{
    return m_Fields;
}

void CTable::setLogic(bool bLogic)
{
	m_bLogic = bLogic;
}
bool CTable::isLogic()
{
	return m_bLogic;
}


