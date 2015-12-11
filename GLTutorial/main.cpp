#include "TestApplication.h"

#include <time.h>

int main()
{
	srand(time(NULL));

	using namespace AssetLibrary;

	Application* App = new TestApplication();
	App->init();
	App->play();
	App->kill();
	delete App;

	return 0;
}