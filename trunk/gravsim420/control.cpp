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
	ICameraSceneNode *fpsCam;
	ICameraSceneNode *normalCam;
	solarSys *sys;
	ISceneNode *selectedNode;
	IrrlichtDevice *device;
	bool mouseEnabled;
	vector3df currCamPos;
	vector3df currCamTarget;
public:
	
	void setup(ISceneManager *smgrz, ICameraSceneNode *fpsCamz, ICameraSceneNode *normalCamz, solarSys *sysz, IrrlichtDevice *devicez)
	{
		smgr = smgrz;
		fpsCam = fpsCamz;
		normalCam = normalCamz;
		sys = sysz;
		device = devicez;
		mouseEnabled = true;
		device->getCursorControl()->setVisible(false);
	}

	virtual bool OnEvent(SEvent event)
	{

		if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
		{
			switch(event.MouseInput.Event)
			{
			case EMIE_RMOUSE_PRESSED_DOWN:
				{
					ISceneNode *selectedNode = smgr->getSceneCollisionManager()->getSceneNodeFromCameraBB(fpsCam);

					if(selectedNode)
					{
						//type 1. changing parent nodes makes all kinds of control issues
						//fpsCam->setParent(selectedNode);
						//fpsCam->setPosition(vector3df(0,0,0));
						//fpsCam->setTarget(sys->getStar()->getPosition());
						//type 2. may be cooler
						currCamPos = fpsCam->getPosition();
						fpsCam->setPosition(selectedNode->getPosition());
						fpsCam->setTarget(currCamPos);
					}
					break;
				}
			case EMIE_MOUSE_MOVED:
				{
					//event.MouseInput.X;
					//type 1. set camera back to root scene node
					vector3df pos = fpsCam->getAbsolutePosition();
					fpsCam->setParent(smgr->getRootSceneNode());
					fpsCam->setPosition(pos);
					
					//type 2. do nothing
					break;
				}
			case EMIE_LMOUSE_PRESSED_DOWN:
				{
					sys->spawnPlanet(fpsCam->getPosition(), (fpsCam->getTarget()-fpsCam->getPosition()).normalize()*15);
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
					vector3df v = fpsCam->getPosition();
					v.Y += event.KeyInput.Key == KEY_KEY_F ? 2.0f : -2.0f;
					fpsCam->setPosition(v);
					break;
				}

			case KEY_KEY_A:
			case KEY_KEY_D:
				{
					vector3df v = fpsCam->getPosition();
					v.X += event.KeyInput.Key == KEY_KEY_D ? 2.0f : -2.0f;
					fpsCam->setPosition(v);
					break;
				}
			case KEY_KEY_W:
			case KEY_KEY_S:
				{
					vector3df v = fpsCam->getPosition();
					v.Z += event.KeyInput.Key == KEY_KEY_W ? 2.0f : -2.0f;
					fpsCam->setPosition(v);
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
					sys->resetMatt();
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
					fpsCam->setPosition(vector3df(-35,-35,-35));
					fpsCam->setTarget(vector3df(0,0,0));
					break;
				}
			case KEY_KEY_Q:
				{
					if (mouseEnabled)
					{
						currCamPos = fpsCam->getPosition();
						currCamTarget = fpsCam->getTarget();
						normalCam->setPosition(currCamPos);
						normalCam->setTarget(currCamTarget);
						smgr->setActiveCamera(normalCam);
						device->getCursorControl()->setVisible(true);
					}
					else
					{
						currCamPos = fpsCam->getPosition();
						currCamTarget = fpsCam->getTarget();
						smgr->setActiveCamera(fpsCam);
						device->getCursorControl()->setVisible(false);
					}
					mouseEnabled = !mouseEnabled;
					break;
				}
				return true;
			}

		}

		return false;
	}
};

#endif