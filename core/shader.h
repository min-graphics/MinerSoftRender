#ifndef __MIN_SHADER_H__
#define __MIN_SHADER_H__
//1
#include"../model/tgaimage.h"
#include"renderbuffer.h"
#include"pch.h"

struct Material
{
	TGAImage* diffuse_map;
	TGAImage* normal_map;
	TGAImage* specular_map;
	Color color;
	Color specular;
	float gloss;
	float bump_scale;
};

struct ShaderData
{
	Material* matrial;
	RenderBuffer* targetBuffer;
	RenderBuffer* shadow_map;
	bool enable_shadow;
	Vector3f view_pos;
	Vector3f light_dir;
	Color light_color;
	Color ambient;
	Matrix4 model_matrix;
	Matrix4 model_matrix_I;
	Matrix4 view_matrix;
	Matrix4 projection_matrix;
	Matrix4 light_vp_matrix;
	Matrix4 camera_vp_matrix;
};

struct shader_struct_a2v
{
	Vector3f obj_pos;
	Vector3f obj_normal;
	Vector2f uv;
};

struct shader_struct_v2f 
{
	Vector4f clip_pos;
	Vector3f world_pos;
	Vector3f world_normal;
	Vector2f uv;
	float intensity;
};
struct IShader
{
	ShaderData* shader_data;
	
	virtual shader_struct_v2f vertex(shader_struct_a2v* a2v) = 0;
	virtual bool fragment(shader_struct_v2f* v2f,Color& color) = 0;

	Vector4f ObjectToClipPos(Vector3f pos)
	{
		return shader_data->camera_vp_matrix * shader_data->model_matrix * embed<4>(pos);
	}
	Vector4f ObjectToViewPos(Vector3f pos)
	{
		return shader_data->light_vp_matrix * shader_data->model_matrix * embed<4>(pos);
	}
	Vector3f ObjectToWorldPos(Vector3f pos)
	{
		return proj<3>(shader_data->model_matrix * embed<4>(pos));
	}
	Vector3f ObjectToWorldDir(Vector3f dir) 
	{
		return proj<3>(shader_data->model_matrix * embed<4>(dir, 0.f));
	}
	Vector3f ObjectToWorldNormal(Vector3f normal)
	{
		Matrix<1, 4, float> m_normal;
		m_normal[0] = embed<4>(normal);
		return proj<3>((m_normal * shader_data->model_matrix_I)[0]);
	}
	Vector3f WorldSpaceViewDir(Vector3f worldPos)
	{
		return shader_data->view_pos - worldPos;
	}

	Vector3f WorldLightDir()
	{
		return shader_data->light_dir;
	}

	Color tex_diffuse(const Vector2f& uv)
	{
		TGAImage* tex = shader_data->matrial->diffuse_map;
		Vector2i _uv(uv[0] * tex->get_width(), uv[1] * tex->get_height());
		return (Color)(tex->get(_uv[0], _uv[1]));
	}

	Vector3f tex_normal(const Vector2f& uv)
	{
		TGAImage* tex = shader_data->matrial->normal_map;
		Vector2i _uv(uv[0] * tex->get_width(), uv[1] * tex->get_height());
		TGAColor c = tex->get(_uv[0], _uv[1]);//这里采样时，如果把类型换成color，整体会变暗很多
		Vector3f res;
		for (int i = 0; i < 3; i++)
		{
			res[2 - i] = (float)c[i] / 255.f * 2.f - 1.f;//(从0-255转换到-1,1区间内),但是这里是倒着取的
		}
		return res;
	}
	float tex_specular(const Vector2f& uv)
	{
		TGAImage* tex = shader_data->matrial->specular_map;
		Vector2i _uv(uv[0] * tex->get_width(), uv[1] * tex->get_height());
		return tex->get(_uv[0], _uv[1])[0] / 1.f;
	}
	int is_in_shadow(Vector4f depth_pos, float n_dot_l)
	{
		if (shader_data->enable_shadow && shader_data->shadow_map)
		{
			float width = shader_data->shadow_map->width;
			float height = shader_data->shadow_map->height;
			
			Vector3f ndc_coords;
			ndc_coords = proj<3>(depth_pos / depth_pos[3]);//透视除法
			Vector3f pos = viewport_transform(width, height, ndc_coords);
			float depth_bias = 0.05f * (1 - n_dot_l);
			if (depth_bias < 0.005f)depth_bias = 0.01f;
			float current_depth = depth_pos[2] - depth_bias;

			if (pos.x < 0 || pos.y < 0 || pos.x >= width || pos.y >= height)
			{
				return 1;
			}

			float closest_depth = shader_data->shadow_map->get_color(pos.x, pos.y)[0];
			return current_depth < closest_depth;
		}
		return 1;
	}
};

struct PhongShader :public IShader
{

	virtual shader_struct_v2f vertex(shader_struct_a2v* a2v) override;
	virtual bool fragment(shader_struct_v2f* v2f, Color& color) override;
	//~PhongShader();
};
struct ToonShader :public IShader
{
	virtual shader_struct_v2f vertex(shader_struct_a2v* a2v) override;
	virtual bool fragment(shader_struct_v2f* v2f, Color& color) override;
};

struct GouraudShader :public IShader
{
	//顶点着色器会将数据写入varying_intensity
	//片元着色器从varying_intensity中读取数
	
	virtual shader_struct_v2f vertex(shader_struct_a2v* a2v) override;
	virtual bool fragment(shader_struct_v2f* v2f, Color& color) override;
};

struct FlatShader :public IShader
{
	//三个点的信息
	virtual shader_struct_v2f vertex(shader_struct_a2v* a2v) override;
	virtual bool fragment(shader_struct_v2f* v2f, Color& color) override;
};

struct ShadowShader :public IShader
{
	virtual shader_struct_v2f vertex(shader_struct_a2v* a2v) override;
	virtual bool fragment(shader_struct_v2f* v2f, Color& color) override;
};

struct NormalMapShader :public IShader
{
	virtual shader_struct_v2f vertex(shader_struct_a2v* a2v) override;
	virtual bool fragment(shader_struct_v2f* v2f, Color& color) override;
};
struct TextureWithLightShader :public IShader
{
	virtual shader_struct_v2f vertex(shader_struct_a2v* a2v) override;
	virtual bool fragment(shader_struct_v2f* v2f, Color& color) override;
};
struct TextureShader :public IShader
{
	virtual shader_struct_v2f vertex(shader_struct_a2v* a2v) override;
	virtual bool fragment(shader_struct_v2f* v2f, Color& color) override;
};


#endif // !__OUR_GL_H__
