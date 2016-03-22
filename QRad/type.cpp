#include "type.h"

CType::CType()
{
  m_tpmode = tpNormal;
  m_tpSearch = tpNoSearch;
}

CType::~CType()
{

}

void CType::setType( QString type )
{
    m_type = type;
}

void CType::setTpMode(BUENO_TP_MODE tp )
{
    m_tpmode = tp;
}

void CType::setTpSearch(BUENO_TP_SEARCH sr)
{
    m_tpSearch = sr;
}

void CType::setTable(QString name )
{
    m_Table = name;
}

void CType::setField(QString name )
{
    m_Field = name;
}

void CType::addDefault(QString name )
{
    m_defaults.append(name);
}

QString CType::getType( )
{
    return m_type;
}


BUENO_TP_MODE CType::geTpMode()
{
    return m_tpmode;
}

BUENO_TP_SEARCH CType::getTpSearch()
{
    return m_tpSearch;
}

QString CType::getTable()
{
    return m_Table;
}

QString CType::getField()
{
    return m_Field;
}

QStringList CType::getDefaults()
{
    return m_defaults;
}
