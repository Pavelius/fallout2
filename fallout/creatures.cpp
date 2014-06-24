#include "f2lib.h"
#include "ai.h"

static struct object_creature
{
	char				name[32];
	tags				tagged[4];
	unsigned			attr[WoundLeftLeg+1];
	unsigned			traits;
	struct gears		gears;
	unsigned			stop;
	point				position;
	short unsigned		action;
	short unsigned		direction;
	short unsigned		frame;
	short unsigned		flags;
	int					index;
} objects[LastCreature-FirstCreature+1];
static int			last_valid = Player;

struct invetory
{
	unsigned		item;
	int				mid;
};
invetory			inventories[4096];

bool creatures::istagged(int mid, int id)
{
	object_creature& e = objects[mid-FirstCreature];
	return e.tagged[0]==id || e.tagged[1]==id || e.tagged[2]==id || e.tagged[3]==id;
}

void creatures::settag(int mid, int id, int value)
{
	object_creature& e = objects[mid-FirstCreature];
	if(value)
	{
		if(istagged(mid, id))
			return;
		for(auto& i : e.tagged)
		{
			if(!i)
			{
				i = (tags)id;
				return;
			}
		}
	}
	else
	{
		for(auto& i : e.tagged)
		{
			if(i==id)
			{
				i = Empthy;
				return;
			}
		}
	}
}

void creatures::create(int mid, int pid)
{
	static tags copy_fields[] = {Age, Gender};
	rmset(&objects[mid-FirstCreature], 0, sizeof(objects[mid-FirstCreature]));
	creatures::set(mid, Name, premade::gets(pid, Name));
	creatures::set(mid, Race, premade::get(pid, Gender));
	for(auto e : copy_fields)
		creatures::set(mid, e, premade::get(pid, e));
	for(int i = FirstStat; i<=LastStat; i++)
		objects[mid-FirstCreature].attr[i] = premade::get(pid, i);
	for(int i = 0; i<2; i++)
	{
		if(premade::traits(pid, i))
			creatures::set(mid, premade::traits(pid, i), 1);
	}
	for(int i = 0; i<4; i++)
	{
		if(premade::tag(pid, i))
			creatures::settag(mid, premade::tag(pid, i), 1);
	}
	creatures::set(mid, HP, creatures::get(mid, MaxHP));
}

static int gcount(int mid, int t1, int t2)
{
	int r = 0;
	object_creature& e = objects[mid-FirstCreature];
	for(int i = t1; i<=t2; i++)
	{
		if(i<sizeof(objects[0].attr)/sizeof(objects[0].attr[0]))
			r += e.attr[i];
		else
			r += creatures::get(mid, i);
	}
	return r;
}


static int gcountd(int mid, int t1, int t2)
{
	int r = 0;
	const int mc = sizeof(objects[0].attr)/sizeof(objects[0].attr[0]);
	object_creature& e = objects[mid-FirstCreature];
	for(int i = t1; i<=t2; i++)
	{
		if(i<sizeof(objects[0].attr)/sizeof(objects[0].attr[0]))
		{
			if(e.attr[i])
				r++;
		}
		else
		{
			if(creatures::get(mid, i))
				r++;
		}
	}
	return r;
}

static int sbns(int mid, int id)
{
	return (creatures::istagged(mid, id) ? 20 : 0) - creatures::get(mid, Gifted)*10;
}

static int tagcount(int mid)
{
	int r = 0;
	object_creature& e = objects[mid-FirstCreature];
	for(auto i : e.tagged)
	{
		if(i)
			r++;
	}
	return r;
}

int creatures::getr(int mid, int id)
{
	object_creature& e = objects[mid-FirstCreature];
	if(id<sizeof(e.attr)/sizeof(e.attr[0]))
		return e.attr[id];
	return 0;
}

int creatures::get(int id)
{
	switch(id)
	{
	case LastValid:
		return last_valid;
	}
	return 0;
}

static res::token armour_appearance(tags type, res::token def, bool woman)
{
	switch(type)
	{
	case Robe:
		return res::HAROBE;
	case ArmourAdvance:
		return res::HANPWR;
	case ArmourPower:
		return res::HAPOWR;
	case ArmourMetal:
		return woman ? res::HFMETL : res::HMBMET;
	case ArmourLeather:
		return woman ? res::HFLTHR : res::HMLTHR;
	case ArmourCombat:
		return woman ? res::HFCMBT : res::HMCMBT;
	case ArmourLeatherJacket:
		return woman ? res::HFMAXX : res::HMMAXX;
	default:
		return def;
	}
}

