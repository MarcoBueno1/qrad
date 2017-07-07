#ifndef SKELETON_H
#define SKELETON_H

#include <QString>
#include <QMap>
#include <QPair>
#include "model.h"
#include "type.h"
#include "field.h"
#include "table.h"

#include "pathmaker.h"

typedef enum
{
	tpEditSet,
	tpOther
}QRAD_SKEL_TYPE, *PQRAD_SKEL_TYPE;

class skeleton
{
public:
    skeleton(QString strName);
    skeleton();
    ~skeleton();

//    void AddField(QString strName, QString type);
//    void AddField(CField* field, CType* type);
    void AddTable(CTable *);
    QList<CTable*> *GetTables();

    void CreateFile();
//    void SetTable(CTable *table);
//    CTable *GetTable();
	
	void setType(QRAD_SKEL_TYPE type);
	QRAD_SKEL_TYPE getType();

    void SetName(QString name);
    QString GetName();
    void IgnoreIfExist(bool );
    void AddTable(QString Name);
private:
    QString m_Name;
    bool m_IgnoreIfExist;

//    CTable *m_Table;
    QList<CTable*> *m_tables;
    QString KindOf(ModelAttrType attr);

    void CreateH();
    void CreateCpp();

	QRAD_SKEL_TYPE m_type;
};

#endif // SKELETON_H
