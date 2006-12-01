#ifndef INPUT_CPP
#define INPUT_CPP

#include <irrlicht.h>
#include "solarSys.h"


using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#pragma comment(lib, "Irrlicht.lib")

class MyEventReceiver : public IEventReceiver
{
private:
	ISceneManager *smgr;
	ICameraSceneNode *cam;
	solarSys *sys;
	ISceneNode *selectedNode;
public:
	
	void setup(ISceneManager *smgrz, ICameraSceneNode *camz, solarSys *sysz)
	{
		smgr = smgrz;
		cam = camz;
		sys = sysz;
	}

	virtual bool OnEvent(SEvent event)
	{
		/*
		If the key 'W' or 'S' was left up, we get the position of the scene node,
		and modify the Y coordinate a little bit. So if you press 'W', the node
		moves up, and if you press 'S' it moves down.
		*/

		if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
		{
			switch(event.MouseInput.Event)
			{
			case EMIE_RMOUSE_PRESSED_DOWN:
				{
					ISceneNode *selectedNode = smgr->getSceneCollisionManager()->getSceneNodeFromCameraBB(cam);

					if(selectedNode)
					{
						//type 1. changing parent nodes makes all kinds of control issues
						cam->setParent(selectedNode);
						cam->setPosition(vector3df(1,1,1));
						cam->setTarget(vector3df(0,0,0));
						//type 2. may be cooler
						//cam->setPosition(selectedNode->getPosition() - vector3df(1,1,1));
						//cam->setTarget(selectedNode->getPosition());
					}
					break;
				}
			case EMIE_MOUSE_MOVED:
				{
					//type 1. set camera back to root scene node
					cam->setParent(smgr->getRootSceneNode());
					
					//type 2. do nothing
					break;
				}
			case EMIE_LMOUSE_PRESSED_DOWN:
				{
					sys->spawnPlanet(cam->getPosition(), -cam->getTarget().normalize()*5);
					break;
				}
			}
		}

		if (event.EventType == irr::EET_KEY_INPUT_EVENT&&!event.KeyInput.PressedDown)
		{
			switch(event.KeyInput.Key)
			{
			case KEY_KEY_I:
			case KEY_KEY_O:
				{
					printf("\nUpdates: %d",UpdatesPerFrame);
					UpdatesPerFrame += event.KeyInput.Key == KEY_KEY_I ? 1 : -1;
					if(UpdatesPerFrame<0) UpdatesPerFrame=0;
					else if(UpdatesPerFrame>100) UpdatesPerFrame=100;
					break;
				}
			case KEY_KEY_K:
			case KEY_KEY_L:
				{
					printf("\nGravity: %f",G);
					G += event.KeyInput.Key == KEY_KEY_K ? 0.05 : -0.05;
					if(G<0) G=0;
					else if(G>5) G=5;
					break;
				}

			case KEY_KEY_P:
				{
					UpdatesPerFrame =0;
					break;
				}
			case KEY_KEY_D:
				{
					planetObj* star = sys->getStar();  
					cam->setParent(star->node);
					//cam->setPosition(vector3df(75,75,75));
					//cam->setTarget(vector3df(0,0,0));
					break;
				}
			case KEY_KEY_R:
				{
					sys->reset();
					break;
				}
				return true;
			}
		}

		return false;
	}
};

#endif