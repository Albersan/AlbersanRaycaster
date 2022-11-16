#include <stdio.h>

// Calculate The intersection point of two rects given by the coordinates of 4 points, and returns in X Y pointers
void lineLineIntersection(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float& X, float& Y)
{   
	// Line AB represented as a1x + b1y = c1
	float a1 = y2 - y1;
	float b1 = x1 - x2;
	float c1 = a1 * (x1)+b1 * (y1);

	// Line CD represented as a2x + b2y = c2
	float a2 = y4 - y3;
	float b2 = x3 - x4;
	float c2 = a2 * (x3)+b2 * (y3);

	float determinant = a1 * b2 - a2 * b1;

	if (determinant != 0)
	{
		float xa = (b2 * c1 - b1 * c2) / determinant;
		float ya = (a1 * c2 - a2 * c1) / determinant;
		X = xa;
		Y = ya;
	}
}

// split 32bit color into 8 bit r g b a
void unpack_color(const uint32_t& color, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a)
{   
	r = (color >> 0) & 255;
	g = (color >> 8) & 255;
	b = (color >> 16) & 255;
	a = (color >> 24) & 255;
}