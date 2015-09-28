//#include "AppPlanets.h"
//#include "AppRenderer.h"
//#include "AppFBX.h"
#include "AppRenderTarget.h"
#include "AppPostProcess.h"

int main()
{
	Application *App = new AppPostProcess();
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