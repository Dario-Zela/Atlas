#include "Atlas.h"

int main()
{
	Atlas::Logger->Init("djdj");
	Atlas::Application app("Test", 1024, 700);
	app.Run();
}