#include "f2lib.h"
#include "ai.h"

using namespace ui;

static unsigned*	dragobj;
static int			dragare;
static int			dragpos;
int					modify_by_weapon(int action, int weapon);
static const int	ln = 4;

static void preview(int x, int y, int mid)
{
	states push;
	clipping.x1 = x - 40;
	clipping.x2 = x + 40;
	clipping.y1 = y - 100;
	clipping.y2 = y + 30;
	res::token rs = (res::token)creatures::get(mid, Frame);
	int ac = modify_by_weapon(ActionStand, creatures::get(mid, Weapon));
	int ct = imax(res::gframes(rs, ac)/6, 1);
	image(x, y, rs, ac, FINoCenter, ((hot::frame/2)%6)*ct);
}

void ui::show::item(int x, int y, int sx, int sy, unsigned it, bool resize)
{
	states push;
	if(!it)
		return;
	int fid = items::get(it, Inventory);
	int isx = res::gsx(res::INVEN, fid);
	int isy = res::gsy(res::INVEN, fid);
	if(!resize || (isx<=sx && isy<=sy))
		image(x+(sx-isx)/2, y+(sy-isy)/2, res::INVEN, fid, FINoOffset);
	else
	{
		if(isx>sx)
		{
			isy = (isy*sx)/isx;
			isx = sx;
			clipping.x1 = x;
			clipping.x2 = x + sx;
			clipping.y1 = y + (sy - isy)/2;
			clipping.y2 = clipping.y1 + isy;
		}
		else
		{
			isx = (isx*sy)/isy;
			isy = sy;
			clipping.y1 = y;
			clipping.y2 = y + sy;
			clipping.x1 = x + (sx - isx)/2;
			clipping.x2 = clipping.x1 + isx;
		}
		image(0, 0, res::INVEN, fid, FIScaleToClip);
	}
}

static void itemview(int x, int y, int sx, int sy, int mid, int id, int pos, unsigned* it)
{
	rect rc = {x, y, x+sx, y+sy};
	if(dragobj)
	{
		if(hot::mouse.in(rc))
		{
			dragare = id;
			dragpos = pos;
		}
	}
	//rectb(rc.x1, rc.y1, rc.x2, rc.y2, gcol());
	if(!it || *it==0)
		return;
	if(!dragobj)
	{
		if(hot::pressed && hot::key==MouseLeft && hot::mouse.in(rc))
			dragobj = it;
	}
	ui::show::item(rc.x1+3, rc.y1+3, rc.width()-6, rc.height()-6, *it);
	unsigned c1 = items::get(*it, Count);
	if(c1>1)
	{
		states push;
		scol(ColorCheck);
		char temp[16];
		szprint(temp, "x%1i", c1);
		text(rc.x1, rc.y1, temp);
	}
}

static void iteminfo(int x, int y, int width, int mid, int pos, int mode, unsigned item)
{
	char temp[128];
	int type = items::get(item, Type);
	int melee = items::get(item, MeleeDamage);
	if(!item)
	{
		type = Weapon;
		melee = 1;
		if(pos==0)
			text(x, y, szt("Удар рукой"));
		else
			text(x, y, szt("Удар ногой"));
	}
	else
		text(x, y, items::gets(item, Name));
	y += texth();
	switch(type)
	{
	case Weapon:
		creatures::damage(mid, pos);
		szprint(temp, szt("Повр.: %1i-%2i", "Dam.: %1i-%2i"), items::damage.min, items::damage.max);
		text(x, y, temp);
		if(!melee)
		{
			szprint(temp, szt("длн.: %1i"), items::get(item, Range));
			text(x+width-textw(temp), y, temp);
			y += texth();
			szprint(temp, szt("Заряд: %1i/%2i"), 2, items::get(item, AmmoCapacity));
			text(x, y, temp);
		}
		break;
	}
}

static void itemtext(int x, int y, int width, int mid)
{
	char temp[128];
	states push;
	font = res::FONT1;
	scol(ColorText);
	const char* name = items::gets(mid, Name);
	int h = texth(name, width);
	text(x, y, width, name);
	y += h + ln - 1;
	line(x, y, x + width, y, gcol());
	y += ln + 4;
	y += text(x, y, width, items::gets(mid, Description));
	if(items::get(mid, Type)==Weapon)
	{
		szprint(temp, szt("Минимальная СЛ: %1i", "Minmal ST: %1i"), items::get(mid, Strenght));
		y += text(x, y, width, temp);
	}
	szprint(temp, szt("Это весит %1i фунта.", "It weight %1i pounds"), items::get(mid, CarryWeight));
	y += text(x, y, width, temp);
}