int modify_by_weapon(int action, int weapon)
{
	static int actions_base[] = {49,50,-1,-1,-1,53,
		-1, -1,
		54, 55, 61, -1, -1, -1,
		-1, -1
	};
	int t = items::get(weapon, Weapon);
	if(!t || actions_base[action]==-1)
		return action;
	return actions_base[action] + (t-FirstWeaponType)*13;
}

int creatures::get(int mid, int id)
{
	int r;
	tags itype;
	object_creature* e;
	switch(id)
	{
	case Strenght: return objects[mid-FirstCreature].attr[id] + get(mid, Bruiser)*2 + get(mid, Gifted);
	case Perception: return objects[mid-FirstCreature].attr[id] + get(mid, Gifted);
	case Endurance: return objects[mid-FirstCreature].attr[id] + get(mid, Gifted);
	case Charisma: return objects[mid-FirstCreature].attr[id] + get(mid, Gifted);
	case Intellegence: return objects[mid-FirstCreature].attr[id] + get(mid, Gifted);
	case Agility: return objects[mid-FirstCreature].attr[id] + get(mid, SmallFrame) + get(mid, Gifted);
	case Luck: return objects[mid-FirstCreature].attr[id] + get(mid, Gifted);
		// Skills
	case SkillSmallGuns:
		return 5+get(mid, Agility)*4 + sbns(mid, id) - get(mid, GoodNatured)*10;
	case SkillBigGuns:
	case SkillEnergyWeapon:
		return get(mid, Agility)*2 + sbns(mid, id) - get(mid, GoodNatured)*10;
	case SkillUnarmed:
		return 30 + 2*(get(mid, Agility)+get(mid, Strenght)) + sbns(mid, id) - get(mid, GoodNatured)*10;
	case SkillMeleeWeapon:
		return 20 + 2*(get(mid, Agility)+get(mid, Strenght)) + sbns(mid, id) - get(mid, GoodNatured)*10;
	case SkillThrowing:
		return 4*get(mid, Agility) + sbns(mid, id) - get(mid, GoodNatured)*10;
	case SkillFirstAid:
		return 2*(get(mid, Perception)+get(mid, Intellegence)) + sbns(mid, id) + get(mid, GoodNatured)*15;
	case SkillDoctor:
		return 5 + get(mid, Perception) + get(mid, Intellegence) + sbns(mid, id) + get(mid, GoodNatured)*15;
	case SkillSneak:
		return 5 + 3*get(mid, Agility) + sbns(mid, id);
	case SkillLockpick:
		return 10 + get(mid, Perception) + get(mid, Agility) + sbns(mid, id);
	case SkillSteal:
		return 3*get(mid, Agility) + sbns(mid, id);
	case SkillTraps:
		return 10 + get(mid, Perception) + get(mid, Agility) + sbns(mid, id);
	case SkillScience:
		return 4*get(mid, Intellegence) + sbns(mid, id);
	case SkillRepair:
		return 3*get(mid, Intellegence) + sbns(mid, id);
	case SkillSpeech:
		return 5*get(mid, Charisma) + sbns(mid, id) + get(mid, GoodNatured)*15;
	case SkillBarter:
		return 4*get(mid, Charisma) + sbns(mid, id) + get(mid, GoodNatured)*15;
	case SkillGambling:
		return 5*get(mid, Luck) + sbns(mid, id);
	case SkillOutdoorsman:
		return 2*(get(mid, Endurance)+get(mid, Intellegence)) + sbns(mid, id);
	case SkillPoints:
		return 5 + 2*objects[mid-FirstCreature].attr[Intellegence];
	case MaxHP:
		return 15
			+ 2*get(mid, Endurance)
			+ get(mid, Strenght)
			+ (objects[mid-FirstCreature].attr[Level])*(2+get(mid, Endurance)/2);
	case MaxAP:
		return 5 + get(mid, Agility)/2 - (get(mid, Bruiser) ? 2 : 0);
	case AC:
		return get(mid, Agility) + items::get(objects[mid-FirstCreature].gears.body, id);
	case CarryWeight:
		return 25 + (get(mid, SmallFrame) ? 15 : 25)*get(mid, Strenght);
	case MeleeDamage: return imax(1, get(mid, Strenght)-5) + get(mid, HeavyHanded)*4;
	case Stats: return gcount(mid, FirstStat, LastStat);
	case Traits: return gcountd(mid, FirstTraits, LastTraits);
	case Tags: return tagcount(mid);
	case PoisonResistance:
		return get(mid, FastMetabolism) ? 0 : (5 * get(mid, Endurance));
	case RadiationResistance:
		return get(mid, FastMetabolism) ? 0 : (2 * get(mid, Endurance));
	case Initiative:
		return 2 * get(mid, Perception);
	case HealthLevel:
		return imax(1, get(mid, FastMetabolism)*2 + get(mid, Endurance) / 3);
	case CriticalHitChance:
		return get(mid, Luck);
	case Age:
		return world::get(Year) - objects[mid-FirstCreature].attr[id];
	case Gender:
		if(objects[mid-FirstCreature].attr[Race]==Female)
			return Female;
		return Male;
	case Race: return get(mid, Gender);
	case Weapon: return objects[mid-FirstCreature].gears.hands[0];
	case Frame:
		e = &objects[mid-FirstCreature];
		itype = items::type(e->gears.body);
		switch(get(mid, Race))
		{
		case Male:
			return armour_appearance(itype, res::HMJMPS, false);
		case Female:
			return armour_appearance(itype, res::HFJMPS, true);
		default: return res::NABRWN;
		}
	case PositionX: return objects[mid-FirstCreature].position.x;
	case PositionY: return objects[mid-FirstCreature].position.y;
	case Animation: return objects[mid-FirstCreature].frame;
	case Action: return modify_by_weapon(objects[mid-FirstCreature].action, get(mid,Weapon));
	case Direction: return objects[mid-FirstCreature].direction;
	case DamageNormal:
	case DamageFire:
	case DamageLaser:
	case DamageExplosive:
	case DamageElectrical:
	case DamageEMP:
	case DamagePlasma:
	case DamageResistanceNormal:
	case DamageResistanceLaser:
	case DamageResistanceFire:
	case DamageResistancePlasma:
	case DamageResistanceElectrical:
	case DamageResistanceEMP:
	case DamageResistanceExplosive:
		return items::get(objects[mid-FirstCreature].gears.body, id);
	case EquipmentWeight:
		r = items::get(objects[mid-FirstCreature].gears.hands[0], CarryWeight)
			+ items::get(objects[mid-FirstCreature].gears.hands[1], CarryWeight)
			+ items::get(objects[mid-FirstCreature].gears.body, CarryWeight);
		for(auto& m : inventories)
		{
			if(!m.mid)
				break;
			if(m.mid==mid)
				r += items::get(m.item, CarryWeight);
		}
		return r;
	case Position:
		e = &objects[mid-FirstCreature];
		return map::xy2h(e->position.x, e->position.y);
	case Order:
		return objects[mid-FirstCreature].index;
	default:
		if(id>=FirstTraits && id<=LastTraits)
			return (objects[mid-FirstCreature].traits&(1<<(id-FirstTraits))) ? 1 : 0;
		else if(id<sizeof(objects[0].attr)/sizeof(objects[0].attr[0]))
			return objects[mid-FirstCreature].attr[id];
		return 0;
	}
}

