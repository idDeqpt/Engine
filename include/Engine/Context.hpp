#ifndef CONTEXT_CLASS_HEADER
#define CONTEXT_CLASS_HEADER

#include <Engine/Core/TimeManager.hpp>
#include <Engine/Core/ResourceManager.hpp>

#include <Engine/System/EventManager.hpp>

#include <Engine/Graphics/RenderScene.hpp>
#include <Engine/Graphics/LightManager.hpp>
#include <Engine/Graphics/RenderManager.hpp>

#include <Engine/Physics/PhysicsWorld.hpp>


namespace eng
{
	class Context
	{
	public:
		core::TimeManager&      getTimeManager()      {return m_time_manager;}
		gfx::RenderManager&     getRenderManager()    {return m_render_manager;}
		sys::EventManager&      getEventManager()     {return m_event_manager;}
		core::ResourceManager&  getResourceManager()  {return m_resource_manager;}
		gfx::LightManager&      getLightManager()     {return m_light_manager;}
		gfx::RenderScene&       getRenderScene()      {return m_render_scene;}
		phy::PhysicsWorld&      getPhysicsWorld()     {return m_physics_world;}

	protected:
		core::TimeManager      m_time_manager;
		gfx::RenderManager     m_render_manager;
		sys::EventManager      m_event_manager;
		core::ResourceManager  m_resource_manager;
		gfx::LightManager      m_light_manager;
		gfx::RenderScene       m_render_scene;
		phy::PhysicsWorld      m_physics_world;
	};
}

#endif //CONTEXT_CLASS_HEADER