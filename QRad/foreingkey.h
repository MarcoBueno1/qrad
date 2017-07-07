#ifndef CFOREINGKEY_H
#define CFOREINGKEY_H

#include "table.h"

class CForeingKey
{
public:
    CForeingKey();
    ~CForeingKey();

    void setTable(CTable *table );
    void setKey( CField *field);

    CTable *getTable();
    CField * getKey();

    CTable *TakeTable();
    CField *TakeKey();
private:
    CTable *m_table;
    CField *m_key;
};

#endif // CFOREINGKEY_H
