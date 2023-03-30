#include"../core/shader.h"
//1
shader_struct_v2f ShadowShader::vertex(shader_struct_a2v* a2v)
{
	shader_struct_v2f v2f;

	v2f.clip_pos = ObjectToViewPos(a2v->obj_pos);
	
	return v2f;
}
bool ShadowShader::fragment(shader_struct_v2f* v2f, Color& color)
{
	float factor = v2f->clip_pos[2];
	color = Color::White * factor;
	return false;
}