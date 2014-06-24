#include "f2lib.h"

sdat<ui::zsprite,1024> ui::zsprites;

int ui::zsprite::compare(const void* p1, const void* p2, void* param)
{
	return ((zsprite*)p1)->position.y - ((zsprite*)p2)->position.y;
}

void ui::show::begin()
{
	ui::zsprites.clear();
}

void ui::show::end()
{
	xsort(zsprites.data, zsprites.count, sizeof(zsprites.data[0]), zsprite::compare, 0);
	for(int i = 0; i<zsprites.count; i++)
	{
		const auto& e = zsprites.data[i];
		image(e.position.x, e.position.y, e.rid, e.fid, e.flags, e.an);
	}
}