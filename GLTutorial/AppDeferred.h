#pragma once

#include "Application.h"
#include "FlyCamera.h"

//#include <aie\Gizmos.h>

#include <stb_image.h>
#include <FBXFile.h>
//#include <tiny_obj_loader.h>
#include <vector>
#include <fstream>

class AppDeferred : public Application
{



	int oninit();
	void onkill();

	bool onstep(float deltaTime);
	void ondraw();
};