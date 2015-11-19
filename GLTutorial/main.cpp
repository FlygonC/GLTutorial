//#include "AppPlanets.h"
//#include "AppRenderer.h"
//#include "AppFBX.h"
//#include "AppRenderTarget.h"
//#include "AppPostProcess.h"
//#include "AppDeferredStart.h"

//#include "AssetLibrary.h"
#include "TestApplication.h"

#include <time.h>

int main()
{
/*	Application *App = new AppDeferredStart();
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
	*/
	srand(time(NULL));

	using namespace AssetLibrary;

	Application* App = new TestApplication();
	App->init();
	App->play();
	App->kill();
	delete App;

	return 0;
}