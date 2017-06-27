#include "bankticket.h"

BankTicket::BankTicket( QString Cart,
            QString NossoNumero,
            QString SeuNumero,
            QString NomePagador,
            QDate dtVencto,
            bool pago,
            QDate dtPagto,
            QString Agencia,
            QString Valor,
            QString ValorPago,
            QRAD_BANKTKT_TYPEOP tpOp)
{
    m_Cart = Cart;
    m_NossoNumero = NossoNumero;
    m_SeuNumero = SeuNumero;
    m_NomePagador = NomePagador;
    m_dtVencto = dtVencto;
    b_pago = pago;
    m_dtPagto = dtPagto;
    m_Agencia= Agencia;
    m_Valor =Valor;
    m_ValorPago =ValorPago;
    m_tpOp = tpOp;
}

QString BankTicket::getCart()
{
  return m_Cart;
}

QString BankTicket::getNossoNumero()
{
  return m_NossoNumero;
}

QString BankTicket::getSeuNumero()
{
  return m_SeuNumero;
}

QString BankTicket::getNomePagador()
{
    return m_NomePagador;

}

QDate BankTicket::getdtVencto()
{

    return m_dtVencto;
}

bool BankTicket::getpago()
{
   return b_pago;
}

QDate BankTicket::getdtPagto()
{
   return m_dtPagto;
}

QString BankTicket::getAgencia()
{
    return m_Agencia;

}

QString BankTicket::getValor()
{
   return m_Valor;
}

QString BankTicket::getValorPago()
{
   return m_ValorPago;
}

BankTicket::~BankTicket()
{

}

QRAD_BANKTKT_TYPEOP BankTicket::getTpOp()
{
    return m_tpOp;
}
