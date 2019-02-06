#include <QApplication>
#include <QSqlDatabase>

#include "skeleton.h"
#include "sqlparser.h"
#include "pathmaker.h"
#include "qradhead.h"
#include "qradgui.h"

#include <QDebug>

void configureDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("qraddb");
    db.setHostName("localhost");
    db.setPassword("qradpassword");
    db.setUserName("qraddb");

    if(!db.open())
    {
        qDebug() <<"Database error";
    }
    else
    {
        qDebug() << "connection Ok";
    }

}

bool WrongParameters()
{
   if( QCoreApplication::arguments().count() < 4 )
       return true;

   if(( QCoreApplication::arguments().count() == 4) && !QCoreApplication::arguments().contains("-f"))
       return true;


   if(( QCoreApplication::arguments().at(1) == "-f" ) && (QCoreApplication::arguments().count() > 2 ))
   {
       return false;
   }


   if(( QCoreApplication::arguments().at(1) != "-s" )
           || (QCoreApplication::arguments().at(3) != "-t" )
           || (QCoreApplication::arguments().at(5) != "-c" ) )
   {
       return true;
   }


   for( int i = 5; i < (QCoreApplication::arguments().count()-2); i++ )
   {
         if( (QCoreApplication::arguments().at(i) != "-c" ) ||
             (QCoreApplication::arguments().at(i+2) != "-i" ) )
             return true;
   }

   return false;
}

int main(int argc, char *argv[])
{
    bool bDoCreate = false;
    QApplication a(argc, argv);

    /* future use
    if(argc == 1)
    {
        configureDatabase();
        QRadGui *gui = new QRadGui;
        gui->exec();
        delete gui;
        return 0;
    }
    */

    if( WrongParameters() )
    {
        qInfo() << "few arguments:";
        qInfo() << "-s project_name";
        qInfo() << "-t table";
        qInfo() << "-c field";

        qInfo() << "-i fieldtype";

        qInfo() << "----------------";
        qInfo() << "Field types:";
        qInfo() << "----------------";
        qInfo() << "int";
        qInfo() << "QString";
        qInfo() << "QDate";
        qInfo() << "QTime";
        qInfo() << "bool";
        qInfo() << "double";
        qInfo() << "----------------";
        qInfo() << "Field Modifiers:";
        qInfo() << "----------------";
        qInfo() << ":hide                         : hide field, exists only in databse";
        qInfo() << ":nomodel                      : only exist in form ( not in dtabase )";
        qInfo() << ":search                       : field used to search";
        qInfo() << ":searchmaster                 : master search field";
        qInfo() << ":multi.table.field[value1,..] : Foreing key ( creates smart comboboxes and related table )";

        qInfo() << "----------------";
        qInfo() << "SQL Parser:";
        qInfo() << "----------------";
        qInfo() << "-f <file.sql> -command        : create a commando line parameters from sql field";

        qInfo() << "Examples:";
        qInfo() << "----------------";
        qInfo() << "qrad -s User -t User -c id -i int -c name.Name -i QString:searchmaster -c gender.Gender -i int:multi:Gender.Description[Male,Female] -c birthday -i QDate -c registration -i QTime";
        return 0;
    }
	

    skeleton *skel =  new skeleton();

    for( int i = 0; i < argc-1; i++ )
    {
        if(QString(argv[i]) == "-s")
        {
            skel->setName(argv[i+1]);
            PathMaker::setDir(skel->getName() );
        }
        if(QString(argv[i]) == "-t" )
		{
            CTable *table = new CTable;
            table->setName(argv[i+1]);
            skel->addTable(table);
		}
        else if (QString(argv[i]) == "-c" )
        {
            QList<CTable *> *m_tables = skel->getTables();
            if( m_tables )
            {
                CTable *pTable = m_tables->at(m_tables->count()-1);
                pTable->addField(QString(argv[i+1]), QString(argv[i+3]));
                bDoCreate = true;
            }

        }
        else if (QString(argv[i]) == "-f" )
        {
          SqlParser *parser = new SqlParser;
          if( parser->Build(skel, (argv[i+1])))
                  bDoCreate = true;
          if(( argc >  (i + 2) ) && (QString(argv[i+2]) == "-command"))
          {
              parser->PrintCommand();
              delete parser;
              return 0;
          }
          delete parser;
          break;
        }
    }

    if( bDoCreate )
    {
		QRadHead *head = new QRadHead;
		head->addSkel(skel);
		head->Run();
		delete head;
    }
	else
		delete skel;
	
    return 0;// a.exec();
}
