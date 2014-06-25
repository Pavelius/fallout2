#include "f2lib.h"

using namespace ui;

static void field(int id, int num, int key = 0)
{
	const int x1 = 29;
	const int y1 = 19 + num*41;
	ui::radio(x1, y1, id, 299, key);
	const char* name = gstr(id);
	ui::text(x1+95-ui::textw(name)/2, y1+2, name);
}

static void test()
{
	map::create();
	ui::rectf(0, 0, width, height, 0xDC);
	for(int ty = 0; ty<5; ty++)
	{
		for(int tx = 0; tx<5; tx++)
		{
			int t = tx + ty*100;
			image(map::t2x(t)+width/2, map::t2y(t)+height/2+16, res::TILES, 132);
		}
	}
	bool show_hittest = false;
	bool show_grid = false;
	bool show_grid_hittes = false;
	int x1 = map::h2x(0)+width/2;
	int y1 = map::h2y(0)+height/2;
	int an = 0;
	ui::frame* f;
	res::token rs = res::HFCMBT;
	//int ac = 50 + (WeaponTypeSpear-FirstWeaponType)*13;
	int ac = ActionWalk;
	int fc = res::gframes(rs, ac)/6;
	int dr = 3;
	cursors::normal();
	while(true)
	{
		cursors::background();
		if(show_grid_hittes)
		{
			for(int y = 150; y<350; y++)
			{
				for(int x = 250; x<400; x++)
				{
					int i = map::xy2h(x-width/2, y-height/2);
					if(i==-1)
						continue;
					int tx = i%200;
					int ty = i/200;
					*ptr(x, y) = ty*16 + tx;
				}
			}
		}
		if(show_grid)
		{
			for(int ty = 0; ty<10; ty++)
			{
				for(int tx = 0; tx<10; tx++)
				{
					int t = tx + ty*200;
					int x1 = map::h2x(t)+width/2;
					int y1 = map::h2y(t)+height/2;
					image(x1, y1, res::INTRFACE, 1);
					//text(x1+(32-textw(sznumber(t)))/2, y1+(12-texth()/2), sznumber(t));
				}
			}
		}
		if(show_hittest)
		{
			for(int y = 250; y<350; y++)
			{
				for(int x = 250; x<400; x++)
				{
					int i = map::xy2t(x-width/2, y-height/2);
					if(i==-1)
						continue;
					int tx = i%100;
					int ty = i/100;
					*ptr(x, y) = ty*16 + tx;
				}
			}
		}
		int i2 = map::xy2h(x1-width/2, y1-height/2);
		int x2 = map::h2x(i2)+width/2;
		int y2 = map::h2y(i2)+height/2;
		image(x1, y1, rs, ac, FIReal, dr*fc + (an%fc));
		line(x2-4, y2, x2+4, y2, 0xD7);
		line(x2, y2-4, x2, y2+4, 0xD7);
		line(x1-4, y1, x1+4, y1, 0x47);
		line(x1, y1-4, x1, y1+4, 0x47);
		cursors::normal(false);
		int t1 = map::xy2t(hot::mouse.x - width/2, hot::mouse.y - height/2);
		int t2 = map::xy2h(hot::mouse.x - width/2, hot::mouse.y - height/2);
		text(10, 10, sznumber(t1));
		text(600, 10, sznumber(t2));
		int id = input();
		switch(id)
		{
		case 0:
		case KeyEscape:
			return;
		case Alpha+'T': show_hittest = !show_hittest; break;
		case Alpha+'G': show_grid = !show_grid; break;
		case Alpha+'H': show_grid_hittes = !show_grid_hittes; break;
		case InputTimer:
			if((hot::frame%6)==0)
			{
				f = ui::gframe(rs, ac, dr*fc + (an%fc));
				x1 += f->mx;
				y1 += f->my;
				an++;
			}
			break;
		}
	}
}

int ui::main(const char* cmdline)
{
	locale = 0;
	create(szt("Fallout", "Возрождение"));
	timer(70);
	xsrand();
	usepal((unsigned char*)res::get(res::COLOR));
	world::clear();
	creatures::clear();
	while(true)
	{
		ui::font = res::FONT4;
		ui::scol(ColorButton);
		image((width-res::gsx(res::INTRFACE,140))/2,
			(height-res::gsy(res::INTRFACE, 140))/2,
			res::INTRFACE, 140, FINoOffset);
		field(Intro, 0, Alpha + 'H');
		field(New, 1, Alpha + 'N');
		field(Load, 2);
		field(Options, 3, Alpha + 'O');
		field(Credits, 4);
		field(Exit, 5);
		ui::font = res::FONT1;
		ui::text(10, 460, szt("Разработан Павлом Чистяковым, 2014"));
		ui::text(460, 460, szt("FALLOUT II (remake) 0.10"));
		cursors::normal();
		int id = input();
		switch(id)
		{
		case Intro:
			map::create();
			creatures::create(Player, PremadeDiplomat);
			creatures::set(Player, Position, map::cols*2*80+80);
			creatures::set(Player, Direction, 2);
			creatures::set(Player, Action, ActionStand);
			creatures::add(Player, items::create(ArmourCombat));
			creatures::add(Player, items::create(Pistol10mm), Weapon);
			creatures::add(Player, items::create(Knife), Inventory);
			creatures::add(Player, items::create(Knife,3), Inventory);
			creatures::add(Player, items::create(Money, 100), Inventory);
			creatures::add(Player, items::create(ArmourLeather), Inventory);
			creatures::add(Player, items::create(Pistol10mmAuto), Inventory);
			creatures::add(Player, items::create(Rifle), Inventory);
			creatures::add(Player, items::create(Spear), Inventory);
			creatures::add(Player, items::create(FlameThrower), Inventory);
			creatures::add(Player, items::create(ArmourLeatherJacket), Inventory);
			creatures::add(Player, items::create(RifleShootgun), Inventory);
			creatures::add(Player, items::create(CardBlueAccess), Inventory);
			creatures::add(Player, items::create(HandTangler), Inventory);
			creatures::add(Player, items::create(Robe), Inventory);
			creatures::add(Player, items::create(KnifeVibro), Inventory);
			creatures::add(Player, items::create(PsychoNeutralizer), Inventory);
			creatures::add(Player, items::create(Ammo10mmDM, 48), Inventory);
			dialog::game();
			break;
		case New:
			if(dialog::newgame()==Next)
			{
				map::create();
				creatures::set(Player, Position, 100*40+40);
				creatures::set(Player, Direction, 2);
				creatures::set(Player, Action, ActionWalk);
				creatures::add(Player, items::create(Spear));
				dialog::game();
			}
			break;
		case Options:
			test();
			break;
		case Credits:
			ui::dialog::worldmap();
			break;
		case 0:
			return 0;
		}
	}
}