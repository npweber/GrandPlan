#pragma once
#ifndef DataUnit_h
#define DataUnit_h

#include <string>

using namespace std;

//DataUnit.h: 
//-Representation of a planner event; 
//-An object that fits into an index of an array of this object
//that represents 1 week of planner events, each by each hour.

typedef struct DataUnit {
	//Holds a title, and a description
	string title;
	string description;
};

#endif 