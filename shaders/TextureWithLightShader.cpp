#include"../core/shader.h"
//1
shader_struct_v2f TextureWithLightShader::vertex(shader_struct_a2v* a2v)
{
	shader_struct_v2f v2f;
	v2f.clip_pos = ObjectToClipPos(a2v->obj_pos);
	v2f.world_normal = ObjectToWorldNormal(a2v->obj_normal);
	v2f.uv = a2v->uv;

	return v2f;
}
bool TextureWithLightShader::fragment(shader_struct_v2f* v2f, Color& color)
{
	float intensity = saturate(v2f->world_normal * WorldLightDir());
	color = tex_diffuse(v2f->uv) * intensity;

	return false;
}