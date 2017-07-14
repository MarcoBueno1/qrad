#include "ticket.h"
#include "accounttoreceivehistorymodel.h"
#include "accounttoreceivemodel.h"


ORM_BEGIN_MAP(ticket)
     ORM_MAP_PRIMARYKEY(id, "id");
     ORM_MAP_FIELD(NossoNumero, "nossonumero");
     ORM_MAP_FIELD(SeuNumero, "seunumero");
     ORM_MAP_FIELD(clientid, "clientid");
     ORM_MAP_FIELD(Vencto, "vencto");
     ORM_MAP_FIELD(Valor, "valor");
     ORM_MAP_FIELD(PagoEm, "pagoem");
     ORM_MAP_FIELD(ValorPago, "valorpago");
     ORM_MAP_FIELD(idticket, "idticket");
     ORM_MAP_FIELD(Status, "status");
     ORM_MAP_FIELD(Type, "type");
     ORM_MAP_FIELD(Obs, "obs");
     ORM_MAP_FIELD(User, "vuser");
     ORM_MAP_FIELD(AccountId, "accountid");
     ORM_MAP_FIELD(SendStatus, "sendstatus");
     ORM_MAP_FIELD(LoId, "loid");
     ORM_MAP_FIELD(Removed, "removed");
     ORM_MAP_FIELD(ExtraTxId, "extratxid");
     ORM_MAP_FIELD(Discount, "discount");
ORM_END_MAP()


int ticket::saveFile(QString path)
{
    int loid = ORM::saveFile(path);
    setLoId(loid);
    return loid;
}

void ticket::getFile(QString path, int Loid)
{
    ORM::getFile(path, Loid);
}

bool ticket::UpdateToPaid()
{
    QCoreApplication *app = QCoreApplication::instance();
    int Userid = app->property("CurrentUserId").toInt();

   AccountToReceiveModel *account = AccountToReceiveModel::findById(getAccountId(),true);
   if( account )
   {
       if(account->updatePaid(true))
       {
           AccountToReceiveHistoryModel *his = new AccountToReceiveHistoryModel;
           his->setDate(QDate::currentDate());
           his->setTime(QTime::currentTime());
           his->setTypeOperation(AccountOpPaid);
           his->setUserId(Userid);
           his->Save();
           delete his;
           setStatus(stPaid);
           updateStatus(stPaid);
           delete account;
           return true;
       }
   }

   return false;
}
