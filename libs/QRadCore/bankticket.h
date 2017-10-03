#ifndef BANKTICKET_H
#define BANKTICKET_H

#include <QString>
#include <QDate>

typedef enum tp_operation
{
    tpRegistered,
    tpLiquidated,
    tpDeed,
    tpChanges,
    tpOther


}QRAD_BANKTKT_TYPEOP, *PQRAD_BANKTKT_TYPEOP;


class BankTicket
{
public:
    BankTicket( QString Cart,
                QString NossoNumero,
                QString SeuNumero,
                QString NomePagador,
                QDate dtVencto,
                bool pago,
                QDate dtPagto,
                QString Agencia,
                QString Valor,
                QString ValorPago,
                QRAD_BANKTKT_TYPEOP tpOp=tpOther);


    ~BankTicket();

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
    QRAD_BANKTKT_TYPEOP getTpOp();

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
    QRAD_BANKTKT_TYPEOP m_tpOp;

};

#endif // BANKTICKET_H
