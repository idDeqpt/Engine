#ifndef CONTEXT_CLASS_HEADER
#define CONTEXT_CLASS_HEADER

#include <Engine/Core/TimeManager.hpp>
#include <Engine/Core/ResourceManager.hpp>

#include <Engine/System/EventManager.hpp>

#include <Engine/Graphics/RenderScene.hpp>
#include <Engine/Graphics/LightManager.hpp>
#include <Engine/Graphics/RenderManager.hpp>

#include <Engine/Physics/PhysicsManager.hpp>


namespace eng
{
	class Context
	{
	public:
		core::TimeManager&  getTimeManager()  {return m_time_manager;}
		gfx::RenderManager& getRenderManager() {return m_render_manager;}
		sys::EventManager&  getEventManager()  {return m_event_manager;}
		gfx::RenderScene&   getRenderScene()   {return m_render_scene;}
		gfx::LightManager&  getLightManager()  {return m_light_manager;}
		core::ResourceManager&  getResourceManager()  {return m_resource_manager;}
		phy::PhysicsManager&    getPhysicsManager()   {return m_physics_manager;}

	protected:
		core::TimeManager  m_time_manager;
		gfx::RenderManager m_render_manager;
		sys::EventManager  m_event_manager;
		gfx::RenderScene   m_render_scene;
		gfx::LightManager  m_light_manager;
		core::ResourceManager  m_resource_manager;
		phy::PhysicsManager    m_physics_manager;
	};
}

#endif //CONTEXT_CLASS_HEADER