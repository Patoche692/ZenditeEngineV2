#pragma once
#include "I_Renderer.h"

class ECSCoordinator;

class OpenGL_Renderer : public I_Renderer
{
public:
	OpenGL_Renderer(std::shared_ptr<Camera> cam);
	void Render(const R_DataHandle& DataHandle, const c_Transform& trans) override;

private:


};