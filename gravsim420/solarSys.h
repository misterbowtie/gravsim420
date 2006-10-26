#ifndef SOLARSYS_H
#define SOLARSYS_H

#include <iostream>
#include <math.h>
#include "const.h"
#include "planetObj.h"
#include <list>
#include <iterator>

using namespace std;

#include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#pragma comment(lib, "Irrlicht.lib")

class solarSys
{
public:
    solarSys(ISceneManager *smgrz, IVideoDriver *driverz);
    ~solarSys();

    void updateView();
    vector3df updatePhysics();
    vector3df getStarPos();

    std::list<planetObj>::iterator cam;

private:
    ISceneManager *smgr;
    IVideoDriver *driver;
    std::list<planetObj> poList;
    std::list<planetObj>::iterator p1;
    std::list<planetObj>::iterator p2;

};

#include "solarSys.cpp"

#endif