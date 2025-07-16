#include <Graphics/Drawable3.hpp>


gfx::Drawable3::Drawable3() : mth::Transformable3() {}

gfx::Drawable3::Drawable3(Drawable3& parent) : mth::Transformable3(*(parent.getParent())) {}