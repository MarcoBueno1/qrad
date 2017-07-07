#ifndef _CType_H
#define _CType_H

#include <QString>
#include <QStringList>


typedef enum tpMode
{
    tpNormal,
    tpMulti,
    tpMultiEdtFull,
    tpCTypeNxN
}BUENO_TP_MODE, *PBUENO_TP_MODE;


typedef enum tpSearch
{
    tpNoSearch,
    tpSearch,
    tpSearchMaster
}BUENO_TP_SEARCH, *PBUENO_TP_SEARCH;

class CType
{
public:
    CType();
    ~CType();

    void setType( QString type );
    void setTpMode(BUENO_TP_MODE tp );
    void setTpSearch( BUENO_TP_SEARCH sr );
    void setTable(QString name );
    void setField(QString name );
    void addDefault(QString name );

    QString getType();
    BUENO_TP_MODE geTpMode();
    BUENO_TP_SEARCH getTpSearch();
    QString getTable();
    QString getField();
    QStringList getDefaults();

private:
    QString m_type;
    BUENO_TP_MODE m_tpmode;
    BUENO_TP_SEARCH m_tpSearch;
    QString m_Table;
    QString m_Field;
    QStringList m_defaults;
};

#endif // CType_H
