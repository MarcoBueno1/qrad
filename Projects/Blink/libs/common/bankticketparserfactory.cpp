#include "bankticketparserfactory.h"
#include "itauinterface.h"
#include "buildtkt.h"

BankTicketParserFactory::BankTicketParserFactory()
{

}

BankTicketParserFactory::~BankTicketParserFactory()
{

}

IBankTicketParser *BankTicketParserFactory::GetParser(QString filename)
{
    IBankTicketParser *Int = NULL;

    if(filename.toUpper().endsWith(".PDF"))
    {
        Int = new ItauInterface(filename);
    }
    if(filename.toUpper().endsWith(".RET"))
    {
        Int = new BuildTkt(filename);
    }
    return Int;
}