const char* creatures::gets(int mid, int id)
{
	switch(id)
	{
	case Name:
		return objects[mid-FirstCreature].name;
	default:
		return "creature";
	}
}

void creatures::clear()
{
	// clear all and set default to man
	rmset(objects, 0, sizeof(objects));
	for(int i = FirstCreature; i<=LastCreature; i++)
	{
		setr(i, Strenght, 5);
		setr(i, Perception, 5);
		setr(i, Endurance, 5);
		setr(i, Charisma, 5);
		setr(i, Intellegence, 5);
		setr(i, Agility, 5);
		setr(i, Luck, 5);
		set(i, Age, 25);
	}
	rmset(inventories, 0, sizeof(inventories));
}

void creatures::setr(int mid, int id, int value)
{
	object_creature& e = objects[mid-FirstCreature];
	if(id<(sizeof(e.attr)/sizeof(e.attr[0])))
		e.attr[id] = value;
}

bool creatures::set(int mid, int id, int value)
{
	object_creature& e = objects[mid-FirstCreature];
	switch(id)
	{
	case Age:
		e.attr[id] = world::get(Year) - value;
		break;
	case Action:
		if(e.action!=value)
		{
			set(mid, Animation, 0);
			e.action = value;
			e.index = -1;
		}
		break;
	case Animation:
		e.frame = value;
		e.stop = 0;
		break;
	case Position:
		e.position.x = map::h2x(value);
		e.position.y = map::h2y(value);
		break;
	case Direction:
		e.direction = value%6;
		break;
	case Order:
		e.index = value;
		break;
	default:
		if(id>=FirstTraits && id<=LastTraits)
		{
			if(value)
				e.traits |= 1<<(id-FirstTraits);
			else
				e.traits &= ~(1<<(id-FirstTraits));
		}
		else if(id<sizeof(e.attr)/sizeof(e.attr[0]))
		{
			e.attr[id] = value;
			return true;
		}
		return false;
	}
	return true;
}

