#ifndef UISKELETON_H
#define UISKELETON_H

#include "skeleton.h"

class uiskeleton
{
public:
    uiskeleton();
    ~uiskeleton();
    void Build(skeleton *skel);
    void IgnoreIfExist(bool );
private:
    bool m_IgnoreIfExist;


};

#endif // UISKELETON_H
