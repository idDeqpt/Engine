#ifndef GRAPHICS_API_CLASS_HEADER
#define GRAPHICS_API_CLASS_HEADER

#include <memory>

namespace eng::gfx::gl
{

	class Api
	{
	public:
		enum class Type
		{
			OPENGL_3_3,
			AUTO
		};

		static Api* getInstance();
		static void createInstance(Type type = Type::AUTO);

		virtual ~Api() = default;

		virtual void init() = 0;

	protected:
		static std::unique_ptr<Api> s_instance;

	private:
		static std::unique_ptr<Api> createImpl(Type type);
	};
}

#endif //GRAPHICS_API_CLASS_HEADER