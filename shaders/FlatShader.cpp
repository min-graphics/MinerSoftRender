#include"../core/shader.h"

//���Է��������в�ֵ����������Դ�������αߵĲ��
shader_struct_v2f FlatShader::vertex(shader_struct_a2v* a2v)
{
	shader_struct_v2f v2f;

	v2f.clip_pos = embed<4>(a2v->obj_pos);
	v2f.intensity = a2v->obj_normal * WorldLightDir();
	return v2f;
}

bool FlatShader::fragment(shader_struct_v2f* v2f, Color& color)
{
	color = Color::White * v2f->intensity;
	return false;
}