#include"renderbuffer.h"
//1
RenderBuffer::RenderBuffer(int width,int height)
{
	int color_buffer_size = width * height * 4;
	int depth_buffer_size = width * height * 4;

	Color default_color = TGAColor(0, 0, 0, 1);
	float default_depth = 1.;


	assert(width > 0 && height > 0);

	this->width = width;
	this->height = height;
	this->color_buffer = (unsigned char*)malloc(color_buffer_size);
	this->depth_buffer = (float*)malloc(depth_buffer_size);

	this->renderbuffer_clear_color(default_color);
	this->renderbuffer_clear_depth(default_depth);

}
RenderBuffer::~RenderBuffer()
{
	free(depth_buffer);
	free(color_buffer);
}


void RenderBuffer::set_depth(int x, int y, float depth)
{
	int index = width * y + x;
	depth_buffer[index] = depth;
}
float RenderBuffer::get_depth(int x, int y)
{
	int index = y * width + x;
	return depth_buffer[index];
}

void RenderBuffer::set_color(int x, int y, Color color)
{
	int index = (y * width + x)*4;
	color_buffer[index + 0] = color[0] * 255;
	color_buffer[index + 1] = color[1] * 255;
	color_buffer[index + 2] = color[2] * 255;
}

Color RenderBuffer::get_color(int x, int y)
{
	int index = (y * width + x) * 4;
	return Color(color_buffer[index + 0]/255.f, color_buffer[index + 1]/255.f, color_buffer[index + 2]/255.f, 1);
}

void RenderBuffer::renderbuffer_release()
{
	free(color_buffer);
	free(depth_buffer);
	free(this);
}
void RenderBuffer::renderbuffer_clear_color(Color color)
{
	int numsofPixel = this->width * this->height;
	for (int i = 0; i < numsofPixel; i++)
	{
		color_buffer[i * 4 + 0] = color[0] * 255.f;
		color_buffer[i * 4 + 1] = color[1] * 255.f;
		color_buffer[i * 4 + 2] = color[2] * 255.f;
		color_buffer[i * 4 + 3] = color[3] * 255.f;
	}
}
void RenderBuffer::renderbuffer_clear_depth(float depth)
{
	int numsofPixel = this->width * this->height;
	for (int i = 0; i < numsofPixel; i++)
	{
		depth_buffer[i] = depth;
	}
}