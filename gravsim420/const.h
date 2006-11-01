#ifndef CONST_H
#define CONST_H

#define t 0.01f  //Physics time step
#define UpdatesPerFrame 50
#define G .8 // Gravity Const
#define D 0    // Debug
#define SystemMass 1000 // Net mass of system
#define numPieces 250.f ///220.0f
#define StarSize (ev[2]) // percent of SystemMass
#define TFORCE (ev[0])
#define OFORCE (ev[1])

//alg const
#define OPT 0 // on/off 
#define goalNumPlanets 7

double ev[] = {3226,4.08,.743};  ////{3615,3.45};


float randf();

#include "const.cpp"

#endif
