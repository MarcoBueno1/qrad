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
        qDebug() <<"Impossivel conectar no banco";
    }
    else
    {
        qDebug() << "Conexão Ok";
    }

}
int main(int argc, char *argv[])
{
    bool bDoCreate = false;
    QApplication a(argc, argv);

    if(argc == 1)
    {
        configureDatabase();
        QRadGui *gui = new QRadGui;
        gui->exec();
        delete gui;
        return 0;
    }

    if( argc < 4 )
    {
        qDebug() << "Poucos argumentos:";
        qDebug() << "-t tabela -c campo -i tipo          : Gera codigo a partir de linha de comando";
        qDebug() << "-t tabela -c campo -i tipo:multi:Tabela.Campo[marco,antonio,bueno,da,silva]   ";
        qDebug() <<                                     ": Gera codigo a partir de linha de comando, campo multiselecao associado a tabela";
        qDebug() << "-f <arquivo.sql>                    : Gera codigo a partir de um arquivo em SQL";
        qDebug() << "-f <arquivo.sql> -command           : Gera linha de comando a partir de um arquivo em SQL";
        qDebug() << "Modificadores de campo:";
        qDebug() << "----------------";
        qDebug() << ":hide                               - campo escondido, não aparece nas telas de edição nem de pesquisa, apenas banco de dados";
        qDebug() << ":nomodel                            - campo visual, não existe na tabela\n";
        qDebug() << "Tipos possiveis:";
        qDebug() << "----------------";
        qDebug() << "int";
        qDebug() << "QString";
        qDebug() << "bool";
        qDebug() << "double\n\n";
        qDebug() << "Subclasses de tipos:";
        qDebug() << "---------------------";
        qDebug() << ":search                              - Tela de Pesquisa: Indica que este campo será usado como campo de pesquisa";
        qDebug() << ":searchmaster                        - Tela de Pesquisa: Indica que este campo terá precedencia na tela de pesquisa";
        qDebug() << ":multi.tabela.campo[valor1,..]       - Cadastro........: Indica que este campo é chave estrangeira, relacionado a outra tabela conforme exemplo abaixo ( cria combobox inteligente )\n";
        qDebug() << "Exemplos:";
        qDebug() << "----------------";
        qDebug() << "/qrad -t dsm_sale.Depositario -c id -i int -c total -i double:search -c discount -i double:search -c cashierid.Envelope -i int:multi:Envelopes.Tipo[A4,A5] -c obs -i QString:searchmaster -c coo -i int";
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
