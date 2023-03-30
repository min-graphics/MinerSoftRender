#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__
//1
#include"../core/renderbuffer.h"
//多包含导致一堆错#include"macro.h"
#include"../model/model.h"
#include"../core/shader.h"

struct DrawData
{
	Model* model;
	IShader* shader;
	RenderBuffer* render_buffer;
};
void graphics_draw_triangle(DrawData* app_data);
#endif // !
