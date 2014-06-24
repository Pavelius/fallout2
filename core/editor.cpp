#include "crt.h"
#include "ui.h"
#include "ai.h"
#include "formats.h"

using namespace ui;

const char* gstr(int id)
{
	return "";
}

const int md = 16;

static void panel(int x1, int y1)
{
	image(x1, y1, res::INTRFACE, 16, FINoOffset);
	buttonf(x1+210, y1+60, Options, 18, 17, false, 0);
	buttonf(x1+265, y1+29, Weapon, 32, 31, false, 0);
}

int ui::main(const char* cmdline)
{
	locale = 0;
	create(0);
	usepal();
	map::create();
	int ti = 132;
	int cp = 0;
	int i1 = map::pos(40, 40);
	int ox = map::t2x(i1)-width/2;
	int oy = map::t2y(i1)-height/2;
	map::swall(i1, 0, 100);
	map::swall(i1+1, 0, 101);
	map::swall(i1+100, 0, 101);
	map::swall(i1+101, 0, 101);
	creatures::set(Player, Action, ActionWalk);
	creatures::set(Player, Position, i1);
	creatures::set(Player, Direction, 2);
	timer(20);
	while(true)
	{
		rectf(0, 0, width, height, 0x34);
		if(hot::pressed)
		{
			i1 = map::xy2t(ox + hot::mouse.x, oy + hot::mouse.y+map::tile_height);
			map::sfloor(i1, ti);
		}
		show::map(ox, oy, 26);
		//panel(0, 381);
		image(4, 4, res::INTRFACE, 74, FINoOffset);
		image(4+50, 4+50, res::TILES, ti);
		text(10, 54, sznumber(ti));
		char temp[100];
		szprint(temp, "%1i, %2i", ox, oy);
		text(0, 460, temp);
		cp = map::xy2t(ox+hot::mouse.x, oy+hot::mouse.y);
		text(600, 460, sznumber(cp));
		cursors::normal();
		switch(input())
		{
		case 0:
			return 0;
		case Alpha+'S': ti++; break;
		case Alpha+'A': ti--; break;
		case KeyDown: oy += md; break;
		case KeyUp: oy -= md; break;
		case KeyRight: ox += md; break;
		case KeyLeft: ox -= md; break;
		case InputTimer: map::animate(); break;
		}
		if(ti<1)
			ti = 1;
	}
}