#include "parser.h"
#include <QDebug>

// sample syntax
// Field
// :hide.caption
// :nomodel.caption

// Type
// :multi.tbl.f{default1,default2}
// :multiEdtFull.tbl.f{default1,default2}

#define TOKEN_SKEL  "-s"
#define TOKEN_TABLE "-t"
#define TOKEN_FIELD "-f"
#define TOKEN_TYPE  "-i"
#define TOKEN_FILE  "-file"
#define TOKEN_COMMAND  "-c"


QRAD_STATE parser::m_state   = isIdle;
QList<QRAD_MACHINE_NODE *> *parser::m_machine = NULL;
QList<skeleton*> *parser::m_skels= NULL;
bool parser::m_error = false;

parser::parser()
{

}

parser::~parser()
{

}

CType *parser::ParseType(QString Token )
{
  QString tmpToken = Token;
  QString tmp;
  CType * m = new CType;

  m->setTpSearch(tpNoSearch);
  if( tmpToken.contains( TOKEN_TYPE_SEARCH_MASTER )  )
  {
      tmpToken.remove(TOKEN_TYPE_SEARCH_MASTER);
      m->setTpSearch(tpSearchMaster);
  }
  else if( tmpToken.contains( TOKEN_TYPE_SEARCH)  )
  {
      tmpToken.remove(TOKEN_TYPE_SEARCH);
      m->setTpSearch(tpSearch);
  }

  tmp = tmpToken;
  if( tmp.contains(":"))
      tmp.truncate(tmp.indexOf(":"));
  if( tmp.contains("."))
      tmp.truncate(tmp.indexOf("."));

  m->setType(tmp);

  if( tmpToken.contains( TOKEN_TYPE_MULTI )  || tmpToken.contains( TOKEN_TYPE_MULTI_EDTFULL ) )
  {
    qDebug() <<  tmpToken ;
    if( tmpToken.contains( TOKEN_TYPE_MULTI_EDTFULL ) )
        m->setTpMode(tpMultiEdtFull);
    else
        m->setTpMode(tpMulti);
    tmp = tmpToken.mid(tmpToken.lastIndexOf(":")+1);
    tmp.truncate(tmp.indexOf("."));
    m->setTable(tmp);

    tmp = tmpToken.mid(tmpToken.lastIndexOf(".")+1);
    if(tmp.indexOf("[") > -1 )
        tmp.truncate(tmp.indexOf("["));
    m->setField(tmp);

    if( (tmpToken.indexOf("[") > -1 ) )
    {
        tmp = tmpToken.mid(tmpToken.indexOf("[")+1);
        do
        {
           QString tmp2 = tmp;
           if(tmp.indexOf(",") > -1 )
               tmp2.truncate(tmp.indexOf(","));
           if(tmp.indexOf("]") > -1 )
               tmp2.truncate(tmp.indexOf("]"));

           m->addDefault(tmp2);
           if( (tmp.indexOf(",") == -1 ) )
           {
              break;
           }
           else
               tmp = tmp.mid(tmp.indexOf(",")+1);
        }while(1);
    }

  }

  return m;
}


CField *parser::ParseField(QString Token )
{
  QString tmp = Token;
  CField * field = new CField;

  field->setMode(fNormal);
  if( tmp.contains("."))
  {
      field->setCaption(tmp.mid(tmp.indexOf(".")+1));
  }
  if( tmp.contains("."))
      tmp.truncate(tmp.indexOf("."));
  if( tmp.contains(":"))
      tmp.truncate(tmp.indexOf(":"));

  if( field->getCaption().isEmpty())
      field->setCaption( tmp );
  field->setField( tmp );

  if( Token.contains( TOKEN_FIELD_HIDE) )
  {
    field->setMode(fHide);
  }
  else if( Token.contains( TOKEN_FIELD_NOMODEL) )
      field->setMode(fNoModel);

  return field;
}

CTable *parser::parseTable(QString Token)
{
    QString tmp = Token;
    CTable * table = new CTable;

    if( tmp.contains("."))
    {
        table->setCaption(tmp.mid(tmp.indexOf(".")+1));
    }
    if( tmp.contains("."))
        tmp.truncate(tmp.indexOf("."));
//    if( tmp.contains(":"))
//        tmp.truncate(tmp.indexOf(":"));

    if( table->getCaption().isEmpty())
        table->setCaption( tmp );
    table->setName( tmp );

    qDebug() << "Name:" << table->getName() << " Caption:" << table->getCaption();

    return table;
}

skeleton * parser::parseSkel(QString Token)
{
	Q_UNUSED(Token);
    return new skeleton;
}

void parser::printInvalid( char *arg )
{
	qDebug() << "Ivalid arg:" << QString(arg);
	m_error = true;
}

void parser::do_StoreS(char *szData )
{
	skeleton *skl = parseSkel(QString(szData));
	if( !skl )
    {
		printInvalid( szData );
		return;
	}   	
	
	if( NULL == m_skels)
		m_skels = new QList<skeleton*>;
	
	m_skels->append(skl);
}

