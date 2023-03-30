#include"../core/shader.h"

//������������������ţ�������ţ�
shader_struct_v2f GouraudShader::vertex(shader_struct_a2v* a2v)
{
	shader_struct_v2f v2f;

	v2f.clip_pos = ObjectToClipPos(a2v->obj_pos);
	v2f.intensity = (ObjectToWorldNormal(a2v->obj_normal) * WorldLightDir());
	return v2f;
}
//���ݴ�����������꣬��ɫ���Լ�varying_intensity�������ǰ���ص���ɫ
bool GouraudShader::fragment(shader_struct_v2f* v2f,Color& color)
{
	color = Color::White * v2f->intensity;
	return false;
}