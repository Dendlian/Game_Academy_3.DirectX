#pragma once
#include "Engine/Vector.h"
#include "Engine/Rendering.h"

#include "Physics/CollisionProcess.h"

class Object abstract
{
public:
	virtual void Set()			abstract;
	virtual void Move()			abstract;
};

