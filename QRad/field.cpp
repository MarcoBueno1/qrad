#include "field.h"
#include "foreingkey.h"

CField::CField()
{
  m_fkey = NULL;
  m_type = NULL;
  m_Mode = fNormal;
}

CField::~CField()
{
  if( m_fkey )
      delete m_fkey;
  if(m_type)
      delete m_type;
}

void CField::setField(QString field )
{
    m_Field = field;
}

void CField::setField(QString field, QString type)
{
    m_Field = field;
    if( m_type == NULL )
        m_type = new CType;
    m_type->setType(type);
}

void CField::setCaption(QString caption )
{
    m_Caption = caption;
}

void CField::setMode( BN_Field mode )
{
    m_Mode = mode;
}

void CField::setType(CType *tp)
{
    m_type = tp;
}

QString CField::getField( )
{
    return m_Field;
}

QString CField::getCaption()
{
    if( m_Caption.isEmpty())
        m_Caption =m_Field;
    return m_Caption;
}

BN_Field CField::getMode( )
{
    return m_Mode;
}

void CField::setForeingKey(CForeingKey *fkey)
{
    m_fkey = fkey;
}

CForeingKey *CField::getForeingKey()
{
    return m_fkey;
}

CForeingKey *CField::TakeForeingKey()
{
    CForeingKey *f = m_fkey;
    m_fkey = NULL;
    return f;
}

CType *CField::getType()
{
    return m_type;
}

bool CField::HasFKey()
{
    return m_fkey?true:false;
}
