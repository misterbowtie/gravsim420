#ifndef SOLARSYS_CPP
#define SOLARSYS_CPP

#include <iostream>
#include <math.h>
#include "const.h"
#include <list>
#include <iterator>
#include "solarSys.h"

using namespace std;

solarSys::solarSys(ISceneManager *smgrz, IVideoDriver *driverz)
{
    smgr = smgrz;
    driver = driverz;

    planetObj *npo = new planetObj(smgr, driver);
    poList.push_back((*npo));   //i say bagglsy, s is very odd indeeed s?  we don't know their positions though...


//    planetObj *sun = new planetObj(smgr, driver, vector3df(0,-0.5f,0),vector3df(0,0.5f,0),vector3df(0,0,0), 999, 999);
//    planetObj *moon = new planetObj(smgr, driver, vector3df(120,0,0),vector3df(0,0.9f,0),vector3df(0,0,0), 0.1f, 1);
//    planetObj *earth = new planetObj(smgr, driver, vector3df(140,0,0),vector3df(0,1.2f,0),vector3df(0,0,0), 60, 60);
//    planetObj *mercury = new planetObj(smgr, driver, vector3df(30,0,0),vector3df(0,2.8f,0),vector3df(0,0,0), 0.01f, 1);
    //planetObj *venus = new planetObj(smgr, driver, vector3df(-60,0,0),vector3df(0,-1.5f,0),vector3df(0,0,0), 50, 50);

    /*    planetObj *center = new planetObj(smgr, driver, vector3df(000,400,200), vector3df(0,0,0), vector3df(0,0,0), 3000, 40);
        planetObj *left = new planetObj(smgr, driver, vector3df(23,23,17),vector3df(1,-1,1),vector3df(0.5,0,0), 250, 350);
        planetObj *right = new planetObj(smgr, driver, vector3df(11,-15,15),vector3df(-1,1,-1),vector3df(-0.5,0,0), 750, 500);

        /*
            planetObj *middle = new planetObj(smgr, driver, vector3df(2,-40,2),vector3df(0,2,0),vector3df(0,0,0), 50, 50);
            planetObj *small_1= new planetObj(smgr, driver, vector3df(16,-233,7),vector3df(0,2,1),vector3df(0,0,0),60, 72);
            planetObj *small_2= new planetObj(smgr, driver, vector3df(0,-300,0),vector3df(-1,-2,1),vector3df(0,0,0),20, 24);
            planetObj *small_3= new planetObj(smgr, driver, vector3df(0,200,0),vector3df(1,-2,-1),vector3df(0,0,0),25, 32);
            planetObj *med_1 = new planetObj(smgr, driver, vector3df(-100,0,0),vector3df(1,-2,-1),vector3df(0,0,0),200, 225);
            poList.push_back((*middle));
            poList.push_back((*small_1));
            poList.push_back((*small_2));
            poList.push_back((*small_3));
            poList.push_back((*med_1));

        poList.push_back((*center));
        poList.push_back((*left));
        poList.push_back((*right));


    //    poList.push_back((*sun));
    //    poList.push_back((*moon));
    //    poList.push_back((*earth));
    //    poList.push_back((*mercury));
        //poList.push_back((*venus));
    //*/
    cam = poList.begin();
    if (D)cout<<"\nSolar System Initialized";
}

solarSys::~solarSys()
{
    //remove elements??  what about manually removing ones created with split?
    //not neccessary?
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
vector3df solarSys::updatePhysics()
{
    int poSize = poList.size();
    if (D)cout << "\nUpdate Physics";
    for (p1 = poList.begin(); p1 != poList.end(); p1++)
    {
        p2 = p1;		//Hack??  need to do p2 = p1++ without breaking p1...so initial condition for the forloop is alread set
        p2++;
        for (; p2 != poList.end();)
        {
            if ((*p1).getMass()<=0 || (*p2).getMass()<=0){cout << " Oh God! "; continue;}

            vector3df r = (*p2).getPosition() - (*p1).getPosition();
            float dist = r.getLength();

            if ((dist < (*p1).getSize()) || (dist < (*p2).getSize()))  //Could this be dist < p1.size+p2.size ?
            {
                (*p1).join(&(*p2));  //Syntax is more or less correct,  strange results though
                p2 = poList.erase(p2);
            }
            else
            {
                vector3df force = r.normalize() * G * (*p1).getMass() * (*p2).getMass() / (dist * dist);
                (*p1).addForce(force);
                (*p2).addForce(-force);
                p2++;
            }
        }
    }

    vector3df centerM(0,0,0);

    for (p1 = poList.begin(); p1 != poList.end(); p1++)
    {
        (*p1).move();
        centerM += (*p1).getPosition()*(*p1).getMass();
    }

    if (poList.size() < 2 && randf()<0.01)
    {
        p1 = poList.begin();
        float m = (*p1).getMass()/numPieces;
        for (int p=0; p<numPieces-1; p++)
        {
            poList.push_back((*(*p1).split(m)));
            //THIS JUST SEEMS WRONG!
        }
        if (D) cout << "\nBIG BANG!!!!!";

    }
    //*/
    if (poSize!=poList.size()) // && D)
    {
        cout<<"\nPLANET LIST: " << poList.size();
        for (p1 = poList.begin(); p1 != poList.end(); p1++)
            (*p1).report();
        cout<<"\nCenter of Mass: "<<centerM.getLength();
        cout<<"\n-----------";


    }
    return centerM;
}

vector3df solarSys::getStarPos()
{
    return (*poList.begin()).getPosition();
}

#endif