bool creatures::set(int mid, int id, const char* value)
{
	object_creature& e = objects[mid-FirstCreature];
	switch(id)
	{
	case Name:
		e.name[0] = 0;
		if(value)
			szcpy(e.name, value, sizeof(e.name)/sizeof(e.name[0]));
		break;
	default:
		return false;
	}
	return true;
}

bool creatures::isallow(int mid, int id)
{
	return false;
}

bool creatures::add(int mid, unsigned item, int id, int pos, bool run)
{
	object_creature& e = objects[mid-FirstCreature];
	if(id==-1)
		id = items::get(item, Type);
	if(id!=Armour && id!=Weapon)
		id = Inventory;
	if(id==Weapon && pos==-1)
	{
		if(e.gears.hands[0])
			pos = 0;
		else
			pos = 1;
	}
	switch(id)
	{
	case Armour:
		if(items::get(item, Type)!=Armour)
			return false;
		if(e.gears.body)
			add(mid, e.gears.body, Inventory);
		e.gears.body = item;
		break;
	case Weapon:
		if(e.gears.hands[pos])
			add(mid, e.gears.hands[pos], Inventory);
		e.gears.hands[pos] = item;
		break;
	default:
		// 1 - add to exist
		for(auto& m : inventories)
		{
			if(!m.mid)
				break;
			if(m.mid!=mid)
				continue;
			if(!m.item)
			{
				m.item = item;
				return true;
			}
			items::add(m.item, item);
			if(!item)
				return true;
		}
		// 2 - create new
		for(auto& m : inventories)
		{
			if(!m.mid)
			{
				m.mid = mid;
				m.item = item;
				return true;
			}
		}
		return false;
	}
	return true;
}

int creatures::invertory(unsigned** result, int count, int mid)
{
	int r = 0;
	for(auto& m : inventories)
	{
		if(!m.mid)
			break;
		if(m.mid==mid && m.item)
		{
			if(result)
				result[r] = &m.item;
			if(++r>=count)
				break;
		}
	}
	return r;
}

unsigned* creatures::gears(int mid, int id, int pos)
{
	object_creature& e = objects[mid-FirstCreature];
	switch(id)
	{
	case Armour:
		return &e.gears.body;
	case Weapon:
		return &e.gears.hands[pos];
	default:
		return 0;
	}
}

void creatures::damage(int mid, int weapon)
{
	items::damage.min = 1;
	items::damage.max = 1;
	unsigned* pi = gears(mid, Weapon, weapon);
	if(pi)
		items::get(*pi, Damage);
	if(!pi || *pi==0 || items::get(*pi, MeleeDamage))
		items::damage.max += creatures::get(mid, MeleeDamage);
}

void creatures::animate()
{
	int ml = get(LastValid);
	unsigned tm = (unsigned)timeticks();
	for(int mid = FirstCreature; mid<=ml; mid++)
	{
		object_creature& a = objects[mid - FirstCreature];
		if(a.stop>0)
		{
			a.stop--;
			continue;
		}
		res::token rs = (res::token)creatures::get(mid, Frame);
		int ac = creatures::get(mid, Action);
		int c1 = res::gframes(rs, ac) / 6;
		int p2, p3, x3, y3;
		ui::frame* f;
		switch(a.action)
		{
		case ActionWalk:
			if((hot::frame%2)==0)
				break;
		case ActionRun:
			f = ui::gframe(rs, ac, a.direction*c1 + a.frame);
			a.position.x += f->mx;
			a.position.y += f->my;
			p3 = creatures::get(mid, Position);
			x3 = map::h2x(p3);
			y3 = map::h2y(p3);
			p2 = creatures::get(mid, Order);
			if(iabs(x3-a.position.x)<=6 && iabs(y3-a.position.y)<=6)
			{
				if(p2==p3)
				{
					creatures::set(mid, Action, ActionStand);
					creatures::set(mid, Position, p3);
					break;
				}
				else
					a.direction = map::d2o(map::direction(p3, p2));
			}
			if(++a.frame>=c1)
				a.frame = 0;
			break;
		case ActionStand:
			if((hot::frame%2)==0)
				break;
			if(++a.frame>=c1)
			{
				a.frame = 0;
				a.stop = (1+(xrand()%8))*12;
			}
			break;
		}
	}
}