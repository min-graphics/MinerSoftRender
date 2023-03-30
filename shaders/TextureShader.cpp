#include"../core/shader.h"
//1
shader_struct_v2f TextureShader::vertex(shader_struct_a2v* a2v)
{
	shader_struct_v2f v2f;
	v2f.clip_pos = ObjectToClipPos(a2v->obj_pos);
	v2f.uv = a2v->uv;
	return v2f;
}
bool TextureShader::fragment(shader_struct_v2f* v2f, Color& color)
{
	color = tex_diffuse(v2f->uv);
	return false;
}