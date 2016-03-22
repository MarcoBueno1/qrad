#ifndef _CFIELD_H
#define _CFIELD_H
#include <QString>
#include "type.h"

class CForeingKey;

typedef enum flMode
{
    fNormal,
    fHide,
    fNoModel
}BN_Field, *PBNField;


class CField
{
public:
    CField();
    ~CField();

    void setField(QString field );
    void setField(QString field , QString type );
    void setCaption(QString field );
    void setMode( BN_Field mode );
    void setType( CType *tp);

    QString getField();
    QString getCaption();
    BN_Field getMode();

    bool HasFKey();
    void setForeingKey(CForeingKey *fkey);
    CForeingKey *getForeingKey();
    CForeingKey *TakeForeingKey();
    CType *getType();
private:
    QString     m_Field;
    QString     m_Caption;
    BN_Field    m_Mode;
    CForeingKey *m_fkey;
    CType       *m_type;
};

#endif // _CFIELD_H
