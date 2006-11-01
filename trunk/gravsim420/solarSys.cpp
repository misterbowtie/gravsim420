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

	if(OPT) optimizeSystem();
	
	cam = poList.begin();
}

solarSys::~solarSys()
{
}
void solarSys::reset()
{
	age=0;
	poList.clear();
	planetObj *npo = new planetObj(smgr, driver);
    poList.push_back((*npo));
}

void solarSys::updatePhysics()
{
    age++;
	sumDist=0;
	poSize = poList.size();
    for (p1 = poList.begin(); p1 != poList.end(); p1++)
    {
        p2 = p1;
        p2++;
        for (; p2 != poList.end();)
        {
/*
			if((*p2).getPosition().getLength()>400)
			{	p2 = poList.erase(p2);
				if(OPT){reset();return;}
				continue;
			}
*/			
			r = (*p2).getPosition() - (*p1).getPosition();
			dist = r.getLength();
			sumDist += dist;
            if ((dist < (*p1).getSize()+(*p2).getSize()))
            {
                (*p1).join(&(*p2));
                p2 = poList.erase(p2);
            	continue;
			}
			else
			{
				force = r.normalize() * G * (*p1).getMass() * (*p2).getMass() / (pow(dist,2));
				(*p1).addForce(force);
				(*p2).addForce(-force);
				p2++;
			}
		}
    }

    for (p1 = poList.begin(); p1 != poList.end(); p1++)
    {
        (*p1).move();
    }

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

void solarSys::optimizeSystem()
{
	double *param[] = {&ev[0],&ev[1],&ev[2]};
	genePool gp(20,param,3);

	int score=0;
	
	int count=0;
	reset();
	while(1)
	{	
		updatePhysics();
		if (poList.size() < goalNumPlanets)
		{
			score = age-sumDist/poList.size()/poList.size();
			gp.score(score);
			
			if(age>5000000)
			{
				//gp.setBest();
				gp.print();
				reset();
				return;
			}
			reset();
			
			gp.nextTest();
		}
	}
}

#endif
