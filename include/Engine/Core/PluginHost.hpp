#ifndef PLUGIN_HOST_CLASS_HEADER
#define PLUGIN_HOST_CLASS_HEADER

namespace eng::core
{
	class Context;

	class Node;

	struct PluginHost
	{
		Context& context;
		Node&   root;
	};
} //namespace eng::core

#endif //PLUGIN_HOST_CLASS_HEADER