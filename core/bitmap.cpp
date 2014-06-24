#include "f2lib.h"

#pragma optimize("t", on)

static unsigned char	buffer[ui::height][ui::width];
rect					ui::clipping;
int						ui::scanline;

static inline void correct(int& x1, int& y1, int& x2, int& y2)
{
	if(x1>x2)
		iswap(x1, x2);
	if(y1>y2)
		iswap(y1, y2);
}

static bool correct(int& x1, int& y1, int& x2, int& y2, const struct rect& clip)
{
	correct(x1, y1, x2, y2);
	if(x2<clip.x1 || x1>clip.x2 || y2<clip.y1 || y1>clip.y2)
		return false;
	if(x1<clip.x1)
		x1 = clip.x1;
	if(x2>clip.x2)
		x2 = clip.x2-1;
	if(y1<clip.y1)
		y1 = clip.y1;
	if(y2>clip.y2)
		y2 = clip.y2-1;
	return true;
}

unsigned char* ui::ptr(int x, int y)
{
	return &buffer[y][x];
}

void ui::rectf(int x1, int y1, int x2, int y2, unsigned char color)
{
	while(y1<y2)
	{
		rmset(ptr(x1, y1), color, x2-x1);
		y1++;
	}
}

static void linev(unsigned char* p, int sn, int h, unsigned char color)
{
	while(h--)
	{
		*p = color;
		p += sn;
	}
}

void ui::rectb(int x0, int y0, int x1, int y1, unsigned char color)
{
	line(x0, y0, x1, y0, color);
	line(x1, y0, x1, y1, color);
	line(x0, y1, x1, y1, color);
	line(x0, y0, x0, y1, color);
}

void ui::pixel(int x, int y, unsigned char c)
{
	if(x<clipping.x1 || x>=clipping.x2 || y<clipping.y1 || y>=clipping.y2)
		return;
	*ptr(x, y) = c;
}

void ui::line(int x0, int y0, int x1, int y1, unsigned char color)
{
	if(y0==y1)
	{
		if(!correct(x0, y0, x1, y1, clipping))
			return;
		rmset(ptr(x0, y0), color, x1-x0+1);
	}
	else if(x0==x1)
	{
		if(!correct(x0, y0, x1, y1, clipping))
			return;
		linev(ptr(x0, y0), scanline, y1-y0+1, color);
	}
	else
	{
		int dx = iabs(x1-x0), sx = x0<x1 ? 1 : -1;
		int dy = -iabs(y1-y0), sy = y0<y1 ? 1 : -1;
		int err = dx+dy, e2;
		for(;;)
		{
			pixel(x0, y0, color);
			if(x0==x1 && y0==y1)
				break;
			e2 = 2*err;
			if(e2 >= dy) { err += dy; x0 += sx; }
			if(e2 <= dx) { err += dx; y0 += sy; }
		}
	}
}