#ifndef PATHMAKER_H
#define PATHMAKER_H

#include <QString>

class PathMaker
{
public:
    PathMaker();
    ~PathMaker();
    static QString Path();
    static void setDir(QString strDir );
private:
    static QString m_child;
};

#endif // PATHMAKER_H
