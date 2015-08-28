#include "AppGrid.h"
#include "AppPlanets.h"

int main()
{
	Application *App = new AppPlanets();
	if (App->startup() == true)
	{
		while (App->update())
		{
			App->draw();
		}
	}
	else
	{
		return -1;
	}
	App->shutdown();
	
	return 0;
}