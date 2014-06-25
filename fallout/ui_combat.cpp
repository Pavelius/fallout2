#include "f2lib.h"

using namespace ui;

static void begin()
{
	unsigned frame = hot::frame;
	while((hot::frame-frame)<20)
	{
		ui::show::mapview();
		image(608, 477, res::INTRFACE, 104, 0, ((hot::frame-frame)/4)%5);
		cursors::wait();
		ui::input();
	}
}

static void end()
{
	unsigned frame = hot::frame;
	while((hot::frame-frame)<20)
	{
		ui::show::mapview();
		image(608, 477, res::INTRFACE, 104, 0, 5 - (((hot::frame-frame)/4)%5));
		cursors::wait();
		ui::input();
	}
}

static void moving(int player)
{
	while(true)
	{
		//text(580, 0, sznumber(index));
		//debuging(0,0);
		//text(580, 0, sznumber(index));
		cursors::normal();
		int id = input();
		switch(id)
		{
		case 0:
			return;
		case Alpha + 'I':
		case Inventory:
			ui::dialog::inventory(Player);
			break;
		case KeyLeft: map::camera.x -= 16; break;
		case KeyRight: map::camera.x += 16; break;
		case KeyUp: map::camera.y -= 16; break;
		case KeyDown: map::camera.y += 16; break;
		case InputTimer: creatures::animate(); break;
		}
	}
}

void ui::dialog::combat()
{
	begin();
	end();
}