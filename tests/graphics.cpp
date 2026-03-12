#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Engine/Graphics/Color.hpp>
#include <Engine/Graphics/Window.hpp>
#include <Engine/Graphics/Shader.hpp>
#include <Engine/Graphics/Shape2D.hpp>
#include <Engine/Graphics/Text2D.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Engine/Graphics/Font.hpp>
#include <Engine/Graphics/LightManager.hpp>
#include <Engine/Graphics/EventManager.hpp>
#include <Engine/Graphics/Mesh.hpp>
#include <Engine/Graphics/GeometricMesh.hpp>
#include <Engine/Graphics/RenderStates.hpp>
#include <Engine/Graphics/RenderTarget.hpp>
#include <Engine/Graphics/PrimitiveType.hpp>
#include <Engine/Graphics/CanvasItem.hpp>
#include <Engine/Math/Vec2.hpp>
#include <Engine/Math/Vec3.hpp>
#include <Engine/Math/Vec4.hpp>
#include <Engine/Math/Quaternion.hpp>
#include <Engine/Math/Transform3.hpp>
#include <Engine/Math/Transformable3.hpp>
#include <cmath>
#include <vector>
#include <random>

void print(const eng::mth::Mat4& mat)
{
	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
			std::cout << mat[i][j] << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void print(const eng::mth::Mat3& mat)
{
	for (unsigned int i = 0; i < 3; i++)
	{
		for (unsigned int j = 0; j < 3; j++)
			std::cout << mat[i][j] << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


void print(const eng::mth::Quaternion& q)
{
	std::cout << q.w << " ";
	std::cout << q.x << " ";
	std::cout << q.y << " ";
	std::cout << q.z << " ";
	std::cout << std::endl;
}

void print(const eng::mth::Vec4& vec)
{
	std::cout << vec.x << " ";
	std::cout << vec.y << " ";
	std::cout << vec.z << " ";
	std::cout << vec.w << " ";
	std::cout << std::endl;
}

void print(const eng::mth::Vec3& vec)
{
	std::cout << vec.x << " ";
	std::cout << vec.y << " ";
	std::cout << vec.z << " ";
	std::cout << std::endl;
}

eng::mth::Vec4 mult(const eng::mth::Mat4& mat, const eng::mth::Vec4& vec)
{
	eng::mth::Vec4 result;
	for (int i = 0; i < 4; i++)
	{
		result.x += mat[i][0]*vec.x;
		result.y += mat[i][1]*vec.y;
		result.z += mat[i][2]*vec.z;
		result.w += mat[i][3]*vec.w;
	}
	return result;
}


int main()
{
	unsigned int width  = 900;
	unsigned int height = 600;
	srand(0);
	eng::gfx::Window window(width, height, "LearnOpenGL");
	
	glCullFace(GL_FRONT);

	std::string shaders_dir = "E:/Programming/Projects/C++/Engine/include/Engine/Graphics/shaders";

	eng::gfx::Shader shader3d_deferred;
	shader3d_deferred.loadFromFile(shaders_dir + "/default3d.vert",
						  shaders_dir + "/default3d-deferred.frag");
	std::cout << "Shader 3d-deferred error: " << shader3d_deferred.getLastError() << std::endl;
	if (shader3d_deferred.getLastError() != eng::gfx::Shader::Error::NO_ERROR)
		std::cout << "Shader 3d-deferred error log: " << shader3d_deferred.getLastErrorLog() << std::endl;

	eng::gfx::Shader shader2d_deferred_light;
	shader2d_deferred_light.loadFromFile(shaders_dir + "/default2d.vert",
						  shaders_dir + "/default3d-deferred-light.frag");
	std::cout << "Shader 2d-deferred-light error: " << shader2d_deferred_light.getLastError() << std::endl;
	if (shader2d_deferred_light.getLastError() != eng::gfx::Shader::Error::NO_ERROR)
		std::cout << "Shader 2d-deferred-light error log: " << shader2d_deferred_light.getLastErrorLog() << std::endl;

	eng::gfx::Shader shader2d;
	shader2d.loadFromFile(shaders_dir + "/default2d.vert",
						  shaders_dir + "/default2d.frag");
	std::cout << "Shader 2d error: " << shader2d.getLastError() << std::endl;
	if (shader2d.getLastError() != eng::gfx::Shader::Error::NO_ERROR)
		std::cout << "Shader 2d error log: " << shader2d.getLastErrorLog() << std::endl;

	eng::gfx::LightManager::initialize();
	eng::gfx::EventManager::initialize(window.getHandler());

	std::string resources_dir = "E:/Programming/Projects/C++/Engine/tests/resources";
	eng::gfx::Texture tex[12];
	std::cout << "TEXTURE\n";
	std::cout << "Tex0: " << tex[0].loadFromFile(resources_dir + "/image1.png") << std::endl;
	std::cout << "Tex1: " << tex[1].loadFromFile(resources_dir + "/image2.png") << std::endl;
	std::cout << "Tex2: " << tex[2].loadFromFile(resources_dir + "/bricks.jpg") << std::endl;
	std::cout << "Tex3: " << tex[3].loadFromFile(resources_dir + "/box-map.png") << std::endl;
	std::cout << "Tex4: " << tex[4].loadFromFile(resources_dir + "/bricks-normal.jpg") << std::endl;
	std::cout << "Tex5: " << tex[5].loadFromFile(resources_dir + "/bricks-parallax.jpg") << std::endl;
	std::cout << "Tex6: " << tex[6].loadFromFile(resources_dir + "/bricks/redbricks2b-albedo.png") << std::endl;
	std::cout << "Tex7: " << tex[7].loadFromFile(resources_dir + "/bricks/redbricks2b-normal.png") << std::endl;
	std::cout << "Tex8: " << tex[8].loadFromFile(resources_dir + "/bricks/redbricks2b-metalness.png") << std::endl;
	std::cout << "Tex9: " << tex[9].loadFromFile(resources_dir + "/bricks/redbricks2b-rough.png") << std::endl;
	std::cout << "Tex10: " << tex[10].loadFromFile(resources_dir + "/bricks/redbricks2b-height4b.png") << std::endl;
	std::cout << "Tex11: " << tex[11].loadFromFile(resources_dir + "/bricks/redbricks2b-ao.png") << std::endl;
	std::cout << "Err: "  << glGetError() << std::endl;


	eng::gfx::Font font;
	font.loadFromFile(resources_dir + "/GameFont.ttf", 24);

	const unsigned int floor_size     = 50;
	const unsigned int floor_accuracy = 30;
	const unsigned int floor_points_count  = floor_accuracy*floor_accuracy;
	const unsigned int floor_normals_count = (floor_accuracy - 1)*(floor_accuracy - 1)*2;
	const unsigned int floor_indexes_count = floor_normals_count*3;

	eng::mth::Vec3 floor_points    [floor_points_count];
	eng::mth::Vec2 floor_tex_coords[floor_points_count];
	eng::mth::Vec3 floor_normals   [floor_normals_count];
	eng::mth::Vec3 floor_tangents  [floor_normals_count];
	unsigned int floor_indexes[floor_indexes_count];
	unsigned int floor_normal_indexes[floor_indexes_count];

	unsigned int index = 0;
	for (unsigned int i = 0; i < floor_accuracy; i++)
		for (unsigned int j = 0; j < floor_accuracy; j++)
		{
			floor_points[index]       = eng::mth::Vec3(floor_size*float(i)/(floor_accuracy-1), (rand()%10)*0.0,           floor_size*float(j)/(floor_accuracy-1));
			floor_tex_coords[index++] = eng::mth::Vec2(float(i)/(floor_accuracy-1),            float(j)/(floor_accuracy-1));
		}
	index = 0;
	for (unsigned int i = 0; i < (floor_accuracy - 1); i++)
		for (unsigned int j = 0; j < (floor_accuracy - 1); j++)
		{
			unsigned int first  = i*floor_accuracy + j;
			unsigned int second = first + floor_accuracy;

			floor_normals[index/3]  = -(floor_points[second] - floor_points[first]).cross(floor_points[first + 1] - floor_points[first]).norm();
			floor_tangents[index/3] = eng::mth::Vec3(1, 0, 0);
			floor_normal_indexes[index] = index/3;
			floor_indexes[index++] = first;
			floor_normal_indexes[index] = index/3 + 1;
			floor_indexes[index++] = second;
			floor_normal_indexes[index] = index/3 + 2;
			floor_indexes[index++] = first + 1;

			floor_normals[index/3]  = -(floor_points[second] - floor_points[first + 1]).cross(floor_points[second + 1] - floor_points[second]).norm();
			floor_tangents[index/3] = eng::mth::Vec3(1, 0, 0);
			floor_normal_indexes[index] = index/3;
			floor_indexes[index++] = second;
			floor_normal_indexes[index] = index/3 + 1;
			floor_indexes[index++] = second + 1;
			floor_normal_indexes[index] = index/3 + 2;
			floor_indexes[index++] = first + 1;
		}

	eng::gfx::Mesh floor;
	std::cout << "SUCCESS: " << floor.loadData({floor_points,     floor_points_count,  floor_indexes,
												floor_tex_coords, floor_points_count,  floor_indexes,
												floor_normals,    floor_normals_count, floor_indexes,
												floor_tangents,   floor_normals_count, floor_indexes, floor_indexes_count}) << std::endl;
	floor.setMaterial({
		&tex[6],
		&tex[7],
		&tex[8],
		&tex[9],
		&tex[10],
		&tex[11],
		nullptr
	});

	float obj_size = 1;
	eng::gfx::GeometricMesh obj(eng::gfx::GeometricMesh::Type::PARALLELEPIPED);
	//eng::gfx::GeometricMesh obj(eng::gfx::GeometricMesh::Type::ELLIPSOID);
	obj.setSize(eng::mth::Vec3(1.5, 1.5, 1.5));
	obj.setOrigin(eng::mth::Vec3(0, -2.5, 0));
	//obj.setAccuracy(500);
	obj.setMaterial({
		&tex[6],
		&tex[7],
		&tex[8],
		&tex[9],
		&tex[10],
		&tex[11],
		nullptr
	});

	eng::gfx::RenderStates states;
	eng::gfx::View view3d;
	//view3d.setOrtho(0, 10, 10, 0, -30, 30);
	view3d.setPerspective(3.14*0.25, float(width)/height, 1, 100);
	view3d.setPosition(eng::mth::Vec3(0, 1, 2));
	eng::gfx::View view2d;
	view2d.setOrtho(0, width, height, 0, -10, 10);

	std::vector<eng::mth::Mat4> translations;
	for (unsigned int i = 0; i < floor_points_count; i+=10)
		translations.push_back(eng::mth::Mat4(1, 0, 0, floor_points[i].x,
										 0, 1, 0, floor_points[i].y,
										 0, 0, 1, floor_points[i].z,
										 0, 0, 0, 1));
	obj.loadInstances(translations.data(), translations.size());

	eng::gfx::LightManager::enableDirectionalLight({eng::mth::Vec3(0, -1, 0), eng::mth::Vec3(1, 1, 1)});

	eng::gfx::Text2D info_texts[2];
	for (unsigned int i = 0; i < 2; i++)
	{
		info_texts[i].setFont(font);
		info_texts[i].setPosition(eng::mth::Vec2(0, font.getSize()*i));
	}

	eng::gfx::Shape2D shape(eng::gfx::Shape2D::Type::RECTANGLE);
	//eng::gfx::Shape2D shape(eng::gfx::Shape2D::Type::CIRCLE);
	shape.setPosition(eng::mth::Vec2(700, 400));
	shape.setSize(eng::mth::Vec2(100, 100));
	//shape.setTexture(*font.getTexture());
	shape.setTexture(tex[2]);

	eng::gfx::Texture::PixelFormat formats[] = {
		eng::gfx::Texture::PixelFormat::RGBA32F, //position
		eng::gfx::Texture::PixelFormat::RGBA32F, //normal
		eng::gfx::Texture::PixelFormat::RGBA, //albedo
		eng::gfx::Texture::PixelFormat::RGBA, //met rog hei ao
		eng::gfx::Texture::PixelFormat::RGBA //emission
	};
	eng::gfx::RenderTarget rt(5, formats);

	eng::gfx::Shape2D render_shape0(eng::gfx::Shape2D::Type::RECTANGLE);
	render_shape0.setPosition(eng::mth::Vec2(0, 180));
	render_shape0.setSize(eng::mth::Vec2(240, 180));
	render_shape0.setScale(eng::mth::Vec2(1, -1));
	render_shape0.setTexture(rt.getTexture(0));

	eng::gfx::Shape2D render_shape1(eng::gfx::Shape2D::Type::RECTANGLE);
	render_shape1.setPosition(eng::mth::Vec2(240, 180));
	render_shape1.setSize(eng::mth::Vec2(240, 180));
	render_shape1.setScale(eng::mth::Vec2(1, -1));
	render_shape1.setTexture(rt.getTexture(1));

	eng::gfx::Shape2D render_shape2(eng::gfx::Shape2D::Type::RECTANGLE);
	render_shape2.setPosition(eng::mth::Vec2(480, 180));
	render_shape2.setSize(eng::mth::Vec2(240, 180));
	render_shape2.setScale(eng::mth::Vec2(1, -1));
	render_shape2.setTexture(rt.getTexture(2));

	eng::gfx::Shape2D render_shape3(eng::gfx::Shape2D::Type::RECTANGLE);
	render_shape3.setPosition(eng::mth::Vec2(0, 360));
	render_shape3.setSize(eng::mth::Vec2(240, 180));
	render_shape3.setScale(eng::mth::Vec2(1, -1));
	render_shape3.setTexture(rt.getTexture(3));

	eng::gfx::Shape2D render_shape4(eng::gfx::Shape2D::Type::RECTANGLE);
	render_shape4.setPosition(eng::mth::Vec2(240, 360));
	render_shape4.setSize(eng::mth::Vec2(240, 180));
	render_shape4.setScale(eng::mth::Vec2(1, -1));
	render_shape4.setTexture(rt.getTexture(4));

	eng::gfx::Shape2D light_shape(eng::gfx::Shape2D::Type::RECTANGLE);
	light_shape.setPosition(eng::mth::Vec2(0, 1));
	light_shape.setSize(eng::mth::Vec2(1));
	light_shape.setScale(eng::mth::Vec2(1, -1));

	eng::gfx::View light_view;
	light_view.setOrtho(0, 1, 1, 0, -10, 10);

	eng::mth::Transformable3 parent;
	obj.setParent(parent);

	float speed = 0.1;
	eng::mth::Vec2 rot_angles;
	float delta_time = 0;
	float light_rot = 0;
	while(window.isOpen())
	{
		float start_time = glfwGetTime();
		eng::mth::Vec3 vel;
		eng::gfx::EventManager::pull();
		if (eng::gfx::EventManager::isPressed(GLFW_KEY_ESCAPE)) window.close();
		if (eng::gfx::EventManager::isJustPressed(GLFW_KEY_R)) tex[5].setSmooth(false);
		if (eng::gfx::EventManager::isJustReleased(GLFW_KEY_R)) tex[5].setSmooth(true);
		if (eng::gfx::EventManager::isJustPressed(GLFW_KEY_L)) eng::gfx::EventManager::setCursorLock(!eng::gfx::EventManager::getCursorLock());
		if (eng::gfx::EventManager::isPressed(GLFW_KEY_W))
			vel.z -= speed;
		if (eng::gfx::EventManager::isPressed(GLFW_KEY_S))
			vel.z += speed;
		if (eng::gfx::EventManager::isPressed(GLFW_KEY_A))
			vel.x -= speed;
		if (eng::gfx::EventManager::isPressed(GLFW_KEY_D))
			vel.x += speed;
		if (eng::gfx::EventManager::isPressed(GLFW_KEY_LEFT_CONTROL))
			vel.y -= speed;
		if (eng::gfx::EventManager::isPressed(GLFW_KEY_SPACE))
			vel.y += speed;
		if (eng::gfx::EventManager::isPressed(GLFW_KEY_RIGHT))
			light_rot += 0.05;
		if (eng::gfx::EventManager::isPressed(GLFW_KEY_LEFT))
			light_rot -= 0.05;

		if (eng::gfx::EventManager::Mouse::moved())
			rot_angles = rot_angles + eng::gfx::EventManager::Mouse::getDelta();
		eng::gfx::LightManager::enableDirectionalLight({eng::mth::Quaternion(eng::mth::Vec3(0, 0, 1), light_rot).rotateVec(eng::mth::Vec3(0, -1, 0)), eng::mth::Vec3(1, 1, 1)});
		
		view3d.setRotation(eng::mth::Quaternion(eng::mth::Vec3(0, 1, 0), 0));
		view3d.rotate(eng::mth::Quaternion(eng::mth::Vec3(0, 1, 0), -rot_angles.x*0.01));
		view3d.rotate(eng::mth::Quaternion(eng::mth::Vec3(1, 0, 0), -rot_angles.y*0.01));

		if (vel.x || vel.y || vel.z)
			view3d.relativeMove(vel);

		float time = (GLfloat)glfwGetTime();
		//obj.setRotation(eng::mth::Quaternion(eng::mth::Vec3(0.1, 1, 0), time*0.5));
		parent.setRotation(eng::mth::Quaternion(eng::mth::Vec3(0, 1, 0), time*0.5));

		info_texts[0].setString("Frame time: " + std::to_string(delta_time) + "s");
		info_texts[1].setString("Posi\ntion: " + std::to_string(view3d.getPosition().x) + " " + std::to_string(view3d.getPosition().y) + " " + std::to_string(view3d.getPosition().z));

		window.clear(eng::gfx::Color(160));
		rt.clear(eng::gfx::Color(0));

		glEnable(GL_CULL_FACE);
		eng::gfx::View::setActive(&view3d);
		eng::gfx::Shader::setActive(&shader3d_deferred);

		rt.draw(obj, states);
		rt.draw(floor, states);

		glDisable(GL_CULL_FACE);
		//glClear(GL_DEPTH_BUFFER_BIT);
		eng::gfx::View::setActive(&view2d);
		eng::gfx::Shader::setActive(&shader2d);

		//window.draw(ci, states);
		for (unsigned int i = 0; i < 0; i++)
		{
			info_texts[i].setColor(eng::gfx::Color(255, (sin(start_time) + 1)*0.5*255, 255, 255));
			window.draw(info_texts[i], states);
		}
		window.draw(shape, states);
		/*window.draw(render_shape0, states);
		window.draw(render_shape1, states);
		window.draw(render_shape2, states);
		window.draw(render_shape3, states);
		window.draw(render_shape4, states);*/

		eng::gfx::View::setActive(&light_view);
		eng::gfx::Shader::setActive(&shader2d_deferred_light);

			shader2d_deferred_light.use();
			eng::gfx::View* active_view = &view3d;
			eng::mth::Vec3 view_loc_pos = active_view->getPosition();
			eng::mth::Vec4 view_glob_pos = active_view->getGlobalTransform().getMatrix()*eng::mth::Vec4(view_loc_pos.x, view_loc_pos.y, view_loc_pos.z, 1);
			shader2d_deferred_light.setUniform3fv("uViewPos", 1, &view_glob_pos.x);

			glActiveTexture(GL_TEXTURE0);
			eng::gfx::Texture::bind(&(rt.getTexture(0)));
			shader2d_deferred_light.setUniform1i("uPosition", 0);
			glActiveTexture(GL_TEXTURE1);
			eng::gfx::Texture::bind(&(rt.getTexture(1)));
			shader2d_deferred_light.setUniform1i("uNormal", 1);
			glActiveTexture(GL_TEXTURE2);
			eng::gfx::Texture::bind(&(rt.getTexture(2)));
			shader2d_deferred_light.setUniform1i("uAlbedo", 2);

			eng::gfx::LightManager::DirectionalLight light = eng::gfx::LightManager::getDirectionalLight();
			bool use_directional_light = light.direction.x || light.direction.y || light.direction.z;
			shader2d_deferred_light.setUniform1i("uUseDirectionalLight", use_directional_light);
			if (use_directional_light)
			{
				shader2d_deferred_light.setUniform3fv("uDirectionalLight.direction", 1, &light.direction.x);
				shader2d_deferred_light.setUniform3fv("uDirectionalLight.color", 1, &light.color.x);
			}

		window.draw(light_shape, states);

		window.display();

		delta_time = glfwGetTime() - start_time;
		std::cout << delta_time << std::endl;
	}

	eng::gfx::LightManager::finalize();

	window.destroy();
	system("pause");
	return 0;
}