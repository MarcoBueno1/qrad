#include <QString>


class AutoRepo
{
 public:
      AutoRepo();
	  ~AutoRepo();
 
      static bool Build(QString FileName, QString Title, QString Sql);
};