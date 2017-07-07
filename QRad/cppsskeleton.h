#ifndef CPPSSKELETON_H
#define CPPSSKELETON_H

#include "skeleton.h"


class cppsskeleton
{
public:
    cppsskeleton();
    ~cppsskeleton();
    void Build(skeleton *Skel);
    void IgnoreIfExist(bool );
private:
    bool m_IgnoreIfExist;

};

#endif // CPPSSKELETON_H
