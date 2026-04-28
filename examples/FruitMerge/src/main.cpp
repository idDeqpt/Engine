#include <Engine/Core/Engine.hpp>
#include <scenes/Root.hpp>

int main()
{
	Root r;
	eng::core::Engine engine(r);
	engine.setup();
	engine.mainLoop();

	return 0;
}