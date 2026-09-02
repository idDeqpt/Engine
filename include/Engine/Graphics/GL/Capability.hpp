#ifndef GRAPHICS_GL_CAPABILITY_STRUCT_HEADER
#define GRAPHICS_GL_CAPABILITY_STRUCT_HEADER

#include <Engine/Graphics/PrimitiveType.hpp>

namespace eng::gfx::gl
{
	struct Capability
	{
        enum class Mode
        {
            ANY,
            ENABLED,
            DISABLED
        };

        Capability() = default;
        Capability(bool flag_for_all)
        {
            depth_test   = blend = scissor_test =
            cull_face    = polygon_offset_fill  =
            stencil_test = dither = multisample =
            (flag_for_all ? Mode::ENABLED : Mode::DISABLED);
        }

        Mode depth_test          = Mode::ANY;
        Mode blend               = Mode::ANY;
        Mode scissor_test        = Mode::ANY;
        Mode cull_face           = Mode::ANY;
        Mode polygon_offset_fill = Mode::ANY;
        Mode stencil_test        = Mode::ANY;
        Mode dither              = Mode::ANY;
        Mode multisample         = Mode::ANY;
	};
} //namespace eng::gfx::gl

#endif //GRAPHICS_GL_CAPABILITY_STRUCT_HEADER