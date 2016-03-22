#ifndef _CTABLE_H
#define _CTABLE_H

#include <QString>
#include <QList>
#include <QPair>
#include "field.h"
#include "type.h"

class CTable
{
public:
    CTable();
    ~CTable();

    void setName(QString name);
    void setCaption(QString caption);
    void setLogic(bool bLogic);

    QString getName();
    QString getCaption();
	bool isLogic();

    void AddField(CField *field);
    QList<CField*> *getFields();

    void AddField(QString field, QString type);
private:
    QList<CField*> *m_Fields;
    QString m_name;
    QString m_caption;
	bool    m_bLogic;
};

#endif // _CTABLE_H
