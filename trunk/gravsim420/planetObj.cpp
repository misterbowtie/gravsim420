#ifndef PLANETOBJ_CPP
#define PLANETOBJ_CPP

#include <iostream>
#include <math.h>
#include "const.h"
#include "planetObj.h"

using namespace std;

planetObj::planetObj(ISceneManager *smgrz, IVideoDriver* driverz, vector3df p = vector3df(0,0,0), vector3df v = vector3df(0,0,0), vector3df r = vector3df(0,ROT,0), float m = 1, float s=1)
{
    smgr = smgrz;
    driver = driverz;

    rotationSpeed = r;
    position = p;
    velocity = v;
    force = vector3df(0,0,0);
    rotation = vector3df(0,0,0);
    mass = m;
    volume = s;

    //mesh = smgr->getMesh("media/earth.x");
    node = smgr->addSphereSceneNode(1,10);

	particle = smgr->addParticleSystemSceneNode(false,node);
	

    changeAttb();
}

planetObj::~planetObj()
{
	node->remove();
   	if (D){cout<<"\nDELETE PLANET.";report();}
}

void planetObj::report()
{
    cout << "\n M: " << mass << " S: " << volume << " V: " << velocity.getLength() << " P: " << position.getLength();
}

void planetObj::join(planetObj *p2)
{
    if (D){
        cout << "\nJoin:";
        report();
        (*p2).report();
    }
    mass += (*p2).mass;

    float p2scale = (*p2).mass / mass;
    float p1scale = 1.0 - p2scale;

    position = position * p1scale + (*p2).position * p2scale;
    velocity = velocity * p1scale + (*p2).velocity * p2scale;

    force += (*p2).force;

    volume += (*p2).volume;

	(*p2).createParticleExplosion();
	
	changeAttb();

	if (D){
        cout << "\nPlanet Joined";
    }
}

void planetObj::addForce(vector3df f)
{
    force += f;
}

void planetObj::move()
{
    a = force / mass;
    velocity += a * t;
    position += velocity * t;
    force.set(0,0,0);

	node->setPosition(vector3df(0,0,0));

    rotation-=rotationSpeed/size;
    node->setRotation(rotation);

    node->setPosition(position);
	
}

void planetObj::explode(std::list<planetObj> &poList)
{
    if (D){cout<<"\nExplode before: ";report();}
	
	std::list<planetObj>::iterator p1 = poList.end();
	vector3df centerG;

	for(int p=0; p<numPieces; p++)
	{	
		planetObj *npo = new planetObj(smgr, driver);

		float mscale = 1/numPieces;// * (1.0f +  randf()*.2-.1); // +/-10% error
		(*npo).mass = mscale * mass;
	    
		float sscale = mscale*(1.0f +  randf()*.4-.2); // +/-20% error
		(*npo).volume = volume * sscale;
	    
		(*npo).changeAttb();

		// make displacement vector
		vector3df offset;
		if(randf()<.8)
			offset.set(randf()-.5, randf()-.5, randf()-.5);
		else
			offset = -centerG;
		offset.setLength((.5+randf()*1.5)* size);

		centerG+=offset;

		// displace radius of orignal size
		(*npo).position = position + offset;

		// retain rotaional velocity, but linear
		(*npo).velocity = velocity + offset*OFORCE + offset.crossProduct(rotationSpeed);

		/*float display[4];
		if (D){	(offset * dist * (1-mscale)).getAs4Values(display);
			cout<<"\n offset: "<<display[0]<<","<<display[1]<<","<<display[2];
		}*/
		force -= (*npo).velocity/t;

		poList.push_back(*npo);
	}

	force/=numPieces;

}

void planetObj::changeAttb()
{
    size = pow(volume,.3333f);

	//node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);  //fixes lighting?

    node->setMaterialFlag(EMF_LIGHTING, false);
	//particle->setEmitter(0);

    if (mass>SystemMass*.1f)
    {	//star
        node->setMaterialTexture( 0, driver->getTexture("media/sun.jpg") );
		smgr->addLightSceneNode(node);  //if this is being executed for all nodes....then the lighting is fucked up too
		node->setMaterialFlag(video::EMF_LIGHTING, false);
		node->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);

		//particle = smgr->addParticleSystemSceneNode(false,node);
		particle->setScale(vector3df(2,2,2));
		particle->setParticleSize(dimension2d<f32>(8.0f, 8.0f));
		particle->setParticlesAreGlobal(false);  //makes it only create particles when changeattr is called
		IParticleEmitter* em = particle->createPointEmitter(vector3df(0.0f,size*.002f,0.0f),10,50,SColor(0,255,255,255), SColor(0,255,255,255), 800,850,180);
		//
		particle->setEmitter(em);
		em->drop();
		IParticleAffector* paf = particle->createFadeOutParticleAffector();
		particle->addAffector(paf);
		paf->drop();

		particle->setMaterialFlag(video::EMF_LIGHTING, false);
		particle->setMaterialTexture(0, driver->getTexture("media/fire.bmp"));
		particle->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
    }
    else 
	{
		if (mass/volume > 1.2)
		{	//rock
			node->setMaterialTexture( 0, driver->getTexture("media/dirt.jpg") );
		}
		else if(mass/volume > 1.1)
		{
			//redrock
			node->setMaterialTexture( 0, driver->getTexture("media/redrock.jpg") );
		}
		else if(mass/volume > 1)
		{
			//ice
			node->setMaterialTexture( 0, driver->getTexture("media/ice.jpg") );

			
		}
		else if(mass/volume > .9)
		{
			//gas
			node->setMaterialTexture( 0, driver->getTexture("media/gas.jpg"));
		}
		else
		{
			//planet
			node->setMaterialTexture( 0, driver->getTexture("media/earth.jpg"));
		}
		node->setMaterialFlag(video::EMF_LIGHTING, true);
	}
	node->setScale(vector3df(size,size,size));

}
vector3df planetObj::getRotation(){ return rotation;}
vector3df planetObj::getPosition(){ return position;}
vector3df planetObj::getVelocity(){ return velocity;}
vector3df planetObj::getForce(){ return force;}
float planetObj::getMass(){ return mass;}
float planetObj::getSize(){ return size;}
float planetObj::getVolume(){ return volume;}

void planetObj::createParticleExplosion()
{

	IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false);
	ps->setPosition((position));
	ps->setParticleSize(dimension2d<f32>(size, size));
	ps->setParticlesAreGlobal(false);  //makes it only create particles when changeattr is called
	IParticleEmitter* em = ps->createPointEmitter(velocity.normalize()/500,4000,5000,SColor(0,255,255,255), SColor(0,255,255,255), volume*800, volume*1000,110);
	ps->setEmitter(em);
	em->drop();
	IParticleAffector* paf = ps->createFadeOutParticleAffector();
	ps->addAffector(paf);
	paf->drop();

	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialTexture(0, driver->getTexture("media/fire.bmp"));
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	
	ISceneNodeAnimator *anim  = smgr->createDeleteAnimator(1000/UpdatesPerFrame);
	ps->addAnimator(anim);
}

#endif
