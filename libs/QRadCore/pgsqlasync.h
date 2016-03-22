#ifndef _PGSQLAsync_
#define _PGSQLAsync_

#include <QString>


class PGSQLAsync
{
public:

    PGSQLAsync();

    static unsigned int Send( QString strFilePath,
                              QString strstrHostName,
                              QString strdatabasename,
                              QString strusername,
                              QString strpassword);

    static void Receive( unsigned int loid,
                         QString strFilePath,
                         QString strstrHostName,
                         QString strdatabasename,
                         QString strusername,
                         QString strpassword );
};

#endif //
