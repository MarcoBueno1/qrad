#include "bankticketparserfactory.h"
#include "itauinterface.h"

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
    return Int;
}
