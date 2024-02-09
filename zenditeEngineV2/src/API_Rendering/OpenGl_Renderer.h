#pragma once
#include "I_Renderer.h"

class ECSCoordinator;

class OpenGL_Renderer : public I_Renderer
{
public:
	void Render(const R_DataHandle& DataHandle, const transform& trans) override;

private:


};