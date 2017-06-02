#ifndef ITAUTICKET_H
#define ITAUTICKET_H

#include <QString>
#include <QDate>

class itauticket
{
public:
    itauticket( QString Cart,
                QString NossoNumero,
                QString SeuNumero,
                QString NomePagador,
                QDate dtVencto,
                bool pago,
                QDate dtPagto,
                QString Agencia,
                QString Valor,
                QString ValorPago );


    ~itauticket();

    QString getCart();
    QString getNossoNumero();
    QString getSeuNumero();
    QString getNomePagador();
    QDate getdtVencto();
    bool getpago();
    QDate getdtPagto();
    QString getAgencia();
    QString getValor();
    QString getValorPago();

private:
    QString m_Cart;
    QString m_NossoNumero;
    QString m_SeuNumero;
    QString m_NomePagador;
    QDate m_dtVencto;
    bool b_pago;
    QDate m_dtPagto;
    QString m_Agencia;
    QString m_Valor;
    QString m_ValorPago;

};

#endif // ITAUTICKET_H
