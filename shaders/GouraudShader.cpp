#include"../core/shader.h"

//接受两个变量（面序号，顶点序号）
shader_struct_v2f GouraudShader::vertex(shader_struct_a2v* a2v)
{
	shader_struct_v2f v2f;

	v2f.clip_pos = ObjectToClipPos(a2v->obj_pos);
	v2f.intensity = (ObjectToWorldNormal(a2v->obj_normal) * WorldLightDir());
	return v2f;
}
//根据传入的质心坐标，颜色，以及varying_intensity计算出当前像素的颜色
bool GouraudShader::fragment(shader_struct_v2f* v2f,Color& color)
{
	color = Color::White * v2f->intensity;
	return false;
}