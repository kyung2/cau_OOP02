#pragma once
#include <d3dx9.h>
class CSphere;
class ICollidable{
public:
	virtual bool hasIntersected(CSphere&) = 0;
	virtual bool hitBy(CSphere&) = 0;

};