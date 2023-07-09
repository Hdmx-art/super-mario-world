#ifndef ITEM_H
#define ITEM_H

#include "levelobject.h"

class Mario;

class Item : public LevelObject 
{
public:
	Item(World* pWorld) : LevelObject(pWorld) {

	}

	virtual void on_hit(Mario* pMario) {
	}
};

#endif