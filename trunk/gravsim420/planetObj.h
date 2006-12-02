#ifndef PLANETOBJ_H
#define PLANETOBJ_H

#include <iostream>
#include <math.h>
#include "const.h"
#include <list>
#include <iterator>

#include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#pragma comment(lib, "Irrlicht.lib")


using namespace std;

class planetObj
{
public:
    planetObj(ISceneManager *smgrz, IVideoDriver* driverz,  vector3df p, vector3df v, vector3df r, float m, float s);
    ~planetObj();
    void join(planetObj *p2);
    void addForce(vector3df f);
    void move();
    void report();
    void changeAttb();
    void explode(std::list<planetObj> &poList);

    vector3df getRotation();
    vector3df getPosition();
    vector3df getVelocity();
    vector3df getForce();
    float getMass();
    float getSize();
    float getVolume();
	bool operator<(const planetObj &po){ return (mass>po.mass);}

    void createParticleExplosion();


    ISceneNode *node;

private:
    ISceneManager *smgr;
    //IAnimatedMesh *mesh;
    IVideoDriver *driver;
	IParticleSystemSceneNode *particle;  //Particle Node

    vector3df rotationSpeed;
    vector3df rotation;
    vector3df position;
    vector3df velocity;

    vector3df force;
    float mass;
    float volume;
    float size;
	vector3df a;
};



#include "planetObj.cpp"

#endif
