#include "f2lib.h"
#include "formats.h"

using namespace ui;

#pragma optimize("t", on)
#pragma pack(push)
#pragma pack(1)
struct frm_header
{
	unsigned		version;
	unsigned short	speed;
	unsigned short	action;
	unsigned short	count;
	short			shiftx[6];
	short			shifty[6];
	unsigned		offsets[6];
	unsigned		size;
};
struct frm_frame
{
	unsigned short	width;
	unsigned short	height;
	unsigned		size;
	short			ox;
	short			oy;
};
struct aaf_frame
{
	unsigned short	sx;
	unsigned short	sy;
	unsigned		offset;
};
struct aaf_header
{
	unsigned		version;
	unsigned short	height;
	unsigned short	gap_hor;
	unsigned short	space;
	unsigned short	gap_ver;
	aaf_frame		frames[256];
};
#pragma pack(pop)

static int frm_count(const char* src)
{
	frm_header e; rmset(&e, 0, sizeof(e));
	frm_frame e1; rmset(&e1, 0, sizeof(e1));
	io::file file(src, io::file::Read);
	e.version = file.getLE32();
	if(e.version!=4)
		return 0;
	e.speed = file.getLE16();
	e.action = file.getLE16();
	e.count = file.getLE16();
	return e.count;
}

static void pack(sprite* image, const char* src, int id, int frame_id=-1)
{
	frm_header e; rmset(&e, 0, sizeof(e));
	frm_frame e1; rmset(&e1, 0, sizeof(e1));
	io::file file(src, io::file::Read);
	if(!file.isvalid())
		return;
	e.version = file.getLE32();
	if(e.version!=4)
		return;
	e.speed = file.getLE16();
	e.action = file.getLE16();
	e.count = file.getLE16();
	for(int i = 0; i<6; i++)
		e.shiftx[i] = file.getLE16();
	for(int i = 0; i<6; i++)
		e.shifty[i] = file.getLE16();
	unsigned dcount = 1;
	for(int i = 0; i<6; i++)
	{
		e.offsets[i] = file.getLE32();
		if(e.offsets[i])
			dcount++;
	}
	e.size = file.getLE32();
	unsigned tsize = 0;
	unsigned fid = 0;
	if(frame_id!=-1)
	{
		dcount = 6;
		fid = frame_id*e.count;
	}
	while(tsize<e.size)
	{
		int oox = 0;
		int ooy = 0;
		int sid = 0;
		for(unsigned i = 0; i<e.count; i++)
		{
			e1.width = file.getLE16();
			e1.height = file.getLE16();
			e1.size = file.getLE32();
			e1.ox = file.getLE16();
			e1.oy = file.getLE16();
			tsize += e1.width*e1.height + 12;
			oox += e1.ox;
			ooy += e1.oy;
			unsigned char* p = new unsigned char[e1.width*e1.height];
			file.read(p, e1.width*e1.height);
			store(image, id, p, e1.width, e1.height,
				oox, ooy,
				AUTO,
				e.count*dcount, fid++, e.action,
				e.shiftx[sid], e.shifty[sid],
				e1.ox, e1.oy);
			delete p;
		}
		sid++;
		if(frame_id!=-1)
			break;
	}
}

static int listcount(const char* p)
{
	int result = 0;
	while(*p)
	{
		char filename[260];
		p = szline(filename, p);
		result++;
	}
	return result;
}

void cnvlst(const char* dst, const char* src)
{
	char temp[260];
	const char* p1 = (const char*)io::file::load(src, 0, 1);
	if(!p1)
		return;
	const char* p = p1;
	ui::sprite* image = (ui::sprite*)new char[1024*1024*50];
	szfpath(temp, src);
	image->setup(listcount(p));
	int id = 0;
	while(*p)
	{
		char filename[260];
		char filepath[260];
		p = szline(filename, p);
		char* sz = szchr(filename, ';');
		if(sz)
		{
			*sz = 0;
			while(sz>filename && (*sz==' ' || *sz==0))
			{
				*sz = 0;
				sz--;
			}
		}
		pack(image, szmkpath(filepath, temp, filename), id++);
	}
	image->write(dst);
	delete image;
	delete p1;
}

const char* szsrc(const char* folder, const char* name, const char* ext, const char* suffix=0)
{
	static char temp[260];
	szcpy(temp, "D:\\resources\\fallout\\");
	szcat(temp, folder);
	szcat(temp, "\\");
	szcat(temp, name);
	if(suffix)
		szcat(temp, suffix);
	szcat(temp, ".");
	szcat(temp, ext);
	return temp;
}

