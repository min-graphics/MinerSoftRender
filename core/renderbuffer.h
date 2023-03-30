#pragma once
#include"../math/color.h"
//1
class RenderBuffer
{
public:
	RenderBuffer(int width, int height);
	~RenderBuffer();

	int width, height;

	unsigned char* color_buffer;//ÑÕÉ«»º³å
	float* depth_buffer;//Éî¶È»º³å

	void set_depth(int x, int y, float depth);
	float get_depth(int x, int y);
	void set_color(int x, int y, Color color);
	Color get_color(int x, int y);

	void renderbuffer_release();
	void renderbuffer_clear_depth(float depth);
	void renderbuffer_clear_color(Color color);

};