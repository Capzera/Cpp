#ifndef GLOBALUTILS_H
#define GLOBALUTILS_H

#include <QObject>
#include <QGlobalStatic>
#include <bits/stdc++.h>

class Globalutils{
public:
    Globalutils();
    static int getRandom(uint seed, int limit);
};

#endif // GLOBALUTILS_H
