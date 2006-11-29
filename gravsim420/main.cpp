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
    
	i='d';

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

    //ISceneNode* skyBoxNode = smgr->addSkyDomeSceneNode(
      //               driver->getTexture("media/backstars.jpg"),16,16,1,2);
	
	driver->setAmbientLight(video::SColor(0,10,10,10));
	
    ICameraSceneNode *cam = smgr->addCameraSceneNode(0,vector3df(-50,50,-150), vector3df(0,0,0));

    //smgr->addCameraSceneNodeFPS(0,100.0f, -200.0f, 500.0f);
    //device->getCursorControl()->setVisible(false);


	/*ISceneNode *node = smgr->addSphereSceneNode();
	ISceneNodeAnimator * anim =  smgr->createFlyCircleAnimator(vector3df(0,0,0), 10,.001);
	node->addAnimator( anim);
	
	scene::IParticleSystemSceneNode* ps = 0;
	ps = smgr->addParticleSystemSceneNode(false,node);
	ps->setPosition(core::vector3df(0,0,0));
	ps->setScale(core::vector3df(2,2,2));
	ps->setParticleSize(core::dimension2d<f32>(12.0f, 12.0f));
	ps->setParticlesAreGlobal(false);
	//ps->addAnimator(anim);
	anim->drop();
	scene::IParticleEmitter* em = ps->createPointEmitter(
	   core::vector3df(0.0f,.01f,0.0f),
	   20,50,
	   video::SColor(0,255,255,255), video::SColor(0,255,255,255),
	   800,2000,180);
	ps->setEmitter(em);
	em->drop();
	scene::IParticleAffector* paf =
	   ps->createFadeOutParticleAffector();

	ps->addAffector(paf);
	paf->drop();
	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialTexture(0, driver->getTexture("media/fire.bmp"));
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
*/



	solarSys newSolar(smgr, driver);
	int lastTime=0;
	vector3df largestStarPos;
	while (device->run())
    {
		//while(driver->getFPS() > 30);
		//while(clock()-lastTime < ((float)CLK_TCK)/FramesPerSecond ); // prevent super speed
		lastTime = clock();

		driver->beginScene(true, true, SColor(0, 0, 0, 0));
        smgr->drawAll();
        //guienv->drawAll();
        driver->endScene();

        for (int j=0;j<=UpdatesPerFrame;j++)  //see what happens when you comment this out on alienware
        {
            newSolar.updatePhysics();
		}
		//largestStarPos = newSolar.getStarPos();  //Move camera out of loop for another cool view
		//cam->setPosition(largestStarPos-vector3df(35,35,35));
		//cam->setTarget(largestStarPos);
        
    }

    device->drop();

    return 0;
}

