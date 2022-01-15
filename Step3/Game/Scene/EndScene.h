#pragma once
#include "Manager/Scene.h"

class EndScene final : public Scene
{
public:
	virtual void Start()  override;
	virtual bool Update() override;
	virtual void End()    override;

private:

	Rendering::Image::Component	Background;

	Rendering::Camera	Camera;
};

