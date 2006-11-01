#include <irrlicht.h>
//#include <sys/time.h>		//Linux
 #include <time.h>			//Windows

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#pragma comment(lib, "Irrlicht.lib")

#include "solarSys.h"

int main()
{
	//timeval tim;
	//gettimeofday(&tim, NULL);
	//srand((unsigned int) tim.tv_usec);
    srand((unsigned int) time(0));

    video::E_DRIVER_TYPE driverType;

    printf("Please select the driver you want for this example:\n"\
           " (a) Direct3D 9.0c\n (b) Direct3D 8.1\n (c) OpenGL 1.5\n"\
           " (d) Software Renderer\n (e) Apfelbaum Software Renderer\n"\
           " (f) NullDevice\n (otherKey) exit\n\n");

    char i;
    //std::cin >> i;
    i='b';
    switch (i)
    {
    case 'a':
        driverType = video::EDT_DIRECT3D9;break;
    case 'b':
        driverType = video::EDT_DIRECT3D8;break;
    case 'c':
        driverType = video::EDT_OPENGL;   break;
    case 'd':
        driverType = video::EDT_SOFTWARE; break;
    case 'e':
        driverType = video::EDT_SOFTWARE2;break;
    case 'f':
        driverType = video::EDT_NULL;     break;
    default:
        return 1;
    }

    IrrlichtDevice *device =
        createDevice(driverType, core::dimension2d<s32>(640, 480));

    if (device == 0)
        return 1;

    device->setWindowCaption(L"GravSim");

    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();

    guienv->addStaticText(L"GravSim",
                          rect<int>(255, 255, 255, 0), true);

    ISceneNode* skyBoxNode = 0;
    skyBoxNode = smgr->addSkyBoxSceneNode(
                     driver->getTexture("media/backstars.jpg"),
                     driver->getTexture("media/backstars.jpg"),
                     driver->getTexture("media/backstars.jpg"),
                     driver->getTexture("media/backstars.jpg"),
                     driver->getTexture("media/backstars.jpg"),
                     driver->getTexture("media/backstars.jpg"));

	driver->setAmbientLight(video::SColor(0,60,60,60));
	
    ICameraSceneNode *cam = smgr->addCameraSceneNode(0, vector3df(0,-300,0), vector3df(0,0,0));
    //smgr->addCameraSceneNodeFPS(0, -100.0f, -100.0f);
    //device->getCursorControl()->setVisible(false);

    solarSys newsolar(smgr, driver);
	
    while (device->run())
    {
		while(1)
		{
			driver->beginScene(true, true, SColor(0, 0, 0, 0));
            smgr->drawAll();
            guienv->drawAll();
            driver->endScene();

            for (int j=0;j<UpdatesPerFrame;j++)
            {
                newsolar.updatePhysics();
            }

            //vector3df v = (*newsolar.cam).getPosition();
            //cam->setPosition(vector3df(300,0,0));
            //cam->setTarget(v);

			cam->setPosition(newsolar.getStarPos()-vector3df(75,75,75));
		    cam->setTarget(newsolar.getStarPos());
        }
    }

    device->drop();

    return 0;
}

