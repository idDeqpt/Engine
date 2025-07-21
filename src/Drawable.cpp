#include <Graphics/Drawable.hpp>


gfx::Drawable::Drawable() : mth::Transformable3() {}

gfx::Drawable::Drawable(Drawable& parent) : mth::Transformable3(*(parent.getParent())) {}