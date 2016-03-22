#ifndef _MANAGERSKELETON_H
#define _MANAGERSKELETON_H
#include "skeleton.h"

class ManagerSkeleton
{
public:
    ManagerSkeleton();
    ~ManagerSkeleton();
    void Build(skeleton *skel);
private:
    void BuildH(skeleton *skel);
    void BuildCPP(skeleton *skel);
    void BuildUI(skeleton *skel);
    QString getDelegateFor(int nCol,CType *tp);
};

#endif // MANAGERSKELETON_H
