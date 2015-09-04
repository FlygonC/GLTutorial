#include "AppGrid.h"
#include "AppPlanets.h"

int main()
{
	Application *App = new AppPlanets();
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
	
	return 0;
}