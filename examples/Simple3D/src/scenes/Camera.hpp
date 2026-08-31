#ifndef CAMERA_CLASS_HEADER
#define CAMERA_CLASS_HEADER

#include <Engine/System/EventManager.hpp>
#include <Engine/Graphics/3D/Camera3D.hpp>
#include <Engine/Math/Quaternion.hpp>
#include <Engine/Math/Vec3.hpp>
#include <Engine/Context.hpp>

class Camera : public eng::gfx::Camera3D
{
public:
	void onSetup() override
	{
		setPerspective(3.14*0.25, float(900)/600, 1, 100);
	}

	void onUpdate(float delta) override
	{
		auto event_manager = m_context.get<eng::sys::EventManager>();
		eng::mth::Vec3 vel;
		constexpr float speed = 30;
		if (event_manager.getKeyboard().isPressed(eng::sys::Keyboard::Key::W))
			vel.z -= speed;
		if (event_manager.getKeyboard().isPressed(eng::sys::Keyboard::Key::S))
			vel.z += speed;
		if (event_manager.getKeyboard().isPressed(eng::sys::Keyboard::Key::A))
			vel.x -= speed;
		if (event_manager.getKeyboard().isPressed(eng::sys::Keyboard::Key::D))
			vel.x += speed;
		if (event_manager.getKeyboard().isPressed(eng::sys::Keyboard::Key::LEFT_CONTROL))
			vel.y -= speed;
		if (event_manager.getKeyboard().isPressed(eng::sys::Keyboard::Key::SPACE))
			vel.y += speed;
		
		if (event_manager.getKeyboard().isPressed(eng::sys::Keyboard::Key::L))
			event_manager.setCursorMode(eng::sys::Mouse::CursorMode::DISABLE);
		if (event_manager.getKeyboard().isPressed(eng::sys::Keyboard::Key::K))
			event_manager.setCursorMode(eng::sys::Mouse::CursorMode::NORMAL);

		if (event_manager.getMouse().moved())
			m_rot_angles = m_rot_angles + event_manager.getMouse().getDelta()*delta;

		this->setRotation(eng::mth::Quaternion(eng::mth::Vec3(0, 1, 0), 0));
		this->rotate(eng::mth::Quaternion(eng::mth::Vec3(0, 1, 0), -m_rot_angles.x));
		this->rotate(eng::mth::Quaternion(eng::mth::Vec3(1, 0, 0), -m_rot_angles.y));

		if (vel.x || vel.y || vel.z)
			this->relativeMove(vel*delta);
	}

protected:
	eng::mth::Vec2 m_rot_angles;
};

#endif //CAMERA_CLASS_HEADER