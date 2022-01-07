#pragma once
#include "Manager/Scene.h"
#include "TestScene.h"

class PlayerScene : public Scene
{
public:
	virtual void Start()  override;
	virtual bool Update() override;
	virtual void End()    override;

private:

};

