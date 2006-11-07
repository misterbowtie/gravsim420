#ifndef CONST_H
#define CONST_H

#define D 0					//Debug

// change to user changable... maybe
#define UpdatesPerFrame 25
#define FramesPerSecond 10.0 //max
double G=.37;				//Gravity Const


// dont mess with these
#define t 0.01f				//Physics time step
#define SystemMass 1000		//Net mass of system
#define numPieces 250.f		//
#define StarSize .43f	//Percent of SystemMass
#define TFORCE 2000
#define OFORCE 4

float randf();

#include "const.cpp"

#endif
