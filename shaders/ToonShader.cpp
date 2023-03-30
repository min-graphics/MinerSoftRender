#include"../core/shader.h"
//1
shader_struct_v2f ToonShader::vertex(shader_struct_a2v* a2v)
{
	shader_struct_v2f v2f;
	v2f.clip_pos = ObjectToClipPos(a2v->obj_pos);
	v2f.intensity = saturate(ObjectToWorldNormal(a2v->obj_normal) * WorldLightDir());
	return v2f;
}
bool ToonShader::fragment(shader_struct_v2f* v2f,Color& color)
{
	float intensity = v2f->intensity;
	if (intensity > .85)intensity = 1;
	else if (intensity > .60)intensity = .80;
	else if (intensity > .45)intensity = .60;
	else if (intensity > .30)intensity = .45;
	else if (intensity > .15)intensity = .30;

	color = Color(1,155/255.f,0) * intensity;
	return false;
}