#pragma once
#include "Engine/Vector.h"
#include "Engine/Rendering.h"

class Character abstract
{
public:
	virtual void SetCharacter() abstract;
	virtual void Move()			abstract;
	virtual void Attack()		abstract;
};

