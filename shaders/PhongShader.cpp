#include"../core/shader.h"
//1
shader_struct_v2f PhongShader::vertex(shader_struct_a2v* a2v)
{
	shader_struct_v2f v2f;
	v2f.clip_pos = ObjectToClipPos(a2v->obj_pos);
	v2f.world_pos = ObjectToWorldPos(a2v->obj_pos);
	v2f.world_normal = ObjectToWorldNormal(a2v->obj_normal);

	v2f.uv = a2v->uv;
	return v2f;
}

bool PhongShader::fragment(shader_struct_v2f* v2f, Color& color)
{
	Vector3f worldNormalDir = (v2f->world_normal).normalize();
	Color albedo = tex_diffuse(v2f->uv) * shader_data->matrial->color;
	Color ambient = shader_data->ambient * albedo;

	float n_dot_l = saturate(worldNormalDir * WorldLightDir());
	Color diffuse = shader_data->light_color * albedo * n_dot_l;
	Vector3f viewDir = WorldSpaceViewDir(v2f->world_pos).normalize();
	Vector3f halfDir = (viewDir + WorldLightDir()).normalize();
	Color specular = shader_data->light_color * shader_data->matrial->specular * std::pow(saturate(worldNormalDir * halfDir), shader_data->matrial->gloss);
	
	Vector4f depth_pos = shader_data->light_vp_matrix * embed<4>(v2f->world_pos);
	int shadow = is_in_shadow(depth_pos, n_dot_l);

	color = ambient + (diffuse + specular) * shadow;
	return false;
}