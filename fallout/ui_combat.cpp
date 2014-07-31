#include "f2lib.h"

using namespace ui;

const int wait_time = 3;

static void begin()
{
	unsigned frame = hot::frame;
	while((hot::frame-frame)<(5*wait_time))
	{
		ui::show::mapview();
		image(608, 477, res::INTRFACE, 104, 0, (hot::frame-frame)/wait_time);
		cursors::wait();
		ui::input();
	}
}

static void end()
{
	unsigned frame = hot::frame;
	while((hot::frame-frame)<(5*wait_time))
	{
		ui::show::mapview();
		image(608, 477, res::INTRFACE, 104, 0, 4 - ((hot::frame-frame)/wait_time));
		cursors::wait();
		ui::input();
	}
}

static void moving(int player)
{
	while(true)
	{
		ui::show::mapview();
		image(608, 477, res::INTRFACE, 104, 0, 4);
		cursors::cross();
		int id = input();
		switch(id)
		{
		case 0:
			return;
		case Alpha + 'I':
		case Inventory:
			ui::dialog::inventory(Player);
			break;
		case InputTimer: creatures::animate(); break;
		default:
			if(map::execute(id))
				break;
			break;
		}
	}
}

void ui::dialog::combat()
{
	begin();
	moving(Player);
	end();
}