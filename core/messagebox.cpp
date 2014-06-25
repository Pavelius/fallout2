#include "f2lib.h"

void ui::dialog::message(const char* string)
{
	states push;
	int id = 217;
	int dx = 20;
	int sx = res::gsx(res::INTRFACE, id);
	int sy = res::gsy(res::INTRFACE, id);
	int w1 = sx - dx*2;
	int x1 = (width-sx)/2;
	int y1 = (height-sy)/2;
	cursors::background();
	// message window and button
	image(x1, y1, res::INTRFACE, id, FINoOffset);
	image(x1+90, y1+97, res::INTRFACE, 209, FINoOffset);
	// message text
	font = res::FONT1;
	scol(ColorState);
	int h1 = texth(string, w1);
	textm((width-w1)/2, (height-h1)/2 - 8, w1, string);
	// save
	cursors::saveback();
	while(true)
	{
		cursors::background();
		button(x1+100, y1+100, 90, Next);
		cursors::normal(false);
		int id = input();
		switch(id)
		{
		case 0:
		case Cancel:
		case Next:
		case KeyEscape:
		case KeyEnter:
			hot::key = 0;
			hot::command = 0;
			return;
		}
	}
}

bool ui::dialog::gender(int x, int y, int& val)
{
	int val1 = val;
	while(true)
	{
		cursors::background();
		image(x, y, res::INTRFACE, 208, FINoOffset);
		image(x+15, y+42, res::INTRFACE, 209, FINoOffset);
		if(buttonf(x+24, y+2, Male, 212, 213, false, (val1!=Female) ? Checked : 0))
			execute(Male);
		if(buttonf(x+74, y+2, Male, 210, 211, false, (val1==Female) ? Checked : 0))
			execute(Female);
		button(x+25, y+45, 80, Next, 0, KeyEnter);
		cursors::normal(false);
		int id = input();
		switch(id)
		{
		case 0:
		case KeyEscape:
			hot::key = 0;
			hot::command = 0;
			return false;
		case Male:
		case Female:
			val1 = id;
			break;
		case KeyEnter:
		case Next:
			val = val1;
			hot::key = 0;
			hot::command = 0;
			return true;
		}
	}
}

bool ui::dialog::numage(int x, int y, int& val, int minimum, int maximum)
{
	while(true)
	{
		cursors::background();
		image(x, y, res::INTRFACE, 208, FINoOffset);
		image(x+8, y+10, res::INTRFACE, 205, FINoOffset);
		image(x+15, y+40, res::INTRFACE, 209, FINoOffset);
		if(buttonf(x+18, y+14, Left, 122, 123, true, 0) || (hot::key==KeyLeft))
		{
			if(val>minimum)
				val--;
		}
		if(buttonf(x+102, y+14, Left, 124, 125, true, 0) || (hot::key==KeyRight))
		{
			if(val<maximum)
				val++;
		}
		number(x+55, y+11, 2, 1, val);
		button(x+17, y+43, 80, Next, 0, KeyEnter);
		cursors::normal(false);
		int id = input();
		switch(id)
		{
		case 0:
		case KeyEscape:
			hot::key = 0;
			hot::command = 0;
			return false;
		case Next:
		case KeyEnter:
			hot::key = 0;
			hot::command = 0;
			return true;
		}
	}
}

bool ui::dialog::string(int x, int y, char* string, int maxchars)
{
	if(!string)
		return false;
	bool show_cursor = true;
	font = res::FONT1;
	scol(ColorText);
	int w1 = 2;
	int h1 = texth() - res::gvar(font, 2);
	char* p;
	while(true)
	{
		show_cursor = ((hot::frame/4)%2) ? true : false;
		cursors::background();
		image(x, y, res::INTRFACE, 208, FINoOffset);
		image(x+13, y+10, res::INTRFACE, 214, FINoOffset);
		int x1 = x+22;
		int y1 = y+16;
		x1 += text(x1, y1, string);
		if(show_cursor)
			rectf(x1, y1, x1+w1, y1+h1-1, gcol());
		image(x+13, y+40, res::INTRFACE, 209, FINoOffset);
		button(x+25, y+43, 80, Next, 0, KeyEnter);
		cursors::normal(false);
		int id = input();
		switch(id)
		{
		case 0:
		case KeyEscape:
			hot::key = 0;
			hot::command = 0;
			return false;
		case InputSymbol:
			if((unsigned char)hot::symbol>=0x20)
			{
				if(maxchars && szlen(string)>=maxchars)
					break;
				p = szend(string);
				p[0] = hot::symbol;
				p[1] = 0;
			}
			break;
		case KeyBackspace:
			p = szend(string);
			if(p>string)
				p[-1] = 0;
			break;
		case Next:
		case KeyEnter:
			hot::key = 0;
			hot::command = 0;
			return true;
		}
	}
}