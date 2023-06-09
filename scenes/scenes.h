#pragma once
#include"../core/pch.h"
#include"gameobject.h"
#include"../core/camera.h"
#include"../platform/platform.h"
//1
static Color AMBIENT = Color(54.f / 255, 58.f / 255, 66.f / 255);
static Color LightColor = Color(255.f / 255, 244.f / 255, 214.f / 255);

class DrawModel
{
public:
	GameObject* light;
	GameObject_StaticModel* gameobject;
	Material* material;
	ShaderData* shader_data;
	DrawData* shadow_draw_data;
	DrawData* draw_data;
	RenderBuffer* shadow_map;
	IShader* shader;
	ShadowShader* shadow_shader;

	DrawModel(GameObject* light, GameObject_StaticModel* go, Material* material, IShader* shader)
	{
		this->light = light;
		this->gameobject = go;
		this->material = material;
		this->shader = shader;

		shader_data = new ShaderData();
		shader_data->matrial = material;

		shadow_draw_data = nullptr;
		shadow_shader = nullptr;
		shadow_map = nullptr;

		draw_data = new DrawData();
		draw_data->model = gameobject->model;
		draw_data->shader = shader;
		draw_data->shader->shader_data = shader_data;
	}
	~DrawModel()
	{
		delete shader_data;
		delete draw_data;
		if (shadow_draw_data) delete shadow_draw_data;
		if (shadow_map) delete shadow_map;
		if (shadow_shader) delete shadow_shader;
	}
	void draw(Camera* camera, RenderBuffer* frameBuffer, bool enable_shadow)
	{
		Matrix4 view_matrix = camera->get_view_matrix();
		Matrix4 projection_matrix = camera->get_proj_matrix();
		Matrix4 model_matrix = gameobject->GetModelMatrix();
		Matrix4 model_matrix_I = model_matrix.invert();

		shader_data->view_pos = camera->get_position();
		shader_data->light_dir = (light->transform.position - camera->get_target_position()).normalize();
		shader_data->light_color = LightColor;
		shader_data->ambient = AMBIENT;
		shader_data->model_matrix = model_matrix;
		shader_data->model_matrix_I = model_matrix_I;
		shader_data->view_matrix = view_matrix;
		shader_data->projection_matrix = projection_matrix;
		float aspect = (float)frameBuffer->width / (float)frameBuffer->height;
		shader_data->light_vp_matrix = orthographic(aspect, 1, 0, 5) * lookat(light->transform.position, camera->get_target_position(), Vector3f(0, 1, 0));
		shader_data->camera_vp_matrix = projection_matrix * view_matrix;
		shader_data->enable_shadow = enable_shadow;

		if (enable_shadow)
		{
			if (!shadow_draw_data)
			{
				shadow_shader = new ShadowShader();
				shadow_map = new RenderBuffer(frameBuffer->width, frameBuffer->height);
				shadow_draw_data = new DrawData();
				shadow_draw_data->model = gameobject->model;
				shadow_draw_data->shader = shadow_shader;
				shadow_draw_data->shader->shader_data = shader_data;
				shadow_draw_data->render_buffer = shadow_map;

			}
			graphics_draw_triangle(shadow_draw_data);
			shader_data->shadow_map = shadow_map;
		}
		draw_data->render_buffer = frameBuffer;
		graphics_draw_triangle(draw_data);

		#undef max()
		if (enable_shadow)
		{
			shadow_map->renderbuffer_clear_color(Color::Black);
			shadow_map->renderbuffer_clear_depth(std::numeric_limits<float>::max());
		}
	}
};

struct ShaderInfo
{
	const char* des;
	IShader* shader;
};

class Scene
{
public:
	GameObject* light;
	Camera* camera;
	RenderBuffer* frameBuffer;
	char text[500];
	bool enable_shadow;
	Scene(RenderBuffer* frameBuffer);
	~Scene();

	virtual void tick(float delta_time);
	virtual void on_key_input(keycode_t key,int pressed);
	char* get_text();
};

class SingleModelScene :public Scene
{
private:
	GameObject_StaticModel* gameobject;
	Material* material;
	ShaderInfo shaderInfos[6];
	DrawModel* draw_model;
	int cur_shader_index;
public:
	SingleModelScene(const char* file_name, RenderBuffer* render_Buffer);
	~SingleModelScene();

	void tick(float delta_time) override;
	void on_key_input(keycode_t, int pressed) override;
};