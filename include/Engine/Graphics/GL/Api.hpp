#ifndef GRAPHICS_API_CLASS_HEADER
#define GRAPHICS_API_CLASS_HEADER

#include <memory>

namespace eng::gfx::gl
{
	class TexturePool;
	class ArrayBuffer;
	class FrameBuffer;
	class Shader;
	class TextureImpl;
	class Capability;

	enum class DepthFunction
	{
		NEVER,
		LESS,
		EQUAL,
		LEQUAL,
		GREATER,
		NOTEQUAL,
		GEQUAL,
		ALWAYS
	};

	enum class BlendFactor
	{
		ZERO,
		ONE,
		SRC_COLOR,
		ONE_MINUS_SRC_COLOR,
		DST_COLOR,
		ONE_MINUS_DST_COLOR,
		SRC_ALPHA,
		ONE_MINUS_SRC_ALPHA,
		DST_ALPHA,
		ONE_MINUS_DST_ALPHA,
		CONSTANT_COLOR,
		ONE_MINUS_CONSTANT_COLOR,
		CONSTANT_ALPHA,
		ONE_MINUS_CONSTANT_ALPHA,
		SRC_ALPHA_SATURATE
	};

	class Api
	{
	public:
    	using ProcAddressLoader = void* (*)(const char* procname);

		enum class Type
		{
			NULL_API,
			OPENGL_3_3,
			AUTO
		};

		static Api* getInstance();
		static void createInstance(Type type = Type::AUTO);

		static TexturePool* getTexturePool();

		virtual ~Api() = default;

		virtual Type getType() = 0;
		virtual void initialize(ProcAddressLoader loader) = 0;
		virtual void finalize();

		virtual void setCapability(const Capability& capability) = 0;

		virtual void useDepthMask(bool flag) = 0;
		virtual void setDepthFunction(const DepthFunction& function) = 0;
		virtual void setBlendFunction(const BlendFactor& source, const BlendFactor& destination) = 0;

		virtual void setScissor(int x, int y, unsigned int width, unsigned int height) = 0;
		virtual void setViewport(int x, int y, unsigned int width, unsigned int height) = 0;
		
		virtual void setActiveTexture(unsigned int texrure_i) = 0;

		virtual std::unique_ptr<ArrayBuffer> createArrayBuffer() = 0;
		virtual std::unique_ptr<FrameBuffer> createFrameBuffer() = 0;
		virtual std::unique_ptr<Shader>      createShader()      = 0;
		
		virtual std::unique_ptr<TextureImpl> createTextureImpl() = 0;

	protected:
		static std::unique_ptr<Api> s_instance;
		static std::unique_ptr<TexturePool> s_texture_pool;

	private:
		static std::unique_ptr<Api> createImpl(Type type);
	};
}

#endif //GRAPHICS_API_CLASS_HEADER