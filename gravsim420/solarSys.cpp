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
	sumDist=0;
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
			if((*p2).getPosition().getLength()>400)
			{	p2 = poList.erase(p2);
				if(OPT){reset();return;}
				continue;
			}
			
			vector3df r = (*p2).getPosition() - (*p1).getPosition();
			float dist = r.getLength();
			sumDist+=dist;
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
        float m = (*p1).getMass()/numPieces;
		while((*p1).getMass()>(SystemMass* StarSize))
        {
            poList.push_back((*(*p1).split(m)));
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
