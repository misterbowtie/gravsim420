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

	e = new double[4];
	e[0]= 412;
	e[1]= .05;
	e[2]= 1;
	e[3]= 9.09;
	numPieces=40;

	//optimizeSystem();
	
	cam = poList.begin();
    if (D)cout<<"\nSolar System Initialized";
}

solarSys::~solarSys()
{
    //remove elements??  what about manually removing ones created with split?
    //not neccessary?
}
void solarSys::reset()
{
	age=0;
	poList.clear();
	planetObj *npo = new planetObj(smgr, driver);
    poList.push_back((*npo));   //i say bagglsy, s is very odd indeeed s?  we don't know their positions though...
}
void solarSys::updateView()
{
    if (D)cout << "\nUpdate View";
    for (p1 = poList.begin(); p1 != poList.end(); p1++)
    {
        (*p1).updateView();
    }
    cam = poList.end();
    cam--;
}
void solarSys::updatePhysics()
{
    age++;
	int poSize = poList.size();
    if (D)cout << "\nUpdate Physics";
    for (p1 = poList.begin(); p1 != poList.end(); p1++)
    {
        p2 = p1;		//Hack??  need to do p2 = p1++ without breaking p1...so initial condition for the forloop is alread set
        p2++;
        for (; p2 != poList.end();)
        {
			if ((*p1).getMass()<=0 || (*p2).getMass()<=0)
			{
				cout << " Oh God! "; 
				continue;
			}

            // remove distance planets
			if((*p2).getPosition().getLength()>500)
			{	p2 = poList.erase(p2);
				reset();
				return;
				//continue;
			}
			
			vector3df r = (*p2).getPosition() - (*p1).getPosition();
			float dist = r.getLength();

			// merge touching planets
            if ((dist < (*p1).getSize()+(*p2).getSize()))  //Could this be dist < p1.size+p2.size ?
            {
                (*p1).join(&(*p2));  //Syntax is more or less correct,  strange results though
                p2 = poList.erase(p2);
            	continue;
			}
            
		
			vector3df force = r.normalize() * G * (*p1).getMass() * (*p2).getMass() / (dist * dist);
            (*p1).addForce(force);
            (*p2).addForce(-force);
            p2++;

		}
    }

    for (p1 = poList.begin(); p1 != poList.end(); p1++)
    {
        (*p1).move();
    }

    if (poList.size() < 2)
    {
        //cout<<"\nenergy: "<<e1<<" "<<e2;
		p1 = poList.begin();
        float m = (*p1).getMass()/numPieces/2;
		while((*p1).getMass()>SystemMass/2)
        {
            poList.push_back((*(*p1).split(m,e)));
            //THIS JUST SEEMS WRONG!
        }
        if (D) cout << "\nBIG BANG!!!!!";
    }
	
    if (poSize!=poList.size()) // && D)
    {	//cout<<"\nPLANET LIST: " << poList.size();
		if (D)
		{
			cout<<"\nPLANET LIST: " << poList.size();
			for (p1 = poList.begin(); p1 != poList.end(); p1++)
				(*p1).report();
			cout<<"\n-----------";
		}

    }
}

vector3df solarSys::getStarPos()
{
    return (*poList.begin()).getPosition();
}

void solarSys::optimizeSystem()
{
	double *param[] = {&e[0],&e[1],&e[2],&e[3]};
	genePool gp(10,param,4);

	gp.print();

	int count=0;
	reset();
	while(1)
	{	
		updatePhysics();
		if (poList.size() < 4)
		{
			gp.score(age);
			gp.nextTest();
			reset();
		}
		
	}
}

#endif
