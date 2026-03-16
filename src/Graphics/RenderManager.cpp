#include <Engine/Graphics/RenderManager.hpp>

#include <Engine/Graphics/RenderTarget.hpp>
#include <Engine/Graphics/RenderTarget.hpp>


namespace eng
{

gfx::RenderManager::initialize()
{
	gladLoadGL();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	s_objects2d.clear();

	s_quad_view.setOrtho(0, 1, 1, 0, -10, 10);
	s_quad_shape.setPosition(eng::mth::Vec2(0, 1));
	s_quad_shape.setSize(eng::mth::Vec2(1));
	s_quad_shape.setScale(eng::mth::Vec2(1, -1));

}

gfx::RenderManager::finalize() {}


void gfx::RenderManager::addObject(CanvasItem* object)
{
	addObject(object, s_objects2d);
}

void gfx::RenderManager::removeObject(CanvasItem* object)
{
	removeObject(object, s_objects2d);
}

bool gfx::RenderManager::setRenderPipeline2d(const std::vector<RenderPass>& new_pipeline)
{
	s_pipeline2d = new_pipeline;

	for (const auto& pass : pipeline)
		if (!pass.shader) return false;

	
	return true;
}


void gfx::RenderManager::draw2d()
{
	if (s_objects2d.empty() || s_pipeline2d.empty())
        return;

	for (const auto& pass : pipeline)
	{
        Shader::setActive(pass.shader);

		View* active_view = gfx::View::getActive();
		mth::Mat4 view_transform_mat = active_view->getGlobalTransform().getMatrix();

		pass.shader->use();
		pass.shader->setUniformMatrix4fv("uProjection", active_view->getProjectionMatrix().getValuesPtr());
		pass.shader->setUniformMatrix4fv("uView", active_view->getViewMatrix().getValuesPtr());
        
        for (CanvasItem* obj : s_objects2d)
        {
            if (obj && obj->isVisible())
                obj->draw();
        }
    }
}



void gfx::RenderManager::addObject(Drawable* object, std::vector<Drawable*> vector)
{
	if (object)
	{
		for (unsigned int i = 0; i < vector.size(); i++)
			if (vector[i] == object)
				return;
		vector.push_back(object);
	}
}

void gfx::RenderManager::removeObject(Drawable* object, std::vector<Drawable*> vector)
{
	if (object)
		for (unsigned int i = 0; i < vector.size(); i++)
			if (vector[i] == object)
				vector.erase(vector.begin() + i--);
}

} // namespace eng