#ifndef PLANETOBJ_CPP
#define PLANETOBJ_CPP

#include <iostream>
#include <math.h>
#include "const.h"
#include "planetObj.h"

using namespace std;

/*planetObj::planetObj(ISceneManager *smgrz, IVideoDriver* driverz)
{
    vector3df zero(0,0,0);
	planetObj(smgrz, driverz, zero, zero, zero, zero, SystemMass, SystemMass);

	if(node) cout<<"NODE CREATED";
}*/

planetObj::planetObj(ISceneManager *smgrz, IVideoDriver* driverz, vector3df p = vector3df(0,0,0), vector3df v = vector3df(0,0,0), vector3df r = vector3df(0,1,0), float m = SystemMass, float s=SystemMass)
{
    smgr = smgrz;
    driver = driverz;

    rotationSpeed = r;
    position = p;
    velocity = v;
    force = vector3df(0,0,0);
    rotation = vector3df(0,0,0);
    mass = m;
    volume = s;

    mesh = smgr->getMesh("media/earth.x");
    node = smgr->addAnimatedMeshSceneNode( mesh );

    changeAttb();
}

planetObj::~planetObj()
{
//remove from smgr
    node->remove();
    if (D){cout<<"\nDELETE PLANET.";report();}
}

void planetObj::report()
{
    cout << "\n M: " << mass << " S: " << volume << " V: " << velocity.getLength() << " P: " << position.getLength();
}

void planetObj::updateView()
{
    if (!node) cout<<"Oh shit";

    //center
    node->setPosition(vector3df(0,0,0));

    rotation+=rotationSpeed;
    node->setRotation(rotation);  // Annoying until fixed

    //translate node...
    node->setPosition(position);


}

void planetObj::join(planetObj *p2)
{
    if (D){
        cout << "\nJoin:";
        report();
        (*p2).report();
    }
    mass += (*p2).mass;

    float p2scale = (*p2).mass / mass;
    float p1scale = 1.0 - p2scale;

    position = position * p1scale + (*p2).position * p2scale;
    velocity = velocity * p1scale + (*p2).velocity * p2scale;

    force += (*p2).force;

    volume += (*p2).volume;

    changeAttb();

    if (D){
        cout << "\nPlanet Joined";
    }
}

void planetObj::addForce(vector3df f)
{
    force += f;
}

void planetObj::move()
{
    vector3df a;
    a = force / mass;
    velocity += a * t;
    position += velocity * t;
    force.set(0,0,0);
}

planetObj *planetObj::split(float scale, double e[])
{
    if (D){cout<<"\nSplit before: ";report();}

    planetObj *npo = new planetObj(smgr, driver);

    float mscale = scale / mass * (1.0f +  randf()*.2-.1); // 10% error
    (*npo).mass = mscale * mass;
    mass -= (*npo).mass;

    float sscale = mscale ;//* (1.0f +  randf()*.2-.1); // 10% error
    (*npo).volume = volume * sscale;
    volume -= (*npo).volume;

    changeAttb();
    (*npo).changeAttb();

    (*npo).velocity = velocity;

    // make displacement vector
    vector3df offset;
    offset.set(randf()-.5, randf()-.5, randf()-.5);
    offset.setLength(1.1);
    
    float dist = size+(*npo).size;
    (*npo).position = position + offset * dist * (1-mscale);
    position = position - offset * dist * mscale;

    float display[4];
    if (D){	(offset * dist * (1-mscale)).getAs4Values(display);
        cout<<"\n offset: "<<display[0]<<","<<display[1]<<","<<display[2];
    }

	vector3df tangent = offset.crossProduct(rotationSpeed); // tangent, direction to travel
	tangent.setLength(e[0]+e[1]*mass);
	offset.setLength(e[2]+e[3]*mass);
    (*npo).force = force + offset + tangent;
    force = force - offset - tangent;

    if (D){	force.getAs4Values(display);
        cout<<"\n force: "<<display[0]<<","<<display[1]<<","<<display[2];
    }

    (*npo).move();
    move();

    if (D)
    {
        cout << "\n after split";
        report();
        (*npo).report();
    }

    return npo;
}

void planetObj::changeAttb()
{
    size = pow(volume,.3333f);

    node->setMaterialFlag(EMF_LIGHTING, false);

    if (mass>SystemMass*.6f)
    {	//star
        node->setMaterialTexture( 0, driver->getTexture("media/sun.jpg") );
    }
    else if (mass<SystemMass*.05f)
    {	//rock
        node->setMaterialTexture( 0, driver->getTexture("media/dirt.jpg") );
    }
    else{
        //planet
        node->setMaterialTexture( 0, driver->getTexture("media/earth.jpg") );
    }

    node->setScale(vector3df(size,size,size));

}

vector3df planetObj::getRotation(){ return rotation;}
vector3df planetObj::getPosition(){ return position;}
vector3df planetObj::getVelocity(){ return velocity;}
vector3df planetObj::getForce(){ return force;}
float planetObj::getMass(){ return mass;}
float planetObj::getSize(){ return size;}
float planetObj::getVolume(){ return volume;}

//void planetObj::setRotation(vector3df rotz){ rotation = rotz;}
//void planetObj::setPosition(vector3df posz){ position = posz;}
//void planetObj::setVelocity(vector3df velz){ velocity = velz;}
//void planetObj::setForce(vector3df forcez){ force = forcez;}
//void planetObj::setMass(float massz){ mass = massz;}
//void planetObj::setSize(float sizez){ size = sizez;}
//void planetObj::setVolume(float volz){ volume = volz;}

#endif
