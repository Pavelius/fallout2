#include "f2lib.h"

using namespace ui;

static int current_premade = PremadeCombat;

static void biography(int x1, int y1, int pid)
{
	char temp[260];
	const char* d = premade::gets(pid, Description);
	if(!d)
		return;
	szprint(temp, szt("Биография %1"), sznameof(premade::gets(pid, Name)));
	text(x1, y1, temp);
	y1 += 30;
	text(x1, y1, 160, d);
}

int ui::dialog::newgame()
{
	creatures::create(Player, current_premade);
	while(true)
	{
		font = res::FONT1;
		scol(ColorText);
		image(width/2, height, res::INTRFACE, 174);
		radio(81, 322, Next, 8, 0);
		radio(436, 319, Change, 8, 0);
		radio(81, 424, Create, 8, Alpha + 'N');
		radio(461, 424, Cancel, 8, KeyEscape);
		if(buttonf(292, 320, KeyLeft, 122, 123, false, 0))
			execute(KeyLeft);
		if(buttonf(318, 320, KeyRight, 124, 125, false, 0))
			execute(KeyRight);
		// premade info
		image(width/2, 40+height/2, res::INTRFACE, 201 + (current_premade-PremadeCombat));
		const char* p1 = premade::gets(current_premade, Name);
		text((width-textw(p1))/2, 50, p1);
		biography(width/2+120, 50, current_premade);
		int x1 = 290;
		int y1 = 80;
		int w1 = 70;
		int w2 = 20;
		for(int i = FirstStat; i<=LastStat; i++)
		{
			const char* p = perks::gets(i, ShortName);
			text(x1+w1-8-textw(p), y1, p);
			text(x1+w1, y1, sznumber(creatures::get(Player,i),2));
			text(x1+w1+w2, y1, gstr(FirstStatStr+creatures::get(Player, i)-1));
			y1 += texth();
		}
		y1 += texth();
		static int info1[] = {HP, AC, MaxAP, MeleeDamage};
		for(auto i : info1)
		{
			char temp[64];
			const char* p = perks::gets(i, ShortName);
			text(x1+w1+w2-4-textw(p), y1, p);
			switch(i)
			{
			case HP:
				szprint(temp, "%1i/%2i", creatures::get(Player, MaxHP), creatures::get(Player, MaxHP));
				text(x1+w1+w2, y1, temp);
				break;
			default:
				text(x1+w1+w2, y1, sznumber(creatures::get(Player, i)));
				break;
			}
			y1 += texth();
		}
		y1 += texth();
		// tags
		for(int n = 0; n<4; n++)
		{
			tags i = premade::tag(current_premade, n);
			if(!i)
				continue;
			const char* p = perks::gets(i, ShortName);
			text(x1+w1+w2-4-textw(p), y1, p);
			text(x1+w1+w2, y1, szpercent(creatures::get(Player, i)));
			y1 += texth();
		}
		for(int n = 0; n<2; n++)
		{
			tags i = premade::traits(current_premade, n);
			if(!i)
				continue;
			const char* p = perks::gets(i, Name);
			text(x1+w1+w2-4-textw(p), y1, p);
			y1 += texth();
		}
		cursors::normal();
		int id = input();
		switch(id)
		{
		case 0:
		case Cancel:
			return 0;
		case KeyRight:
			if(current_premade<PremadeDiplomat)
				current_premade++;
			creatures::create(Player, current_premade);
			break;
		case KeyLeft:
			if(current_premade>PremadeCombat)
				current_premade--;
			creatures::create(Player, current_premade);
			break;
		case Change:
			if(dialog::charedit()==Next)
				return Next;
			creatures::create(Player, current_premade);
			break;
		case Create:
			creatures::create(Player, NewCharacter);
			if(dialog::charedit()==Next)
				return Next;
			creatures::create(Player, current_premade);
			break;
		case Next:
			return Next;
		}
	}
}