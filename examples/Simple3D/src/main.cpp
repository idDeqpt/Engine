#include <Engine/Engine.hpp>
#include <scenes/Root.hpp>
#include <memory>

int main()
{
	eng::Engine engine(std::make_unique<Root>());
	engine.setup();
	engine.mainLoop();

	return 0;
}