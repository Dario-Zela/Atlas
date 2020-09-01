#include "Atlas.h"

int main()
{
	Atlas::Window window;
	if (window.Init("Test", 1024, 700))
	{
		while (window.isRunning())
		{
			window.Broadcast();
		}
	}
	return 0;
}