//#include "AppPlanets.h"
//#include "AppRenderer.h"
#include "AppFBX.h"
#include "AppDeferred.h"

int main()
{
	Application *App = new AppDeferred();
	if (App->init(1600, 900, "Application"))
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