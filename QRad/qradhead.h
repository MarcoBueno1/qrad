#ifndef QRADHEAD_H
#define QRADHEAD_H

#include "skeleton.h"

class QRadHead
{
public:
    QRadHead();
    ~QRadHead();
    void addSkel(skeleton *skel);
	
	int Run();
private:
    void Build();
    QList <skeleton *> m_skels;
};

#endif // QRADHEAD_H
