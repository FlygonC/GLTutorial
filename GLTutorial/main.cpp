//#include "AppPlanets.h"
//#include "AppRenderer.h"
//#include "AppFBX.h"
#include "AppRenderTarget.h"

int main()
{
	Application *App = new AppRenderTarget();
	if (App->init(1280, 720, "Application"))
	{
		while (App->step())
		{
			App->draw();
		}
	}
	else
	{
		return -1;
	}
	App->kill();
	delete App;
	
	return 0;
}