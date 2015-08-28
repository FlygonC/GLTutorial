#include "AppGrid.h"

int main()
{
	Application *App = new AppGrid();
	if (App->startup() == true)
	{
		while (true)
		{
			App->update(0);
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