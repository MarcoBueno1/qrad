#ifndef M2PHI_H
#define M2PHI_H

#include <QString>

#define INVALID_HDSERIAL    "Problema"

class M2phi
{
public:
    static QString GetHDSerial();
    
private:
    static QString ReadHDSerial();
    static QString GetMacAddress();
};

#endif // M2PHI_H
