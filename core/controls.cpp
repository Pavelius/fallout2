#include "f2lib.h"

int			hot::key;
point		hot::mouse;
bool		hot::pressed;
char		hot::symbol;
int			hot::command;
unsigned	hot::frame;

areas ui::area(int id, const rect& rc)
{
	if(!hot::mouse.in(rc))
		return AreaNormal;
	if(hot::pressed)
		return AreaPressed;
	return AreaHilite;
}

void ui::radio(int x, int y, int id, int frame, int key, unsigned state)
{
	rect rc = {x, y,
		x + res::gsx(res::INTRFACE, frame) - 1,
		y + res::gsy(res::INTRFACE, frame) - 1};
	areas a = area(id, rc);
	if((key && hot::key==key)
		|| (hot::key==MouseLeft && !hot::pressed && a==AreaHilite))
		execute(id);
	if(state&Checked)
		a = AreaPressed;
	switch(a)
	{
	case AreaPressed:
		image(x, y, res::INTRFACE, frame+1, FINoOffset);
		break;
	default:
		image(x, y, res::INTRFACE, frame, FINoOffset);
		break;
	}
}

bool ui::buttonf(int x, int y, int id, int normal, int pressed, bool action, unsigned state, areas* ra, rect* rrc)
{
	bool result = false;
	rect rc = {x, y,
		x + res::gsx(res::INTRFACE, normal) - 1,
		y + res::gsy(res::INTRFACE, normal) - 1};
	areas a = area(id, rc);
	if(hot::key==MouseLeft && (hot::pressed==action) && (a==AreaHilite || a==AreaPressed))
		result = true;
	if(state&Checked)
		a = AreaPressed;
	if(ra)
		*ra = a;
	if(rrc)
		*rrc = rc;
	switch(a)
	{
	case AreaPressed:
		image(x, y, res::INTRFACE, pressed, FINoOffset);
		break;
	default:
		image(x, y, res::INTRFACE, normal, FINoOffset);
		break;
	}
	return result;
}

void ui::execute(int id)
{
	hot::command = id;
}

void ui::number(int x, int y, int digits, int id, int value)
{
	states push;
	char temp[16];
	int fid = 170;
	int n = 0;
	if(value<0)
	{
		n = 12;
		value = -value;
	}
	sznum(temp, value, digits, 0);
	for(int i = 0; i<digits; i++)
	{
		clipping.x1 = x+i*14;
		clipping.x2 = x+(i+1)*14;
		clipping.y1 = y;
		clipping.y2 = y+24;
		image(x+i*14-(temp[i]-'0'+n)*14, y, res::INTRFACE, 170, FINoOffset);
	}
}

void ui::button(int x, int y, int width, int id, const char* string, int key, int state)
{
	if(!string)
		string = gstr(id);
	states push;
	scol(ColorButton);
	font = res::FONT3;
	int fid = 8;
	int sx = res::gsx(res::INTRFACE, fid);
	int sy = res::gvar(font, 0);
	int w1 = width - sx;
	text(x + sx + (w1-textw(string))/2, y, string);
	rect rc = {x, y, x + width, y + sy};
	areas a = area(id, rc);
	if((key && hot::key==key)
		|| (hot::key==MouseLeft && !hot::pressed && a==AreaHilite))
		execute(id);
	if(state&Checked)
		a = AreaPressed;
	switch(a)
	{
	case AreaPressed:
		image(x, y+1, res::INTRFACE, fid+1, FINoOffset);
		break;
	default:
		image(x, y+1, res::INTRFACE, fid, FINoOffset);
		break;
	}
	//rectb(rc.x1, rc.y1, rc.x2, rc.y2, ColorWhite);
}

void ui::buttonp(int x, int y, int id, int normal, int pressed, const char* string, int ty, unsigned state)
{
	areas a;
	rect rc;
	if(buttonf(x, y, id, normal, pressed, false, state, &a, &rc))
		execute(id);
	int w1 = textw(string);
	if(a==AreaPressed)
	{
		x++;
		y++;
	}
	font = res::FONT3;
	scol(ColorButton);
	text(x + (rc.width()-textw(string))/2,
		y + ty,
		string);
}

void ui::tumbler(int x, int y, int n)
{
	states push;
	clipping.x1 = x;
	clipping.y1 = y;
	clipping.x2 = x + res::gsx(res::INTRFACE, 242);
	clipping.y2 = y + 46;
	image(x, y - n*47, res::INTRFACE, 242, FINoOffset);
}

void ui::shortcut(int key, int cmd)
{
	if(hot::key==key)
	{
		execute(cmd);
		hot::key = 0;
	}
}

void ui::debuging(int ox, int oy)
{
	char temp[64]; szprint(temp, "%1i, %2i", hot::mouse.x - ox, hot::mouse.y - oy);
	text(0, 0, temp);
}