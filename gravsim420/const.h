#ifndef CONST_H
#define CONST_H

#include <math.h>

#define D 0					//Debug

// change to user changable... maybe
int UpdatesPerFrame =1;
#define FramesPerSecond 30 //max
double G=1;	 //.37			//Gravity Const


// dont mess with these
double t = 0.05f;				//Physics time step
#define SystemMass 1000		//Net mass of system
double numPieces = 1000;		//
//#define StarSize .43f	//Percent of SystemMass
//#define TFORCE 0
double OFORCE = .1;//.449;//
double ROT = .89;//.474;//
#define OPT 0

float randf();

#include "const.cpp"

#endif
