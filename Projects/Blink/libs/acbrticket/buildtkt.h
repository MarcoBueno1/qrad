#ifndef _BUILDTKT_H
#define _BUILDTKT_H

class BuildTkt
{
public:
      BuildTkt(){}
      virtual ~BuildTkt(){}
      
      static bool Init(int Id);
      static bool Print();
      static bool BuildPdfs();
      static bool BuildShipping(QString strDir, int nFileNumber, QString FileName );
      static bool AddTicket( CTicket *pTicket );
       

};


#endif