static void information(int x, int y, int width, int mid)
{
	char temp[128];
	static int values[] = {HP, AC,
		DamageNormal, DamageLaser, DamageFire, DamagePlasma, DamageExplosive};
	states push;
	font = res::FONT1;
	scol(ColorText);
	text(x, y, creatures::gets(mid, Name));
	y += texth() + ln - 1;
	line(x, y, x + width, y, gcol());
	y += ln;
	int x1 = x; int y1 = y;
	for(int id = FirstStat; id<=LastStat; id++, y1 += texth())
	{
		text(x1, y1, perks::gets(id, VeryShortName));
		const char* p = sznumber(creatures::get(mid, id));
		text(x1+32-textw(p), y1, p);
	}
	x1 = x + 44; y1 = y;
	for(int id : values)
	{
		char temp[32];
		text(x1, y1, perks::gets(id, ShortName));
		switch(id)
		{
		case HP:
			szprint(temp, "%1i/%2i", creatures::get(mid, HP), creatures::get(mid, MaxHP));
			break;
		case DamageNormal:
		case DamageLaser:
		case DamageFire:
		case DamagePlasma:
		case DamageExplosive:
			sznum(temp, creatures::get(mid, id));
			szcat(temp, "/");
			szcat(temp, szpercent(creatures::get(mid, id-DamageNormal+DamageResistanceNormal)));
			break;
		default:
			sznum(temp, creatures::get(mid, id));
			break;
		}
		text(x1 + 100 - textw(temp), y1, temp);
		y1 += texth();
	}
	y = y1 + ln - 1;
	line(x, y, x + width, y, gcol());
	y += ln+1;
	iteminfo(x, y, width, mid, 0, 0, *creatures::gears(mid, Weapon, 0));
	y += texth()*3 + ln - 1;
	line(x, y, x + width, y, gcol());
	y += ln+1;
	iteminfo(x, y, width, mid, 1, 0, *creatures::gears(mid, Weapon, 1));
	y += texth()*3 + ln - 1;
	line(x, y, x + width, y, gcol());
	y += ln+1;
	szprint(temp, "Общий вес %1i/%2i", creatures::get(mid, EquipmentWeight), creatures::get(mid, CarryWeight));
	text(x+(width-textw(temp))/2, y, temp);
}

static int compare(const void* p1, const void* p2, void* p)
{
	unsigned i1 = **((unsigned**)p1);
	unsigned i2 = **((unsigned**)p2);
	tags t1 = (tags)items::get(i1, Type);
	tags t2 = (tags)items::get(i2, Type);
	if(t2!=t1)
		return t1-t2;
	return szcmp(items::gets(i1, Name), items::gets(i2, Name));
}

void ui::show::inventory(int x1, int y1, int& first, int count, int mid)
{
	const int sx = 60;
	const int sy = 48;
	rect rc = {x1, y1, x1 + sx, y1 + sy*count};
	// Mouse input
	if(hot::mouse.in(rc))
	{
		switch(hot::key)
		{
		case MouseWheelDown: first++; break;
		case MouseWheelUp: first--; break;
		}
		if(dragobj)
		{
			dragare = Inventory;
			dragpos = -1;
		}
	}
	sdat<unsigned*, LastItems-FirstItems+1> source;
	source.count = creatures::invertory(source.data,
		sizeof(source.data)/sizeof(source.data[0]),
		mid);
	xsort(source.data, source.count, sizeof(source.data[0]), compare, 0);
	if(!source.count)
		return;
	// correction
	if(first+count>=source.count)
		first = source.count - count;
	if(first<0)
		first = 0;
	// Up button
	if(first==0)
		image(x1+80, y1-4, res::INTRFACE, 53, FINoOffset);
	else if(buttonf(x1+80, y1-4, Up, 49, 50, false, 0))
		first--;
	// Down button
	if(first+count>=source.count)
		image(x1+80, y1+20, res::INTRFACE, 54, FINoOffset);
	else if(buttonf(x1+80, y1+20, Up, 51, 52, false, 0))
		first++;
	int last = first + count;
	for(int i = first; i<last; i++)
	{
		if(i>=source.count)
			break;
		itemview(x1, y1, sx, sy, mid, Inventory, i, source.data[i]);
		y1 += sy;
	}
}

