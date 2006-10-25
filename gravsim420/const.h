#ifndef CONST_H
#define CONST_H

#define t 0.05f  //Physics time step
#define UpdatesPerFrame 30
#define G 0.8f // Gravity Const
#define D 0    // Debug
#define SystemMass 1000 // Net mass of system
#define Explosion	(G*t*SystemMass*SystemMass/numPieces/dist/dist)*SystemMass/sqrt(numPieces)*3  //(10000.f*G/t/sqrt(numPieces))
#define numPieces	200.f


float randf();

#include "const.cpp"

#endif
