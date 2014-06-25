#include "f2lib.h"

using namespace ui;

static unsigned char	buffer[width*height];

void cursors::normal(bool save)
{
	if(save)
		saveback();
	image(hot::mouse.x, hot::mouse.y, res::INTRFACE, 267);
}

void cursors::wait()
{
	image(hot::mouse.x, hot::mouse.y, res::INTRFACE, 295, 0, hot::frame/2);
}

void cursors::arrow(int id)
{
	image(hot::mouse.x, hot::mouse.y, res::INTRFACE, 250);
	if(id!=-1)
	{
		image(hot::mouse.x + res::gsx(res::INTRFACE, 250),
			hot::mouse.y + res::gsy(res::INTRFACE, 250),
			res::INTRFACE,
			id,
			FINoOffset);
	}
}

void cursors::finger()
{
	image(hot::mouse.x, hot::mouse.y, res::INTRFACE, 286);
}

void cursors::background()
{
	rmcpy(ptr(0, 0), buffer, width*height);
}

void cursors::saveback()
{
	rmcpy(buffer, ptr(0, 0), width*height);
}