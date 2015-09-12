#include "AppGrid.h"
//#include "AppPlanets.h"
//#include "AppRenderer.h"
#include "AppFBX.h"

int main()
{
	Application *App = new AppFBX();
	if (App->init())
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