static void mainview(int x1, int y1, int mid, int mfid, bool isdrag = false, unsigned info = 0)
{
	static int inventory_origin;
	cursors::background();
	image(x1, y1, res::INTRFACE, mfid, FINoOffset);
	preview(x1+206, y1+116, mid);
	itemview(x1+152, y1+184, 92, 60, mid, Armour, 0, creatures::gears(mid, Armour, 0));
	itemview(x1+152, y1+287, 92, 60, mid, Weapon, 0, creatures::gears(mid, Weapon, 0));
	itemview(x1+245, y1+287, 92, 60, mid, Weapon, 1, creatures::gears(mid, Weapon, 1));
	ui::show::inventory(x1+48, y1+46, inventory_origin, 6, mid);
	if(info)
		itemtext(x1+298, y1+48, 142, info);
	else
		information(x1+298, y1+48, 142, mid);
	if(buttonf(x1+432, y1+322, Cancel, 299, 300, false, 0))
		execute(Cancel);
}

static void drag(int x1, int y1, int mid, int mfid)
{
	if(!dragobj)
		return;
	unsigned copy = *dragobj;
	unsigned item = items::pick(*dragobj);
	dragare = 0;
	while(true)
	{
		cursors::background();
		mainview(x1, y1, mid, mfid);
		int fr = items::get(item, Inventory);
		image(hot::mouse.x - res::gsx(res::INVEN, fr)/2,
			hot::mouse.y - res::gsy(res::INVEN, fr)/2,
			res::INVEN, fr, FINoOffset);
		if(!hot::pressed)
		{
			if(dragare)
			{
				if(dragare==Inventory)
					dragpos = -1;
				if(!creatures::add(mid, item, dragare, dragpos))
					*dragobj = copy;
			}
			else
				*dragobj = copy;
			dragobj = 0;
			return;
		}
		else
			input();
	}
}

static void inventiry_menu(int x1, int y1, point pt, int mid)
{
	const int mfid = 48;
	while(true)
	{
		mainview(x1, y1, mid, mfid);
		if(dragobj)
			dragobj = 0;
		if(!hot::pressed)
			return;
		cursors::arrow(259);
		int id = input();
		switch(id)
		{
		case 0:
		case KeyEscape:
			hot::key = 0;
			return;
		}
	}
}

static void inventory_view(int mid)
{
	point pt;
	unsigned item = 0;
	unsigned time = 0;
	const int mfid = 48;
	int x1 = (width-res::gsx(res::INTRFACE, mfid))/2;
	int y1 = height-res::gsy(res::INTRFACE, mfid) - res::gsy(res::INTRFACE, 16) - 4;
	while(true)
	{
		mainview(x1, y1, mid, mfid, false, item);
		if(dragobj)
		{
			item = *dragobj;
			dragobj = 0;
			time = hot::frame;
			pt = hot::mouse;
		}
		if(hot::pressed && time && (hot::frame-time)>=4)
		{
			hot::frame = 0;
			inventiry_menu(x1, y1, hot::mouse, mid);
		}
		cursors::arrow(259);
		int id = input();
		switch(id)
		{
		case 0:
		case Cancel:
		case Alpha + 'I':
		case KeyEscape:
			hot::key = 0;
			return;
		case MouseRight:
			if(!hot::pressed)
				break;
			hot::key = 0;
			return;
		}
	}
}

void ui::dialog::inventory(int mid)
{
	const int mfid = 48;
	int x1 = (width-res::gsx(res::INTRFACE, mfid))/2;
	int y1 = height-res::gsy(res::INTRFACE, mfid) - res::gsy(res::INTRFACE, 16) - 4;
	while(true)
	{
		mainview(x1, y1, mid, mfid);
		if(dragobj)
		{
			drag(x1, y1, mid, mfid);
			continue;
		}
		cursors::finger();
		int id = input();
		switch(id)
		{
		case 0:
		case Cancel:
		case Alpha + 'I':
		case KeyEscape:
			return;
		case MouseRight:
			if(!hot::pressed)
				break;
			hot::key = 0;
			inventory_view(mid);
			break;
		}
	}
}