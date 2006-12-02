#include <irrlicht.h>
//#include <sys/time.h>		//Linux
 #include <time.h>			//Windows
#include "control.cpp"
 
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
 
	MyEventReceiver receiver;
 
    video::E_DRIVER_TYPE driverType;
 
    printf("Please select the driver you want for this example:\n"\
           " (a) Direct3D 9.0c\n (b) Direct3D 8.1\n (c) OpenGL 1.5\n"\
           " (d) Software Renderer\n (e) Apfelbaum Software Renderer\n"\
           " (f) NullDevice\n (otherKey) exit\n\n");
 
    char i;
    //std::cin >> i;
 
	i='c';
	switch (i)
    {
	    case 'a': driverType = video::EDT_DIRECT3D9;break;
		case 'b': driverType = video::EDT_DIRECT3D8;break;
		case 'c': driverType = video::EDT_OPENGL;   break;
		case 'd': driverType = video::EDT_SOFTWARE; break;
		case 'e': driverType = video::EDT_SOFTWARE2;break;
		case 'f': driverType = video::EDT_NULL;     break;
		default:  return 1;
    }
 
    IrrlichtDevice *device =
        createDevice(driverType, core::dimension2d<s32>(640, 480), 16,false,false,true,&receiver);
    if (device == 0)
        return 1;
	device->setResizeAble(true);
    device->setWindowCaption(L"GravSim");
 
    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    guienv->addStaticText(L"GravSim",
                          rect<int>(255, 255, 255, 0), true);
 
    //ISceneNode* skyBoxNode = smgr->addSkyDomeSceneNode(driver->getTexture("media/backstars.jpg"),16,16,1,2);
 
	driver->setAmbientLight(video::SColor(0,50,50,50));
 
	ICameraSceneNode *normalCam = smgr->addCameraSceneNode(0,vector3df(-50,50,-150),vector3df(0,0,0));
	ICameraSceneNode *fpsCam = smgr->addCameraSceneNodeFPS(0,100.0f, -200.0f, 500.0f);
    //device->getCursorControl()->setVisible(true);
	solarSys newSolar(smgr, driver);
 
	receiver.setup(smgr, fpsCam, normalCam, &newSolar, device);
 
	int lastTime=0;
	vector3df largestStarPos;
	stringw strStats, strControls;
	vector3df campos, camtarget;
 
	strControls = L"Controls";
	strControls = L"\nGravity (J/K)";
	strControls += L"\nUpdates (I/O/P)";
	strControls += L"\nTimeStep (N/M)";
	strControls += L"\nNumPieces (Z/X)";
	strControls += L"\nResetCamera (E)";
	strControls += L"\nResetSystem (R)";
	strControls += L"\nResetDefault (T)";
	strControls += L"\nOriginCam (Y)";
	strControls += L"\nToggleMouse (Q)";
	IGUIStaticText *controls = guienv->addStaticText(strControls.c_str(), rect<int>(10,200,120,320), true,true,0,-1,false);
	controls->setOverrideColor(SColor(0,255,255,255));
	controls->enableOverrideColor(true);
	IGUIStaticText *stats = guienv->addStaticText(strStats.c_str(), rect<int>(10,10,120,120), true,true,0,-1,false);
	stats->setOverrideColor(SColor(0,255,255,255));
	controls->enableOverrideColor(true);
 
	while (device->run())
    {
		//while(driver->getFPS() > 30);
		while(clock()-lastTime < ((float)CLK_TCK)/FramesPerSecond ); // prevent super speed
		lastTime = clock();
 
		driver->beginScene(true, true, SColor(0, 0, 0, 0));
        smgr->drawAll();
        guienv->drawAll();
        driver->endScene();
 
        for (int j=0;j<UpdatesPerFrame;j++)  //see what happens when you comment this out on alienware
        {
            newSolar.updatePhysics();
 
		}
 
		if(clock()-lastTime < ((float)CLK_TCK)/FramesPerSecond)
		{
			strStats = L"Gravity: ";
			strStats += G;
			strStats += L"\nUpdates: ";
			strStats += UpdatesPerFrame;
			strStats += L"\nTimeStep: ";
			strStats += t;
			strStats += L"\nPlanets: ";
			strStats += newSolar.getPlanetCount();
			strStats += L"\nNumPieces: ";
			strStats += numPieces;
			campos = fpsCam->getPosition();
			strStats += L"\nX: ";
			strStats += campos.X;
			strStats += L"\nY: ";
			strStats += campos.Y;
			strStats += L"\nZ: ";
			strStats += campos.Z;
			camtarget = fpsCam->getTarget();
			strStats += L"\ntX: ";
			strStats += camtarget.X;
			strStats += L"\ntY: ";
			strStats += camtarget.Y;
			strStats += L"\ntZ: ";
			strStats += camtarget.Z;
			stats->setText(strStats.c_str());
		}
	}
 
    device->drop();
 
    return 0;
}

