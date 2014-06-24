#include "f2lib.h"
#include "ai.h"
#include "formats.h"

#pragma pack(push)
#pragma pack(1)
enum flag_all
{
	Flat = 0x00000008,
	NoBlock = 0x00000010, // doesn't block the tile
	MultiHex = 0x00000800,
	NoHighlight = 0x00001000, // doesn't highlight the border; used for containers
	TransRed = 0x00004000,
	TransNone = 0x00008000, // opaque
	TransWall = 0x00010000,
	TransGlass = 0x00020000,
	TransSteam = 0x00040000,
	TransEnergy = 0x00080000,
	LightThru = 0x20000000,
	ShootThru = 0x80000000
};
enum flags_ex_item
{
	HiddenItem = 0x080000,
	Use = 0x000008, // can be used
	UseOnSmth = 0x000010, // can be used on anything
	PickUp = 0x000080,
	BigGun = 0x000001,
	TwoHnd = 0x000002, // weapon is two-handed
};
struct pro_header
{
	unsigned		id;
	unsigned		text;
	unsigned		frame;
	unsigned		light_radius;
	unsigned		light_intensity;
	unsigned		flags; // use value from flag_s
};
struct pro_tile : public pro_header
{
	unsigned		material;
};
struct pro_item : public pro_header
{
	unsigned		flags_ex;
	unsigned		script;
	unsigned		subtype;
	unsigned		material;
	unsigned		size;
	unsigned		weight;
	unsigned		cost;
	unsigned		inv_frm;
	unsigned char	sound;
};
struct pro_armour : public pro_item
{
	unsigned		ac;
	unsigned		resistance[7];
	unsigned		threshold[7];
	unsigned		perk;
	unsigned		male_fid;
	unsigned		female_fid;
};
struct pro_weapon : public pro_item
{
	unsigned		id_anamation;
	unsigned		damage[2];
	unsigned		damage_type;
	unsigned		range[2];
	unsigned		fid_projectile;
	unsigned		strenght;
	unsigned		ap[2];
	unsigned		critical_fail;
	unsigned		perk;
	unsigned		burst;
	unsigned		caliber;
	unsigned		fid_ammo;
	unsigned		maximum_ammo;
	unsigned char	sound;
};
#pragma pack(pop)

static const char* attack_type[16] =
{
	"AttackNo", "AttackPunch", "AttackKick", "AttackSwing", "AttackThrust", "AttackThrow",
	"AttackFireSingle", "AttackFireBurst", "AttackFlame"
};
static const char* damage_type[16] =
{
	"DamageNormal", "DamageLaser", "DamageFire", "DamagePlasma",
	"DamageElectrical", "DamageEMP", "DamageExplosive"
};
static const char* weapon_type[LastWeaponType-FirstWeaponType+2] =
{
	"Empthy",
	"WeaponTypeDagger", "WeaponTypeBatton", "WeaponTypeHammer", "WeaponTypeSpear",
	"WeaponTypePistol", "WeaponTypeAutomatic", "WeaponTypeRifle",
	"WeaponTypeBigGun", "WeaponTypeMachineGun", "WeaponTypeRocket"
};
static const char* item_type[] = {"Armour", "Container", "Drug", "Weapon", "Ammo", "Misc", "Key"};
static const char* mat_type[] = {"Glass", "Metal", "Plastic", "Wood", "Dirt", "Stone", "Cement", "Leather"};

static void skip_armour(io::file& file)
{
	file << "0, {0}, {0}, "; // ac, {threshold}, {resistance}
}

static void skip_weapon(io::file& file)
{
	file << "Empthy, Empthy, {}, {0}, 0, 0, {0}, 0, 0, 0, 0, 0, ";
	//tags			perk;
	//tags			weapon_type;
	//damagei			damage;
	//unsigned		range[2];
	//unsigned		fid_projectile;
	//unsigned char	strenght;
	//unsigned char	ap[2];
	//unsigned		critical_fail;
	//unsigned		burst;
	//unsigned		caliber;
	//unsigned		fid_ammo;
	//unsigned		maximum_ammo;
}

static void add_armour(io::file& file, pro_item* h)
{
	pro_armour* p = static_cast<pro_armour*>(h);
	swap(p->ac);
	file << p->ac << ", "; // ac
	file << "{"; // threshold
	for(int i = 0; i<sizeof(p->threshold)/sizeof(p->threshold[0]); i++)
	{
		swap(p->threshold[i]);
		if(p->threshold[i]>255)
			p->threshold[i] = 255;
		file << p->threshold[i];
		if(i!=sizeof(p->threshold)/sizeof(p->threshold[0])-1)
			file << ", ";
	}
	file << "}, ";
	file << "{"; // resistance
	for(int i = 0; i<sizeof(p->resistance)/sizeof(p->resistance[0]); i++)
	{
		swap(p->resistance[i]);
		if(p->resistance[i]>255)
			p->resistance[i] = 255;
		file << p->resistance[i];
		if(i!=sizeof(p->resistance)/sizeof(p->resistance[0])-1)
			file << ", ";
	}
	file << "}, ";
}

