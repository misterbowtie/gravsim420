#ifndef SOLARSYS_CPP
#define SOLARSYS_CPP

#include <iostream>
#include <math.h>
#include "const.h"
#include <list>
#include <iterator>
#include "solarSys.h"
#include "ARCgenepool.h"

using namespace std;

solarSys::solarSys(ISceneManager *smgrz, IVideoDriver *driverz)
{
    smgr = smgrz;
    driver = driverz;

	reset();

	cam = poList.begin();
}

solarSys::~solarSys()
{
}
void solarSys::reset()
{
	G=.37;
	age=0;
	poList.clear();
	planetObj *npo = new planetObj(smgr, driver);
    poList.push_back((*npo));
}

void solarSys::updatePhysics()
{
    age++;
//	sumDist=0;
	poSize = poList.size();
    for (p1 = poList.begin(); p1 != poList.end(); p1++)
    {
        p2 = p1;
        p2++;
        for (; p2 != poList.end();)
        {
			r = (*p2).getPosition() - (*p1).getPosition();
			dist = r.getLength();
//			sumDist += dist;
            if ((dist < (*p1).getSize()+(*p2).getSize()))
            {
                (*p1).join(&(*p2));
                p2 = poList.erase(p2);
            	continue;
			}
			else // apply forces
			{
				force = r.normalize() * G * (*p1).getMass() * (*p2).getMass() / (pow(dist,2));
				(*p1).addForce(force);
				(*p2).addForce(-force);
				p2++;
			}
		}
    }


    for (p1 = poList.begin(); p1 != poList.end();)
    {	// delete distant planets
		if((*p1).getPosition().getLength()>700)
		{	p1 = poList.erase(p1);
			continue;
		}
		else // move planets
		{
			(*p1).move();
			 p1++;
		}
    }

	// big bang
    if (poList.size() < 2)
    {
		p1 = poList.begin();
        float m = (*p1).getMass()/numPieces;
		while((*p1).getMass()>(SystemMass* StarSize))
        {
            poList.push_back((*(*p1).split(m)));
        }
    }

/*
    if (poSize!=poList.size())
	{
		if (D)
		{
			cout<<"\nPLANET LIST: " << poList.size();
			for (p1 = poList.begin(); p1 != poList.end(); p1++)
				(*p1).report();
			cout<<"\n-----------";
		}
    }
*/

}

vector3df solarSys::getStarPos()
{
    return (*poList.begin()).getPosition();
}

#endif
