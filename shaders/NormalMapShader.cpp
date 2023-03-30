#include"../core/shader.h"
//1
shader_struct_v2f NormalMapShader::vertex(shader_struct_a2v* a2v) {
	shader_struct_v2f v2f;
	v2f.clip_pos = ObjectToClipPos(a2v->obj_pos);
	v2f.uv = a2v->uv;
	v2f.world_pos = ObjectToWorldPos(a2v->obj_pos);
	v2f.world_normal = ObjectToWorldNormal(a2v->obj_normal);
	return v2f;
}

bool NormalMapShader::fragment(shader_struct_v2f* v2f, Color& color) {
	Vector3f normal = v2f->world_normal;

	float x = normal.x;
	float y = normal.y;
	float z = normal.z;

	Vector3f t = Vector3f(x * y / std::sqrt(x * x + z * z), std::sqrt(x * x + z * z), z * y / std::sqrt(x * x + z * z));
	Vector3f b = cross(normal, t);

	Matrix3 TBN;
	TBN[0] = Vector3f(t.x, b.x, normal.x);
	TBN[1] = Vector3f(t.y, b.y, normal.y);
	TBN[2] = Vector3f(t.z, b.z, normal.z);

	Vector3f bump = tex_normal(v2f->uv);
	bump.x = bump.x * shader_data->matrial->bump_scale;
	bump.y = bump.y * shader_data->matrial->bump_scale;
	bump.z = sqrt(1.0 - saturate(Vector2f(bump.x, bump.y) * Vector2f(bump.x, bump.y)));

	normal = (TBN * bump).normalize();

	Vector3f worldNormalDir = normal;
	Color albedo = tex_diffuse(v2f->uv) * shader_data->matrial->color;
	Color ambient = shader_data->ambient * albedo;
	float n_dot_l = saturate(worldNormalDir * WorldLightDir());
	Color diffuse = shader_data->light_color * albedo * n_dot_l;
	Vector3f viewDir = WorldSpaceViewDir(v2f->world_pos).normalize();
	Vector3f halfDir = (viewDir + WorldLightDir()).normalize();
	Color spcular = shader_data->light_color * shader_data->matrial->specular * std::pow(saturate(worldNormalDir * halfDir), shader_data->matrial->gloss);

	Vector4f depth_pos = shader_data->light_vp_matrix * embed<4>(v2f->world_pos);
	int shadow = is_in_shadow(depth_pos, n_dot_l);

	color = ambient + (diffuse + spcular) * shadow;
	return false;
}