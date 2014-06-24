#include "f2lib.h"
#include "ai.h"

using namespace ui;

void console(int x1, int y1, int width, int height, const char* string)
{
	static const char* first;
	static int offset;
	static unsigned last;
	states push;
	font = res::FONT1;
	scol(ColorText);
	if(!first)
	{
		first = string;
		offset = 0;
	}
	clipping.x1 = x1;
	clipping.y1 = y1;
	clipping.x2 = x1 + width;
	clipping.y2 = y1 + height;
	if(last<hot::frame)
	{
		last = hot::frame;
		int h1 = texth(string, width) - offset;
		if(h1>clipping.height())
			offset += 2;
	}
	text(x1, y1-offset, width, string);
}

static void panel_action(int x, int y, int mid)
{
	rect rc = {x, y, x + res::gsx(res::INTRFACE, 32)-4, y + res::gsy(res::INTRFACE, 32)-4};
	buttonf(rc.x1, rc.y1, Weapon, 32, 31, false, 0);
	if(hot::mouse.in(rc) && hot::pressed)
	{
		ui::show::item(rc.x1, rc.y1-2, rc.width(), rc.height(),
			creatures::get(mid, Weapon), false);
	}
	else
	{
		ui::show::item(rc.x1, rc.y1, rc.width(), rc.height(),
			creatures::get(mid, Weapon), false);
	}
}

static void panel(int x1, int y1, int mid)
{
	image(x1, y1, res::INTRFACE, 16, FINoOffset);
	if(buttonf(x1+210, y1+40, Inventory, 47, 46, false, 0))
		execute(Inventory);
	buttonf(x1+210, y1+60, Options, 18, 17, false, 0);
	panel_action(x1+265, y1+29, mid);
	console(x1+28, y1+35, 155, 50, "Ёто длинна€ строка по идее должна разбитьс€ на несколько коротких строчек с переносом. „тобы еще добавить? ќна должна скроллитьс€. »нтеллект отвечает за пам€ть, скорость реакции и способность оценивать событи€. ¬ысокий »нтеллект важен дл€ всех. ¬ли€ет на количество очков умений на уровень, режимы диалога и многие способности.");
}

static void debug(int x, int y)
{
	states push;
	scol(ColorCheck);
	char temp[64];
	szprint(temp, "%1i, %2i = %3i", x, y, map::xy2t(x, y));
	text(10, 10, temp);
}

static void hittest(int ox, int oy, const rect& client, int& index)
{
	if(hot::mouse.in(client))
		index = map::xy2h(ox+hot::mouse.x, oy+hot::mouse.y);
	else
		index = -1;
}

static void order_movement(int mid, int index, bool run)
{
	int pos = creatures::get(mid, Position);
	creatures::set(mid, Action, run ? ActionRun:ActionWalk);
	creatures::set(mid, Order, index);
	creatures::set(mid, Direction, map::d2o(map::direction(pos, index)));
}

int ui::dialog::game()
{
	int index = -1;
	int i1 = map::pos(40, 40);
	int ox = map::t2x(i1)-width/2;
	int oy = map::t2y(i1)-height/2;
	rect rc = {0, 0, width, height - res::gsy(res::INTRFACE, 16)};
	while(true)
	{
		hittest(ox, oy, rc, index);
		ui::show::tiles(ox, oy, 26);
		ui::show::hexagon(ox, oy, index);
		ui::show::begin();
		ui::show::map(ox, oy, 26);
		ui::show::end();
		panel((width-res::gsx(res::INTRFACE, 16))/2, height - res::gsy(res::INTRFACE, 16), Player);
		text(580, 0, sznumber(index));
		cursors::normal();
		int id = input();
		switch(id)
		{
		case 0:
			return 0;
		case Alpha + 'I':
		case Inventory: ui::dialog::inventory(Player); break;
		case KeyLeft: ox -= 16; break;
		case KeyRight: ox += 16; break;
		case KeyUp: oy -= 16; break;
		case KeyDown: oy += 16; break;
		case InputTimer: creatures::animate(); break;
		case MouseLeft:
			if(hot::mouse.in(rc) && hot::pressed)
				order_movement(Player, index, false);
			break;
		case MouseLeft|Shift:
			if(hot::mouse.in(rc) && hot::pressed)
				order_movement(Player, index, true);
			break;
		}
	}
}