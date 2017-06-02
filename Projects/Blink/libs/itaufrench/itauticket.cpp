#include "itauticket.h"

itauticket::itauticket( QString Cart,
            QString NossoNumero,
            QString SeuNumero,
            QString NomePagador,
            QDate dtVencto,
            bool pago,
            QDate dtPagto,
            QString Agencia,
            QString Valor,
            QString ValorPago )
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
}

QString itauticket::getCart()
{
  return m_Cart;
}

QString itauticket::getNossoNumero()
{
  return m_NossoNumero;
}

QString itauticket::getSeuNumero()
{
  return m_SeuNumero;
}

QString itauticket::getNomePagador()
{
    return m_NomePagador;

}

QDate itauticket::getdtVencto()
{

    return m_dtVencto;
}

bool itauticket::getpago()
{
   return b_pago;
}

QDate itauticket::getdtPagto()
{
   return m_dtPagto;
}

QString itauticket::getAgencia()
{
    return m_Agencia;

}

QString itauticket::getValor()
{
   return m_Valor;
}

QString itauticket::getValorPago()
{
   return m_ValorPago;
}

itauticket::~itauticket()
{

}

