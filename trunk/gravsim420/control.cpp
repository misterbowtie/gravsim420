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
	IrrlichtDevice *device;
	bool mouseEnabled;
public:
	
	void setup(ISceneManager *smgrz, ICameraSceneNode *camz, solarSys *sysz, IrrlichtDevice *devicez)
	{
		smgr = smgrz;
		cam = camz;
		sys = sysz;
		device = devicez;
		mouseEnabled = false;
	}

	virtual bool OnEvent(SEvent event)
	{

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
						cam->setPosition(vector3df(0,0,0));
						cam->setTarget(sys->getStar()->getPosition());
						//type 2. may be cooler
						//cam->setPosition(selectedNode->getPosition() - vector3df(1,1,1));
						//cam->setTarget(selectedNode->getPosition());
					}
					break;
				}
			case EMIE_MOUSE_MOVED:
				{
					//event.MouseInput.X;
					//type 1. set camera back to root scene node
					vector3df pos = cam->getAbsolutePosition();
					cam->setParent(smgr->getRootSceneNode());
					cam->setPosition(pos);
					
					//type 2. do nothing
					break;
				}
			case EMIE_LMOUSE_PRESSED_DOWN:
				{
					sys->spawnPlanet(cam->getPosition(), (cam->getTarget()-cam->getPosition()).normalize()*15);
					break;
				}
			}
		}

		if (event.EventType == irr::EET_KEY_INPUT_EVENT&&!event.KeyInput.PressedDown)
		{
			switch(event.KeyInput.Key)
			{
			case KEY_KEY_F:
			case KEY_KEY_V:
				{
					vector3df v = cam->getPosition();
					v.Y += event.KeyInput.Key == KEY_KEY_F ? 2.0f : -2.0f;
					cam->setPosition(v);
					break;
				}

			case KEY_KEY_A:
			case KEY_KEY_D:
				{
					vector3df v = cam->getPosition();
					v.X += event.KeyInput.Key == KEY_KEY_D ? 2.0f : -2.0f;
					cam->setPosition(v);
					break;
				}
			case KEY_KEY_W:
			case KEY_KEY_S:
				{
					vector3df v = cam->getPosition();
					v.Z += event.KeyInput.Key == KEY_KEY_W ? 2.0f : -2.0f;
					cam->setPosition(v);
					break;
				}

			case KEY_KEY_I:
			case KEY_KEY_O:
				{
					UpdatesPerFrame += event.KeyInput.Key == KEY_KEY_I ? -1 : 1;
					if(UpdatesPerFrame<0) UpdatesPerFrame=0;
					else if(UpdatesPerFrame>100) UpdatesPerFrame=100;
					printf("\nUpdates: %d",UpdatesPerFrame);
					break;
				}
			case KEY_KEY_J:
			case KEY_KEY_K:
				{
					G += event.KeyInput.Key == KEY_KEY_J ? -0.025 : 0.025;
					if(G < -10) G = -10;
					else if(G > 10) G = 10;
					printf("\nGravity: %f", G);
					break;
				}
			case KEY_KEY_N:
			case KEY_KEY_M:
				{
					t += event.KeyInput.Key == KEY_KEY_N ? -0.001 : 0.001;
					if(t<0) t=0;
					else if(t>1) t=1;
					printf("\nTimeStep: %f",t);
					break;
				}
			case KEY_KEY_Z:
			case KEY_KEY_X:
				{
					numPieces += event.KeyInput.Key == KEY_KEY_Z ? -5 : 5;
					if(numPieces<50) numPieces=50;
					else if(numPieces>1000) numPieces=1000;
					printf("\nNumPieces: %f",numPieces);
					break;
				}
			case KEY_KEY_P:
				{
					UpdatesPerFrame = 0;
					printf("\nUpdates: %d",UpdatesPerFrame);
					break;
				}
			case KEY_KEY_E:
				{
					//planetObj* star = sys->getStar();  
					//cam->setParent(star->node);
					cam->setPosition(vector3df(35,35,35));
					cam->setTarget(sys->getStar()->getPosition());
					break;
				}
			case KEY_KEY_R:
				{
					sys->reset();
					break;
				}
			case KEY_KEY_T:
				{
					G=1;
					UpdatesPerFrame = 1;
					t=.05;
					break;
				}
			case KEY_KEY_Y:
				{
					cam->setPosition(vector3df(-35,-35,-35));
					cam->setTarget(vector3df(0,0,0));
					break;
				}
			case KEY_KEY_Q:
				{
					mouseEnabled = !mouseEnabled;
					if (mouseEnabled)
						device->getCursorControl()->drop();
					else
						device->getCursorControl()->setVisible(true);
					break;
				}
				return true;
			}

		}

		return false;
	}
};

#endif