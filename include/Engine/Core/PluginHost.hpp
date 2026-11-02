#ifndef PLUGIN_HOST_CLASS_HEADER
#define PLUGIN_HOST_CLASS_HEADER

namespace eng
{
	class Context;

namespace core
{
	class Node;

	struct PluginHost
	{
		eng::Context& context;
		core::Node&   root;
	};
}
}

#endif //PLUGIN_HOST_CLASS_HEADER