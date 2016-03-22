#ifndef HSKELETON_H
#define HSKELETON_H
#include "skeleton.h"

class hskeleton
{
public:
    hskeleton();
    ~hskeleton();
    void Build(skeleton *Skel);
    void IgnoreIfExist(bool );
private:
    bool m_IgnoreIfExist;


};

#endif // HSKELETON_H
