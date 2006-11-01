#ifndef CONST_H
#define CONST_H

#define t 0.01f				//Physics time step
#define UpdatesPerFrame 50
#define G .37				//Gravity Const
#define D 0					//Debug
#define SystemMass 1000		//Net mass of system
#define numPieces 250.f		//
#define StarSize (ev[2])	//Percent of SystemMass
#define TFORCE (ev[0])
#define OFORCE (ev[1])

//alg const
#define OPT 0 // on/off 
#define goalNumPlanets 7

double ev[] = {2000,4,.43};  ////{3615,3.45};

float randf();

#include "const.cpp"

#endif
