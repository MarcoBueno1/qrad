#ifndef SQLPARSER_H
#define SQLPARSER_H

#include <QString>
#include <QList>
#include "skeleton.h"
#include "table.h"

class SqlParser
{
public:
    SqlParser();
    ~SqlParser();

    int Build( skeleton *skel, QString filename);
    QList <skeleton *> *Build( QString filename );


    void PrintCommand();
private:
    QList<skeleton *> *m_skels;
    QString Match(QString Line);
    QString GetFieldName(QString Line);
    QString GetTableName(QString Line);
    bool SkipLine(QString Line);
    void getForeingKey( QString Line );
    int Parse(QString filename);
	QList<skeleton *> *Parse2(QString filename);
    void Dump();
};

#endif // SQLPARSER_H
