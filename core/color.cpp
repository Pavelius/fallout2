#include "f2lib.h"

color::operator const char*() const
{
	static char temp[32];
	szprint(temp, "%1i, %2i, %3i", rgb.r, rgb.g, rgb.b);
	return temp;
}

color color::gray() const
{
	color	result;
	int		level = (rgb.r + rgb.g + rgb.b) / 3;
	result.rgb.r = level;
	result.rgb.g = level;
	result.rgb.b = level;
	result.rgb.a = rgb.a;
	return result;
}

color color::lighten() const
{
	int		n;
	color	result;
	n = rgb.r + (rgb.r >> 2);
	if(n > 255)
		result.rgb.r = 255;
	else
		result.rgb.r = n;
	n = rgb.g + (rgb.g >> 2);
	if(n > 255)
		result.rgb.g = 255;
	else
		result.rgb.g = n;
	n = rgb.b + (rgb.b >> 2);
	if(n > 255)
		result.rgb.b = 255;
	else
		result.rgb.b = n;
	result.rgb.a = rgb.a;
	return result;
}

color color::darken() const
{
	color result;
	result.rgb.r = rgb.r - (rgb.r >> 3);
	result.rgb.g = rgb.g - (rgb.g >> 3);
	result.rgb.b = rgb.b - (rgb.b >> 3);
	result.rgb.a = rgb.a;
	return result;
}

color color::bright(int percent) const
{
	color result;
	int m = (256*(percent-100))/100;
	result.rgb.r = imax(0, imin(rgb.r+m, 255));
	result.rgb.g = imax(0, imin(rgb.g+m, 255));
	result.rgb.b = imax(0, imin(rgb.b+m, 255));
	result.rgb.a = rgb.a;
	return result;
}

color color::mix(const color c1, unsigned char a) const
{
	color result;
	result.rgb.r = (((int)rgb.r*a) + (c1.rgb.r*(255-a)))>>8;
	result.rgb.g = (((int)rgb.g*a) + (c1.rgb.g*(255-a)))>>8;
	result.rgb.b = (((int)rgb.b*a) + (c1.rgb.b*(255-a)))>>8;
	result.rgb.a = rgb.a;
	return result;
}

color color::negative() const
{
	color result;
	result.rgb.r = ~rgb.r;
	result.rgb.g = ~rgb.g;
	result.rgb.b = ~rgb.b;
	result.rgb.a = ~rgb.a;
	return result;
}

unsigned char color::index() const
{
	unsigned char* p = (unsigned char*)res::get(res::COLOR)+768;
	return p[((rgb.r>>3)<<10)|((rgb.g>>3)<<5)|(rgb.b>>3)];
}

color color::get(unsigned char index)
{
	color result;
	unsigned char* p = (unsigned char*)res::get(res::COLOR) + index*3;
	result.rgb.r = p[0]<<2;
	result.rgb.g = p[1]<<2;
	result.rgb.b = p[2]<<2;
	result.rgb.a = 0;
	return result;
}