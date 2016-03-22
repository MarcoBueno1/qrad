#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include "type.h"
#include "field.h"
#include "table.h"
#include "skeleton.h"

#define TOKEN_FIELD_HIDE       ":hide"
#define TOKEN_FIELD_NOMODEL    ":nomodel"

#define TOKEN_TYPE_MULTI          ":multi"
#define TOKEN_TYPE_MULTI_EDTFULL  ":multiedtfull"
#define TOKEN_TYPE_MULTI_N_X_n    ":multinxn"
#define TOKEN_TYPE_SEARCH         ":search"
#define TOKEN_TYPE_SEARCH_MASTER  ":searchmaster"


typedef  void (*QRAD_ACTION) (char *);

typedef enum
{
    isIdle,
	isWaitF,
    isWaitT,
	isTakedFi,
    isUnchanged
}QRAD_STATE, *PQRAD_STATE;


typedef enum
{
    evS,
	evT,
    evF,
    evI,
    evFi,
    evC,
    isUnknown
}QRAD_EVENT, *PQRAD_EVENT;


typedef struct st_node
{
    QRAD_STATE State;
    QRAD_EVENT Event;
    QRAD_STATE nextState;
    QRAD_ACTION Action;
}QRAD_MACHINE_NODE, *PQRAD_MACHINE_NODE;


class parser
{
public:
    parser();
    ~parser();
    static CType  *ParseType( QString Token );
    static CField *ParseField( QString Token );
    static CTable *parseTable( QString Token );
	static skeleton * parseSkel(QString Token);
    static QList<skeleton*> * parseLine(int argc, char *argv[]);
private:
    static bool m_error;
    static QRAD_STATE m_state;
	static QList<skeleton*> *m_skels;
    static QList<QRAD_MACHINE_NODE *> *m_machine;
    static void printInvalid( char *arg );
    static void InitFSM();
	static void FinishFSM();
	static void do_StoreS(char *szData );
	static void do_StoreT(char *szData );
	static void do_StoreFi(char *szData );
	static void do_StoreF(char *szData );
	static void do_StoreI(char *szData );
	static void do_StoreC(char *szData );
    static int ProcessFSM(QRAD_EVENT event, char *szData);
    static void AddLine( QRAD_STATE State, QRAD_EVENT Event, QRAD_STATE nextState, QRAD_ACTION Action );
};

#endif // PARSER_H
