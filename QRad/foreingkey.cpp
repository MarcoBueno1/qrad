#include "foreingkey.h"

CForeingKey::CForeingKey()
{
   m_table= new CTable;
   m_key = new CField;
}

CForeingKey::~CForeingKey()
{
   if(m_table)
       delete m_table;
   if(m_key)
       delete m_key;
}

void CForeingKey::setTable( CTable *table )
{
    if( m_table )
        delete m_table;

    m_table = table;
}

void CForeingKey::setKey( CField *field)
{
   if(m_key)
       delete m_key;
   m_key = field;
}

CTable * CForeingKey::getTable()
{
    return m_table;
}

CTable * CForeingKey::TakeTable()
{
    CTable *t = m_table;
    t = NULL;
    return t;
}

CField * CForeingKey::getKey()
{
    return m_key;
}
CField * CForeingKey::TakeKey()
{
    CField *f = m_key;
    m_key = NULL;
    return f;
}
