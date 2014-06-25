#include "f2lib.h"

using namespace ui;
const int dx = 350;
const int dy = 300;

static void draw(int x, int y, int width, int height, int ox, int oy)
{
	states push;
	clipping.x1 = x;
	clipping.x2 = clipping.x1 + width;
	clipping.y1 = y;
	clipping.y2 = clipping.y1 + height;
	for(int iy = 0; iy<5; iy++)
	{
		int y1 = y + dy*iy - oy;
		for(int ix = 0; ix<4; ix++)
		{
			int x1 = x + dx*ix - ox;
			image(x1, y1, res::INTRFACE, 339 + iy*4 + ix, FINoOffset);
		}
	}
}

static void towns(int x, int y, int width, int height)
{
	states push;
	clipping.x1 = x;
	clipping.x2 = clipping.x1 + width;
	clipping.y1 = y;
	clipping.y2 = clipping.y1 + height;
	image(x-8, y, res::INTRFACE, 364, FINoOffset);
}

void ui::dialog::worldmap()
{
	int ox = 100;
	int oy = 100;
	while(true)
	{
		if(ox<0)
			ox = 0;
		if(ox>dx*4-451)
			ox = dx*4-451;
		if(oy<0)
			oy = 0;
		if(oy>dy*5-446)
			oy = dy*5-446;
		draw(21, 21, 451, 446, ox, oy);
		towns(501, 136, 121, 180);
		image(0, 0, res::INTRFACE, 360, FINoOffset);
		image(22, 0, res::INTRFACE, 359, FINoOffset);
		image(width-168, 0, res::INTRFACE, 361, FINoOffset);
		image(494, 331, res::INTRFACE, 366, FINoOffset);
		image(532, 48, res::INTRFACE, 365, FINoOffset, hot::frame);
		image(22, height+8, res::INTRFACE, 359, FINoOffset|FIMirrorV);
		cursors::normal();
		int id = input();
		switch(id)
		{
		case 0:
		case Cancel:
			return;
		case KeyRight: ox += 8; break;
		case KeyLeft: ox -= 8; break;
		case KeyUp: oy -= 8; break;
		case KeyDown: oy += 8; break;
		}
	}
}