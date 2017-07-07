#ifndef CONTROLLERSKELETON_H
#define CONTROLLERSKELETON_H

#include "skeleton.h"

class controllerskeleton
{
public:
    controllerskeleton();
    ~controllerskeleton();

    void Build(skeleton *Skel);

private:
    void BuildH(skeleton *Skel);
    void BuildCpp(skeleton *Skel);
};

#endif // CONTROLLERSKELETON_H
