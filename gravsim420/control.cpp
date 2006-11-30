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
	ICameraSceneNode *cam;
	solarSys *sys;

public:
	
	void setup(ICameraSceneNode *camz, solarSys *sysz)
	{
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

		if (event.EventType == irr::EET_KEY_INPUT_EVENT&&
			!event.KeyInput.PressedDown)
		{
			switch(event.KeyInput.Key)
			{
			case KEY_KEY_I:
			case KEY_KEY_O:
				{
					printf("\nkey hit! %d",UpdatesPerFrame);
					UpdatesPerFrame += event.KeyInput.Key == KEY_KEY_I ? 1 : -1;
					if(UpdatesPerFrame<0) UpdatesPerFrame=0;
					if(UpdatesPerFrame>100) UpdatesPerFrame=100;
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
				return true;
			}
		}

		return false;
	}
};

#endif