void cnvcrt(const char* dst, const char* name)
{
	static char szanim[][3] = {
		"AA", // stand
		"AB", // walk
		"AE", // climb
		"AK", // pickup
		"AL", // use
		"AN", // dodge blow
		"AO", // damaged
		"AP", // damaged from rear
		"AQ", // unarmed attack 1 (humans hand, animals claws)
		"AR", // unarmed attack 2 (humans legs, animals bite)
		"AS", // throw
		"AT", // run
		"AU", // hands up
		"AG", // cloack off/on
		"BA", // knockout (back)
		"BB", // knockout (forward)
		"BC", // knockout (back only HFCMBT)
		"BD", // killed by bloody mess
		"BE", // killed by immolation
		"BF", // killed by hands off
		"BG", // killed by machine guns
		"BH", // killed by impulse weapon
		"BI", // killed by laser
		"BJ", // killed by immolation
		"BK", // killed by impulse to the trash
		"BL", // killed by blow up
		"BM", // killed by melding
		"BN", // killed by firebat (run and fire)
		"BO", // blooning on ground (face up)
		"BP", // blooning on ground (face down)
		"CH", // knockout (face down), but stand up
		"CJ", // knockout (face up), but stand up
		"NA", // 3D navigation
		// bodies
		"RA", "RB", "RC", "RD", "RE", "RF", "RG", "RH", "RI", "RJ", "RK", "RL", "RM", "RN", "RO", "RP",
		// weapons
		"DA", "DB", "DÑ", "DD", "DE", "DF", "DG", "DH", "DI", "DJ", "DK", "DL", "DM", // Dagger
		"EA", "EB", "EÑ", "ED", "EE", "EF", "EG", "EH", "EI", "EJ", "EK", "EL", "EM", // Batton
		"FA", "FB", "FÑ", "FD", "FE", "FF", "FG", "FH", "FI", "FJ", "FK", "FL", "FM", // Hammer
		"GA", "GB", "GÑ", "GD", "GE", "GF", "GG", "GH", "GI", "GJ", "GK", "GL", "GM", // Spear
		"HA", "HB", "HÑ", "HD", "HE", "HF", "HG", "HH", "HI", "HJ", "HK", "HL", "HM", // Pistol
		"IA", "IB", "IÑ", "ID", "IE", "IF", "IG", "IH", "II", "IJ", "IK", "IL", "IM", // AutomaticPistol
		"JA", "JB", "JÑ", "JD", "JE", "JF", "JG", "JH", "JI", "JJ", "JK", "JL", "JM", // Rifle
		"KA", "KB", "KÑ", "KD", "KE", "KF", "KG", "KH", "KI", "KJ", "KK", "KL", "KM", // Heavy gun
		"LA", "LB", "LÑ", "LD", "LE", "LF", "LG", "LH", "LI", "LJ", "LK", "LL", "LM", // Machine gun
		"MA", "MB", "MÑ", "MD", "ME", "MF", "MG", "MH", "MI", "MJ", "MK", "ML", "MM", // Racket launcher
	};
	ui::sprite* image = (ui::sprite*)new char[1024*1024*100];
	image->setup(sizeof(szanim)/sizeof(szanim[0]));
	int id = 0;
	const char* folder = "art\\criters";
	for(auto e : szanim)
	{
		if(io::file::exist(szsrc(folder, name, "fr0", e)))
		{
			pack(image, szsrc(folder, name, "fr0", e), id, 0);
			pack(image, szsrc(folder, name, "fr1", e), id, 1);
			pack(image, szsrc(folder, name, "fr2", e), id, 2);
			pack(image, szsrc(folder, name, "fr3", e), id, 3);
			pack(image, szsrc(folder, name, "fr4", e), id, 4);
			pack(image, szsrc(folder, name, "fr5", e), id, 5);
		}
		else
			pack(image, szsrc(folder, name, "frm", e), id);
		id++;
	}
	image->write(dst);
	delete image;
}

void cnvcrt()
{
	for(int i = res::HANPWR; i<=res::RESERV; i++)
	{
		char temp[260];
		const char* e = res::gname((res::token)i);
		szcpy(temp, "D:\\resources\\fallout\\pma");
		szcat(temp, "\\");
		szcat(temp, e);
		szcat(temp, ".pma");
		cnvcrt(temp, e);
	}
}

void cnvlst(res::token id)
{
	char src[260];
	char dst[260];
	szprint(src, "%1\\%2\\%3.lst", "D:\\resources\\fallout\\art", res::gname(id), res::gname(id));
	szprint(dst, "%1\\%2.pma", "D:\\resources\\fallout\\pma", res::gname(id));
	cnvlst(dst, src);
}

void cnvlst()
{
	for(int i = res::FirstResource; i<=res::INTRFACE; i++)
		cnvlst((res::token)i);
}