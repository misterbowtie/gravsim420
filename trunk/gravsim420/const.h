#ifndef CONST_H
#define CONST_H

#define D 0					//Debug

// change to user changable... maybe
#define UpdatesPerFrame 40
#define FramesPerSecond .1 //max
double G=1;	 //.37			//Gravity Const


// dont mess with these
#define t 0.05f				//Physics time step
#define SystemMass 1000		//Net mass of system
#define numPieces 1000.f		//
//#define StarSize .43f	//Percent of SystemMass
//#define TFORCE 0
double OFORCE = .1;//.449;//
double ROT = .89;//.474;//
#define OPT 0

float randf();

#include "const.cpp"

#endif
