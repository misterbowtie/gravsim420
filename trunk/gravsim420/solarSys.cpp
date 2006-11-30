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

}

solarSys::~solarSys()
{
}
void solarSys::reset()
{
	//G=.37;
	age=0;
	poList.clear();
	planetObj *npo = new planetObj(smgr, driver,vector3df(),vector3df(),vector3df(0,ROT,0),SystemMass, SystemMass);
    poList.push_back((*npo));
}

void solarSys::updatePhysics()
{
    age++;
	
	poSize = poList.size();
	p1 = poList.begin();
	if(star==NULL)
		star=&(*p1);
    for (p1 = poList.begin(); p1 != poList.end(); p1++)
    {
        p2 = p1;
        p2++;
        for (; p2 != poList.end();)
        {
			r = (*p2).getPosition() - (*p1).getPosition();
			dist = r.getLength();
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
		if(p1->getMass() > star->getMass()) star=&(*p1);
    }


    for (p1 = poList.begin(); p1 != poList.end();)
    {	// delete distant planets
		if( (*p1).getPosition().getDistanceFrom(star->getPosition()) > 1200)
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
    if (poList.size() == 1)
    {
		reset();
		//printList();
		//cout<<"\nBig Bang\n-------------";
		p1 = poList.begin();
        (*p1).explode(poList);
		poList.erase(p1);
		//printList();
    }


    if (poSize!=poList.size())
	{
		if(!OPT) poList.sort();
		if(D) printList();
	}
	
}

planetObj* solarSys::getStar()
{
	return star;
}

void solarSys::printList()
{
	cout<<"\nPLANET LIST: " << poList.size();
	for (p1 = poList.begin(); p1 != poList.end(); p1++)
		(*p1).report();
	cout<<"\n-----------";
}

void solarSys::optimizeSystem()
{
	double *param[] = {&ROT,&OFORCE};
	genePool gp(30,param,sizeof(param)/sizeof(param[0]));

	double score=0;
	int goalNumPlanets=110;
	int maxage = 50000;
	int count=0;
	reset();
	time_t start;
	time(&start);
	cout<<"\nOptimizing Varibles";
	while(1)
	{	
		updatePhysics();
		if (poList.size() < goalNumPlanets || (time(0) - start)>60)
		{
			//score = age*age;
			gp.score(age);
			
			if(age>maxage)
			{
				if(goalNumPlanets<15)
					goalNumPlanets++;
				maxage=age;
			}
			reset();
			time(&start);
			cout<<goalNumPlanets;
			gp.nextTest();
		}
	}
}



#endif
