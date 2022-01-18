#pragma once
#include "Manager/Scene.h"

class StartScene final : public Scene
{
public:
	virtual void Start()  override;
	virtual bool Update() override;
	virtual void End()    override;

private:

	Rendering::Image::Component	Background;

	Rendering::Camera	Camera;

	Rendering::Text::Component		Text1;

};