void parser::do_StoreT(char *szData )
{
    if( !m_skels->count())
    {
        printInvalid( szData );
        return;
    }

    CTable *tbl = parseTable(QString(szData));

    m_skels->at(m_skels->count()-1)->addTable(tbl);
}

void parser::do_StoreFi(char *szData )
{
    Q_UNUSED(szData);
	
}

void parser::do_StoreF(char *szData )
{
    if( !m_skels->count() ||
        !m_skels->at(m_skels->count()-1)->getTables())
    {
        printInvalid( szData );
        return;
    }
    CField *field = ParseField(QString(szData));
	QList<CTable *> *tables = m_skels->at(m_skels->count()-1)->getTables();
	tables->at(tables->count()-1)->addField(field);

}

void parser::do_StoreI(char *szData )
{
    if( !m_skels->count() ||
        !m_skels->at(m_skels->count()-1)->getTables())
    {
        printInvalid( szData );
        return;
    }
    CType *type = ParseType(QString(szData));
	QList<CTable *> *tables = m_skels->at(m_skels->count()-1)->getTables();
	QList<CField *> *fields = tables->at(tables->count()-1)->getFields();
	if( !fields )
    {
        printInvalid( szData );
        return;
    }
	
    fields->at(fields->count()-1)->setType(type);

}
void parser::do_StoreC(char *szData )
{
	Q_UNUSED(szData);
	
}


void parser::AddLine( QRAD_STATE State, QRAD_EVENT Event, QRAD_STATE nextState, QRAD_ACTION Action )
{
	QRAD_MACHINE_NODE *pNode =  (QRAD_MACHINE_NODE*)malloc(sizeof(QRAD_MACHINE_NODE));
	
	pNode->State     = State;
	pNode->Event     = Event;
	pNode->nextState = nextState;
	pNode->Action     = Action;
	m_machine->append(pNode);
}

void parser::InitFSM()
{
   m_machine = new QList<QRAD_MACHINE_NODE *>;
	
   AddLine(isIdle, evS,  isWaitT,  do_StoreS);
   AddLine(isIdle, evFi, isTakedFi,  do_StoreFi);
   
   AddLine(isWaitT, evF, isWaitF, do_StoreT);

   AddLine(isWaitF, evF, isUnchanged, do_StoreF);
   AddLine(isWaitF, evI, isUnchanged, do_StoreI);
   AddLine(isWaitF, evS, isWaitT, do_StoreS);
   
   AddLine(isTakedFi, evC, isIdle, do_StoreC);
}

void parser::FinishFSM()
{
	for( int i=0; i< m_machine->count(); i ++ )
	{
		delete m_machine->at(i);
		m_machine->removeAt(i);
	}
	delete m_machine;
}


int parser::ProcessFSM( QRAD_EVENT event, char *szData)
{
	bool bTransitionFound = false;
	for( int i = 0 ; i < m_machine->count() && !bTransitionFound; i++ )
	{
		if( (m_machine->at(i)->Event == event ) && (m_machine->at(i)->State == m_state) )
		{
			if( m_machine->at(i)->nextState != isUnchanged)
				m_state = m_machine->at(i)->nextState;
			m_machine->at(i)->Action(szData);
			bTransitionFound = true;
		}
	}
	if( !bTransitionFound || m_error)
	{
		qDebug() << "Error";
		return 0;
	}
		
	return 1;
}

QList<skeleton*> * parser::parseLine(int argc, char *argv[])
{
	QRAD_EVENT event = isUnknown;
	m_skels = NULL;

    InitFSM();	
	for( int i = 0; (i < argc) ; i++ )
	{
		if( QString(argv[i]).toLower() == TOKEN_COMMAND )
		{
			ProcessFSM(evC, argv[i]); // dummy argv
		}
		else if( i+1 >= argc)
		{
			break;
		}
		else if ( QString(argv[i]).toLower() == TOKEN_SKEL )
		{
			event = evS;
			i++;
		}
		else if( QString(argv[i]).toLower() == TOKEN_TABLE )
		{
			event = evT;
			i++;
		}
		else if( QString(argv[i]).toLower() == TOKEN_FIELD )
		{
			event = evF;
			i++;
		}
		else if( QString(argv[i]).toLower() == TOKEN_TYPE )
		{
			event = evI;
			i++;
		}
		else if( QString(argv[i]).toLower() == TOKEN_FILE )
		{
			event = evFi;
			i++;
		}
		else if( QString(argv[i]).toLower() == TOKEN_COMMAND )
		{
			event = evFi;
			i++;
		}
		
		if(( event != isUnknown ))
		{
			if( !ProcessFSM(event, argv[i]) )
			{
				if(m_skels) delete m_skels;
				return NULL;
			}
		}
		
	}
	FinishFSM();
	return m_skels;
}