static void add_weapon(io::file& file, pro_item* h)
{
	pro_weapon* p = static_cast<pro_weapon*>(h);
	swap(p->perk);
	swap(p->id_anamation);
	swap(p->damage[0]); swap(p->damage[1]); swap(p->damage_type);
	swap(p->range[0]); swap(p->range[1]);
	swap(p->fid_projectile);
	swap(p->strenght);
	swap(p->ap[0]); swap(p->ap[1]);
	swap(p->critical_fail);
	swap(p->burst);
	swap(p->caliber);
	swap(p->fid_ammo);
	swap(p->maximum_ammo);
	file << "Empthy" << ", "; //perk;
	file << weapon_type[p->id_anamation] << ", "; //weapon_type;
	file << "{" << damage_type[p->damage_type] << ", " << p->damage[0] << ", " << p->damage[1] << "}, "; //damage;
	file << "{" << p->range[0] << ", " << p->range[1] << "}, "; //range[2];
	file << p->fid_projectile << ", "; //fid_projectile;
	file << p->strenght << ", "; //fid_projectile;
	file << "{" << p->ap[0] << ", " << p->ap[1] << "}, "; //ap;
	file << p->critical_fail << ", "; //critical_fail;
	file << p->burst << ", "; //burst;
	file << p->caliber << ", ";
	file << p->fid_ammo << ", ";
	file << p->maximum_ammo << ", ";
}

void cnvitem(pro_item* p, int& att1, int& att2)
{
	att1 = (p->flags_ex>>24)&0x0F;
	att2 = (p->flags_ex>>28)&0x0F;
}

char* szstr(char* text, const char* substring)
{
	char ch = substring[0];
	char* p = text;
	int n = szlen(substring);
	if(n==0)
		return 0;
	else if(n==1)
		return (char*)szchr(p, ch);
	while((p = (char*)szchr(p, ch))!=0)
	{
		if(szcmp(substring, p, n)==0)
			return p;
		p = p + 1;
	}
	return 0;
}

static const char* szdesc(const char* text)
{
	static const char* tmp[] = {"Ìèí ÑË : ", "Ìèí ÑË:"};
	static char temp[4096];
	szcpy(temp, text);
	for(auto e : tmp)
	{
		char* p = szstr(temp, e);
		if(p)
		{
			*p = 0;
			while(p>temp && p[-1]==' ')
			{
				p[-1] = 0;
				p--;
			}
			break;
		}
	}
	return temp;
}

void cnvitm(const char* url, const char* dst)
{
	int att1, att2;
	char temp[260];
	io::file file(dst, io::file::Write);
	file << "object_item items[] = {\n";
	for(const char* p = io::file::find(url); p; p = io::file::find(url))
	{
		if(szcmp(p, "..") == 0)
			continue;
		if(szcmp(p, ".") == 0)
			continue;
		szfpath(temp, url);
		szcat(temp, "\\");
		szcat(temp, p);
		int size;
		pro_item* h = (pro_item*)io::file::load(temp, &size);
		swap(h->id);
		swap(h->text);
		swap(h->frame);
		swap(h->light_radius);
		swap(h->light_intensity);
		swap(h->flags);
		swap(h->script);
		swap(h->subtype);
		swap(h->material);
		swap(h->size);
		swap(h->weight);
		swap(h->cost);
		swap(h->inv_frm);
		cnvitem(h, att1, att2);
		if(h->subtype!=(Container-Armour))
		{
			file << "\t{";
			file << item_type[h->subtype] << ", "; // type
			file << "{\"" << szcpp(szname((h->text/100)*100, msg_items)) << "\"},"; // name
			file << "{";
			file << (h->frame&0xFFFF) << ", "; // frame
			file << (h->inv_frm&0xFFFF); // frame in invertory
			file << "}, ";
			file << "{";
			file << (h->sound); // sound
			file << "}, ";
			file << attack_type[att1] << ", " << attack_type[att2] << ", "; // attack modes
			file << mat_type[h->material] << ", "; // Material
			file << h->size << ", "; // size or volume
			file << h->weight << ", "; // weight
			file << h->cost << ", "; // cost
			switch(size)
			{
			case sizeof(pro_armour) :
				add_armour(file, h);
				skip_weapon(file);
				break;
			case sizeof(pro_weapon) :
				skip_armour(file);
				add_weapon(file, h);
				break;
			default:
				skip_armour(file);
				skip_weapon(file);
				break;
			}
			file << "{\"" << szdesc(szcpp(szname((h->text/100)*100+1, msg_items))) <<  "\"}"; // description
			file << "},\n";
		}
		delete h;
	}
	file << "};";
	io::file::find(0);
}

void cnvtil(io::file& file, const char* url)
{
	pro_tile e;
	io::file src(url);
	if(!src.isvalid())
		return;
	src.read(&e, sizeof(e));
	swap(e.id);
	swap(e.text);
	swap(e.frame);
	swap(e.light_radius);
	swap(e.light_intensity);
	swap(e.flags);
	swap(e.material);
}

void cnvtil(const char* dst, const char* url)
{
	char temp[260];
	io::file file(dst, io::file::Write);
	file << "object_item tiles[] = {\n";
	for(const char* p = io::file::find(url); p; p = io::file::find(url))
	{
		if(szcmp(p, "..") == 0)
			continue;
		if(szcmp(p, ".") == 0)
			continue;
		szfpath(temp, url);
		szcat(temp, "\\");
		szcat(temp, p);
		cnvtil(file, temp);
	}
	file << "};";
	io::file::find(0);
}