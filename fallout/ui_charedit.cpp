#include "f2lib.h"

using namespace ui;

static int focus;

static void label(int x, int y, int width, int id, const char* temp, unsigned state = 0)
{
	font = res::FONT1;
	rect rc = {x, y, x + width, y + texth()};
	areas a = area(id, rc);
	if(a==AreaPressed && hot::key==MouseLeft && hot::pressed)
		focus = id;
	if(state&Disabled)
		scol(ColorDisable);
	else if(state&Checked)
		scol(ColorCheck);
	else
		scol(ColorText);
	if(focus==id)
		highlight();
	text(x, y, temp);
}

int ui::dialog::charedit()
{
	focus = FastMetabolism;
	int w1, x, y;
	char temp[200];
	while(true)
	{
		image(0, 0, res::INTRFACE, 169, FINoOffset);
		// text labels
		font = res::FONT3;
		scol(ColorButton);
		text(50, 326, szt("Дополнительно"));
		text(18, 286, szt("Очки хар."));
		text(383, 5, szt("Умения"));
		text(428, 233, szt("Навыки"));
		// stats
		font = res::FONT1;
		for(int i = FirstStat; i <= LastStat; ++i)
		{
			rect rc = {102, 37 + 33*(i-FirstStat), 102+42, 46 + 33*(i-FirstStat+1)};
			int val = creatures::getr(Player, i);
			if(buttonf(149, 38 + 33*(i-FirstStat), -1, 193, 194, true, 0))
			{
				if(val<10 && (40-creatures::get(Player, Stats))>0)
					creatures::setr(Player, i, val+1);
				focus = i;
			}
			val = creatures::getr(Player, i);
			if(buttonf(149, 49 + 33*(i-FirstStat), -1, 191, 192, true, 0))
			{
				if(val>1)
					creatures::setr(Player, i, val-1);
				focus = i;
			}
			val = creatures::get(Player, i);
			const char* p = gstr(FirstStatStr + imin(9, val-1));
			label(102, 45 + 33*(i-FirstStat), 42, i, p);
			number(59, 37+33*(i-FirstStat), 2, i, val);
		}
		number(126, 282, 2, StatsLeft, 40-creatures::get(Player, Stats));
		// traits
		font = res::FONT1;
		scol(ColorText);
		for(unsigned int i = 0; i != 16; ++i)
		{
			if(i <= 7)
			{
				radio(23, 352 + 13*i, i+FirstTraits, 215, 0);
				label(48, 353 + 13*i, 100, i+FirstTraits, gstr(i+FirstTraits),
					creatures::get(Player, i+FirstTraits) ? Checked : 0);
			}
			else
			{
				radio(299, 352 + 13*(i-8), i+FirstTraits, 215, 0);
				label(189, 353 + 13*(i-8), 100, i+FirstTraits, gstr(i+FirstTraits),
					creatures::get(Player, i+FirstTraits) ? Checked : 0);
			}
		}
		// skills
		w1 = 240;
		for(unsigned int i = FirstSkill; i <= LastSkill; ++i)
		{
			radio(347, 27 + 11*(i-FirstSkill), i, 215, 0);
			label(377, 27 + 11*(i-FirstSkill), w1, i, gstr(i),
				creatures::istagged(Player, i) ? Checked : 0);
			const char* s1 = szpercent(creatures::get(Player, i));
			text(377+w1-textw(s1), 27 + 11*(i-FirstSkill), s1);
		}
		number(522, 228, 2, TagsLeft, 3-creatures::get(Player, Tags));
		// addition stats
		creatures::set(Player, HP, creatures::get(Player, MaxHP));
		x = 194; y = 46; w1 = 120;
		for(int id = HP; id<=WoundLeftLeg; id++)
		{
			switch(id)
			{
			case HP:
				label(x, y, w1, id, gstr(id));
				szprint(temp, "%1i/%2i", creatures::get(Player, HP), creatures::get(Player, MaxHP));
				text(x+w1-textw(temp), y, temp);
				break;
			default:
				label(x, y, w1, id, gstr(id), Disabled);
				break;
			}
			y += 13;
		}
		// info block 2
		x = 194; y = 182; w1 = 120;
		static int list_info[] = {AC, MaxAP, CarryWeight, MeleeDamage, DamageResistanceNormal, PoisonResistance, RadiationResistance, Initiative, HealthLevel, CriticalHitChance};
		for(auto id : list_info)
		{
			switch(id)
			{
			case DamageResistanceNormal:
			case PoisonResistance:
			case RadiationResistance:
			case CriticalHitChance:
				label(x, y, w1, id, perks::gets(id, ShortName));
				szprint(temp, "%1i%%", creatures::get(Player, id));
				text(x+w1-textw(temp), y, temp);
				break;
			default:
				label(x, y, w1, id, perks::gets(id, ShortName));
				szprint(temp, "%1i", creatures::get(Player, id));
				text(x+w1-textw(temp), y, temp);
				break;
			}
			y += 13;
		}
		// description
		if(focus)
		{
			line(350, 300, 620, 300, 0);
			line(350, 301, 620, 301, 0);
			image(483, 308, res::SKILLDEX, perks::get(focus, Frame), FINoOffset);
			font = res::FONT2;
			scol(ColorInfo);
			int w1 = text(350, 275, gstr(focus));
			if(focus>=FirstSkill && focus<=LastSkill)
			{
				states push;
				font = res::FONT1;
				szprint(temp, szt("Осн: %1i%% + %2"), creatures::getr(Player, focus), perks::gets(focus, Formula));
				text(350 + w1 + 8, 275+14, temp);
			}
			font = res::FONT1;
			text(350, 308, 135, perks::gets(focus, Description));
		}
		// buttons
		button(345, 454, 90, Options);
		button(452, 454, 80, Next, 0, KeyEnter);
		button(552, 454, 80, Cancel, szt("Отказ"), KeyEscape);
		// player buttons
		const char* name = creatures::gets(Player, Name);
		buttonp(13, 0, Name, 185, 184, name[0] ? name : gstr(Empthy));
		buttonp(157, 0, Age, 188, 189, szprint(temp, szt("%1i лет"), creatures::get(Player, Age)));
		buttonp(236, 0, Gender, 188, 189, perks::gets(creatures::get(Player, Gender), ShortName));
		cursors::normal();
		int id = input();
		switch(id)
		{
		case 0:
		case Cancel:
			return 0;
		case Next:
			if(creatures::get(Player, Stats)<40)
			{
				dialog::message("Необходимо распределить все очки характеристик!");
				break;
			}
			if(creatures::get(Player, Stats)>40)
			{
				dialog::message("Вы превысили лимит очков характеристик. Уменьшите некоторые из них.");
				break;
			}
			return Next;
		case KeyDown:
			if(focus==LastStat)
				focus = HP;
			else if(focus==LastSkill)
				focus = FirstStat;
			else if(focus==LastTraits)
				focus = FirstSkill;
			else if(focus==LastTraits)
				focus = FirstSkill;
			else if(focus==MeleeDamage)
				focus = DamageResistanceNormal;
			else if(focus==DamageResistanceNormal)
				focus = PoisonResistance;
			else if(focus==PoisonResistance)
				focus = RadiationResistance;
			else if(focus==RadiationResistance)
				focus = Initiative;
			else if(focus==Initiative)
				focus = HealthLevel;
			else if(focus==CriticalHitChance)
				focus = FirstTraits;
			else if(focus==WoundLeftLeg)
				focus = AC;
			else
				focus++;
			break;
		case KeyUp:
			if(focus==HP)
				focus = LastStat;
			else if(focus==FirstSkill)
				focus = LastTraits;
			else if(focus==FirstTraits)
				focus = CriticalHitChance;
			else if(focus==HealthLevel)
				focus = Initiative;
			else if(focus==Initiative)
				focus = RadiationResistance;
			else if(focus==RadiationResistance)
				focus = PoisonResistance;
			else if(focus==PoisonResistance)
				focus = DamageResistanceNormal;
			else if(focus==DamageResistanceNormal)
				focus = MeleeDamage;
			else if(focus==FirstStat)
				focus = LastSkill;
			else if(focus==AC)
				focus = WoundLeftLeg;
			else
				focus--;
			break;
		case Name:
			szcpy(temp, creatures::gets(Player, Name), sizeof(temp)/sizeof(temp[0]));
			if(dialog::string(22, 0, temp))
				creatures::set(Player, Name, temp);
			break;
		case Age:
			w1 = creatures::get(Player, Age);
			if(dialog::numage(160, 0, w1, 16, 35))
				creatures::set(Player, Age, w1);
			break;
		case Gender:
			w1 = creatures::get(Player, Gender);
			if(dialog::gender(236, 0, w1))
				creatures::set(Player, Gender, w1);
			break;
		default:
			if(id>=FirstTraits && id<=LastTraits)
			{
				if(!creatures::get(Player, id))
				{
					if(creatures::get(Player, Traits)>=2)
						dialog::message(szt("У вас уже есть максимум из двух способностей!"));
					else
						creatures::set(Player, id, 1);
				}
				else
					creatures::set(Player, id, 0);
				focus = id;
			}
			else if(id>=FirstSkill && id<=LastSkill)
			{
				if(!creatures::istagged(Player, id))
				{
					if(creatures::get(Player, Tags)>=3)
						dialog::message(szt("У вас уже есть максимум из трех навыков!"));
					else
						creatures::settag(Player, id, 1);
				}
				else
					creatures::settag(Player, id, 0);
				focus = id;
			}
			break;
		}
	}
}