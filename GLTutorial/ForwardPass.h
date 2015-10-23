#pragma once

#include "Render.h"

struct Camera;
struct RenderObject;

class ForwardPass : public AssetLibrary::RenderPass
{
	void prep();
	void post();
	void draw(Camera cam, RenderObject obj);
};