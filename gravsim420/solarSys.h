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

    void reset();
	void resetMatt();
    void updatePhysics();
    planetObj* getStar();
	void printList();
	void optimizeSystem();
	int getPlanetCount();
	void spawnPlanet(vector3df position, vector3df direction);

    
private:
    ISceneManager *smgr;
    IVideoDriver *driver;

    std::list<planetObj> poList;
    std::list<planetObj>::iterator p1;
    std::list<planetObj>::iterator p2;
    
	planetObj* star;

	int age;
	double sumDist;
	int poSize;
	vector3df r;
	float dist;
	vector3df force;
};

#include "solarSys.cpp"

#endif
