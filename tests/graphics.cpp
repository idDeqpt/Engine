#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Engine/Core/ResourceManager.hpp>
#include <Engine/Graphics/Color.hpp>
#include <Engine/System/Window.hpp>
#include <Engine/Graphics/Shader.hpp>
#include <Engine/Graphics/Shape2D.hpp>
#include <Engine/Graphics/Text2D.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Engine/Graphics/Font.hpp>
#include <Engine/Graphics/LightManager.hpp>
#include <Engine/System/EventManager.hpp>
#include <Engine/Graphics/Mesh.hpp>
#include <Engine/Graphics/GeometricMesh.hpp>
#include <Engine/Graphics/RenderScene.hpp>
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
	eng::sys::Window window(width, height, "LearnOpenGL");
	gladLoadGL();
	
	glCullFace(GL_FRONT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::string shaders_dir = "E:/Programming/Projects/C++/Engine/include/Engine/Graphics/shaders";

	eng::gfx::Shader* shader3d_deferred = eng::core::ResourceManager::load<eng::gfx::Shader>({
		shaders_dir + "/default3d.vert",
		shaders_dir + "/default3d-deferred.frag"
	}).second;
	std::cout << "Shader 3d-deferred error: " << shader3d_deferred->getLastError() << std::endl;
	if (shader3d_deferred->getLastError() != eng::gfx::Shader::Error::NO_ERROR)
		std::cout << "Shader 3d-deferred error log: " << shader3d_deferred->getLastErrorLog() << std::endl;

	eng::gfx::Shader* shader2d_deferred_light = eng::core::ResourceManager::load<eng::gfx::Shader>({
		shaders_dir + "/default2d.vert",
		shaders_dir + "/default3d-deferred-light.frag"
	}).second;
	std::cout << "Shader 2d-deferred-light error: " << shader2d_deferred_light->getLastError() << std::endl;
	if (shader2d_deferred_light->getLastError() != eng::gfx::Shader::Error::NO_ERROR)
		std::cout << "Shader 2d-deferred-light error log: " << shader2d_deferred_light->getLastErrorLog() << std::endl;

	eng::gfx::Shader* shader2d = eng::core::ResourceManager::load<eng::gfx::Shader>({
		shaders_dir + "/default2d.vert",
		shaders_dir + "/default2d.frag"
	}).second;
	std::cout << "Shader 2d error: " << shader2d->getLastError() << std::endl;
	if (shader2d->getLastError() != eng::gfx::Shader::Error::NO_ERROR)
		std::cout << "Shader 2d error log: " << shader2d->getLastErrorLog() << std::endl;

	eng::gfx::LightManager::initialize();
	eng::sys::EventManager::initialize(window.getHandler());

	std::string resources_dir = "E:/Programming/Projects/C++/Engine/tests/resources";
	eng::gfx::Texture* tex[12];
	std::vector<std::string>images = {
		"/image1.png",
		"/image2.png",
		"/bricks.jpg",
		"/box-map.png",
		"/bricks-normal.jpg",
		"/bricks-parallax.jpg",
		"/bricks/redbricks2b-albedo.png",
		"/bricks/redbricks2b-normal.png",
		"/bricks/redbricks2b-metalness.png",
		"/bricks/redbricks2b-rough.png",
		"/bricks/redbricks2b-height4b.png",
		"/bricks/redbricks2b-ao.png"
	};

	std::cout << "TEXTURES\n";
	for (unsigned int i = 0; i < images.size(); i++)
	{
		auto load_result = eng::core::ResourceManager::load<eng::gfx::Texture>({resources_dir + images[i]});
		tex[i] = load_result.second;
		std::cout << "Tex" << i << ": " << tex[i]->getLastError() << std::endl;
	}

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
		tex[6],
		tex[7],
		tex[8],
		tex[9],
		tex[10],
		tex[11],
		nullptr
	});

	float obj_size = 1;
	eng::gfx::GeometricMesh obj(eng::gfx::GeometricMesh::Type::PARALLELEPIPED);
	//eng::gfx::GeometricMesh obj(eng::gfx::GeometricMesh::Type::ELLIPSOID);
	obj.setSize(eng::mth::Vec3(1.5, 1.5, 1.5));
	obj.setOrigin(eng::mth::Vec3(0, -2.5, 0));
	//obj.setAccuracy(500);
	obj.setMaterial({
		tex[6],
		tex[7],
		tex[8],
		tex[9],
		tex[10],
		tex[11],
		nullptr
	});

	eng::gfx::RenderStates states;
	eng::gfx::View view3d;
	//view3d.setOrtho(0, 10, 10, 0, -30, 30);
	view3d.setPerspective(3.14*0.25, float(width)/height, 1, 100);
	view3d.setPosition(eng::mth::Vec3(0, 1, 2));
	eng::gfx::View::setActive3d(&view3d);
	eng::gfx::View view2d;
	view2d.setOrtho(0, width, height, 0, -10, 10);
	eng::gfx::View::setActive2d(&view2d);

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
	shape.setPosition(eng::mth::Vec2(700, 400));
	shape.setSize(eng::mth::Vec2(100, 100));
	//shape.setTexture(*font.getTexture());
	shape.setTexture(*tex[2]);

	eng::mth::Transformable3 parent;
	obj.setParent(parent);

	std::vector<eng::gfx::RenderScene::RenderPass> pipeline2d = {
		{
			shader2d,
			{eng::gfx::Texture::PixelFormat::RGBA},
			{},
			nullptr
		}
	};

	std::vector<eng::gfx::RenderScene::RenderPass> pipeline3d = {
		{
			shader3d_deferred,
			{
				eng::gfx::Texture::PixelFormat::RGBA32F, //position
				eng::gfx::Texture::PixelFormat::RGBA32F, //normal
				eng::gfx::Texture::PixelFormat::RGBA, //albedo
				eng::gfx::Texture::PixelFormat::RGBA, //met rog hei ao
				eng::gfx::Texture::PixelFormat::RGBA //emission
			},
			{},
			nullptr
		},
		{
			shader2d_deferred_light,
			{},
			{
				"uPosition",
				"uNormal",
				"uAlbedo"
			},
			[&](eng::gfx::Shader* sh){
				eng::gfx::View* active_view = &view3d;
				eng::mth::Vec3 view_loc_pos = active_view->getPosition();
				eng::mth::Vec4 view_glob_pos = active_view->getGlobalTransform().getMatrix()*eng::mth::Vec4(view_loc_pos.x, view_loc_pos.y, view_loc_pos.z, 1);
				sh->setUniform3fv("uViewPos", 1, &view_glob_pos.x);

				eng::gfx::LightManager::DirectionalLight light = eng::gfx::LightManager::getDirectionalLight();
				bool use_directional_light = light.direction.x || light.direction.y || light.direction.z;
				sh->setUniform1i("uUseDirectionalLight", use_directional_light);
				if (use_directional_light)
				{
					sh->setUniform3fv("uDirectionalLight.direction", 1, &light.direction.x);
					sh->setUniform3fv("uDirectionalLight.color", 1, &light.color.x);
				}
			}
		}
	};
	eng::gfx::RenderScene rs;
	rs.setClearColor(eng::gfx::Color(0));

	rs.setRenderPipeline2d(pipeline2d);
	rs.setRenderPipeline3d(pipeline3d);

	rs.addObject(shape);
	rs.addObject(info_texts[0]);
	rs.addObject(info_texts[1]);

	rs.addObject3d(floor);
	rs.addObject3d(obj);

	float speed = 0.1;
	eng::mth::Vec2 rot_angles;
	float delta_time = 0;
	float light_rot = 0;

	eng::gfx::View::setActive3d(&view3d);
	eng::gfx::View::setActive2d(&view2d);

	std::cout << "START" << std::endl;
	while(window.isOpen())
	{
		float start_time = glfwGetTime();
		eng::mth::Vec3 vel;
		eng::sys::EventManager::pull();
		if (eng::sys::EventManager::isPressed(GLFW_KEY_ESCAPE)) window.close();
		if (eng::sys::EventManager::isJustPressed(GLFW_KEY_R)) tex[5]->setSmooth(false);
		if (eng::sys::EventManager::isJustReleased(GLFW_KEY_R)) tex[5]->setSmooth(true);
		if (eng::sys::EventManager::isJustPressed(GLFW_KEY_L)) eng::sys::EventManager::setCursorLock(!eng::sys::EventManager::getCursorLock());
		if (eng::sys::EventManager::isPressed(GLFW_KEY_W))
			vel.z -= speed;
		if (eng::sys::EventManager::isPressed(GLFW_KEY_S))
			vel.z += speed;
		if (eng::sys::EventManager::isPressed(GLFW_KEY_A))
			vel.x -= speed;
		if (eng::sys::EventManager::isPressed(GLFW_KEY_D))
			vel.x += speed;
		if (eng::sys::EventManager::isPressed(GLFW_KEY_LEFT_CONTROL))
			vel.y -= speed;
		if (eng::sys::EventManager::isPressed(GLFW_KEY_SPACE))
			vel.y += speed;
		if (eng::sys::EventManager::isPressed(GLFW_KEY_RIGHT))
			light_rot += 0.05;
		if (eng::sys::EventManager::isPressed(GLFW_KEY_LEFT))
			light_rot -= 0.05;

		if (eng::sys::EventManager::Mouse::moved())
			rot_angles = rot_angles + eng::sys::EventManager::Mouse::getDelta();
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


		rs.render(window);
		window.display();

		delta_time = glfwGetTime() - start_time;
		std::cout << delta_time << std::endl;
	}

	eng::gfx::LightManager::finalize();

	window.destroy();
	system("pause");
	return 0;
}