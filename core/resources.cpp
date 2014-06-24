#include "f2lib.h"

namespace res
{
	struct element
	{
		const char*		name;
		const char*		ext;
		void*			ptr;
	};
}

static const char* path_to_data = "resources\\";

static res::element elements[res::LastResource - res::FirstResource + 1] =
{
	{"BACKGRND"},
	{"SKILLDEX"},
	{"INVEN"},
	{"ITEMS"},
	{"MISC"},
	{"SCENERY"},
	{"WALLS"},
	{"TILES"},
	{"INTRFACE"},
	{"FONT1"}, {"FONT2"}, {"FONT3"}, {"FONT4"},
	// humans
	{"HANPWR"}, {"HAPOWR"}, {"HAROBE"},
	{"HFCMBT"}, {"HFJMPS"}, {"HFLTHR"}, {"HFMAXX"}, {"HFMETL"}, {"HFPRIM"},
	{"HMBJMP"}, {"HMBMET"},
	{"HMCMBT"}, {"HMJMPS"}, {"HMLTHR"}, {"HMMAXX"}, {"HMMETL"}, {"HMWARR"},
	{"MABOS2"}, {"MABOSS"}, {"MAMTNT"}, {"MAMTN2"},
	{"MAROBE"}, {"NABLUE"}, {"NABRWN"},
	{"NACHLD"},
	{"NAGHUL"}, {"NAGLOW"}, {"NAPOWR"}, {"NAROBE"}, {"NAVGUL"}, {"NAWHIT"},
	{"NFASIA"},
	{"NFBRLP"}, {"NFLYNN"},
	{"NFMAXX"}, {"NFMETL"},
	{"NFNICE"}, {"NFPEAS"}, {"NFPRIM"}, {"NFTRMP"}, {"NFVALT"}, {"NFVRED"},
	{"NMASIA"},
	{"NMBONC"}, {"NMBOXX"}, {"NMBPEA"}, {"NMBRLP"}, {"NMBRSR"}, {"NMBSNP"},
	{"NMCOPP"}, {"NMDOCC"}, {"NMFATT"}, {"NMGANG"}, {"NMGRCH"}, {"NMLABB"},
	{"NMLOSR"}, {"NMLTHR"}, {"NMMAXX"}, {"NMMEXI"}, {"NMMYRN"}, {"NMNICE"}, {"NMOLDD"},
	{"NMPEAS"}, {"NMRGNG"}, {"NMVALT"}, {"NMWARR"},
	// animals and robots
	{"MAANTT"}, {"MABRAN"}, {"MABROM"}, {"MACLAW"}, {"MACLW2"}, {"MADDOG"}, {"MADEGG"},
	{"MADETH"}, {"MAFIRE"}, {"MAGCKO"}, {"MAGKO2"}, {"MALIEN"}, {"MAMANT"}, {"MAMRAT"},
	{"MAMURT"}, {"MAPLNT"}, {"MAQUEN"}, {"MASCP2"}, {"MASCRP"}, {"MASPHN"}, {"MASRAT"},
	{"MATHNG"}, {"MACYBR"}, {"MAFEYE"}, {"MAGUN2"}, {"MAGUNN"}, {"MAHAND"}, {"MAROBO"},
	{"MAROBT"},
	// support
	{"RESERV"},
	{"COLOR", "PAL"},
};

void* res::get(res::token icn)
{
	if(!elements[icn].ptr)
	{
		char temp[260];
		elements[icn].ptr = io::file::load(szmkpath(temp, path_to_data,
			elements[icn].name,
			elements[icn].ext ? elements[icn].ext : "pma"));
	}
	return elements[icn].ptr;
}

const char* res::gname(res::token icn)
{
	return elements[icn].name;
}

void res::header::set(const char* text)
{
	type[0] = text[0];
	type[1] = text[1];
	type[2] = text[2];
	type[3] = 0;
}

int res::gframes(res::token icn)
{
	ui::sprite* p = (ui::sprite*)get(icn);
	if(!p)
		return 0;
	return p->count;
}

int res::gsx(res::token icn, int animation, int frame)
{
	ui::sprite* p = (ui::sprite*)get(icn);
	if(!p)
		return 0;
	ui::cicle& c = p->cicles[animation%p->count];
	if(!c.count)
		return 0;
	ui::frame* f = c.ptr(p) + (frame%c.count);
	return f->sx;
}

int res::gsy(res::token icn, int animation, int frame)
{
	ui::sprite* p = (ui::sprite*)get(icn);
	if(!p)
		return 0;
	ui::cicle& c = p->cicles[animation%p->count];
	if(!c.count)
		return 0;
	ui::frame* f = c.ptr(p) + (frame%c.count);
	return f->sy;
}

formats res::codetype(res::token icn, int animation, int frame)
{
	ui::sprite* p = (ui::sprite*)get(icn);
	if(!p)
		return AUTO;
	ui::cicle& c = p->cicles[animation%p->count];
	if(!c.count)
		return AUTO;
	ui::frame* f = c.ptr(p);
	return f->encode;
}