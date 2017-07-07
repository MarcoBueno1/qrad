#ifndef PROJECTSKELETON_H
#define PROJECTSKELETON_H

#include "skeleton.h"

class projectskeleton
{
public:
    projectskeleton();
    ~projectskeleton();
    void Build(skeleton *skel);
private:
    void BuildPro(skeleton *skel);
    void BuildH(skeleton *skel);
    void BuildCPP(skeleton *skel);
    void BuildJson(skeleton *skel);
    void BuildXML(skeleton *skel);
};

#endif // PROJECTSKELETON_H
