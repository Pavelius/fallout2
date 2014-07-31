#include "f2lib.h"
#include "formats.h"

using namespace ui;

static int cp1251[128] = {
	0x402, 0x403, 0x201A, 0x453, 0x201E, 0x2026, 0x2020, 0x2021, 0x20AC, 0x2030, 0x409, 0x2039, 0x40A, 0x40C, 0x40B, 0x40F,
	0x452, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014, 0, 0x2122, 0x459, 0x203A, 0x45A, 0x45C, 0x45B, 0x45F,
	0xA0, 0x40E, 0x45E, 0x408, 0xA4, 0x490, 0xA6, 0xA7, 0x401, 0xA9, 0x404, 0xAB, 0xAC, 0xAD, 0xAE, 0x407,
	0xB0, 0xB1, 0x406, 0x456, 0x491, 0xB5, 0xB6, 0xB7, 0x451, 0x2116, 0x454, 0xBB, 0x458, 0x405, 0x455, 0x457,
	0x410, 0x411, 0x412, 0x413, 0x414, 0x415, 0x416, 0x417, 0x418, 0x419, 0x41A, 0x41B, 0x41C, 0x41D, 0x41E, 0x41F,
	0x420, 0x421, 0x422, 0x423, 0x424, 0x425, 0x426, 0x427, 0x428, 0x429, 0x42A, 0x42B, 0x42C, 0x42D, 0x42E, 0x42F,
	0x430, 0x431, 0x432, 0x433, 0x434, 0x435, 0x436, 0x437, 0x438, 0x439, 0x43A, 0x43B, 0x43C, 0x43D, 0x43E, 0x43F,
	0x440, 0x441, 0x442, 0x443, 0x444, 0x445, 0x446, 0x447, 0x448, 0x449, 0x44A, 0x44B, 0x44C, 0x44D, 0x44E, 0x44F,
};

static int cp866[128] = {
	0x410, 0x411, 0x412, 0x413, 0x414, 0x415, 0x416, 0x417, 0x418, 0x419, 0x41A, 0x41B, 0x41C, 0x41D, 0x41E, 0x41F,
	0x420, 0x421, 0x422, 0x423, 0x424, 0x425, 0x426, 0x427, 0x428, 0x429, 0x42A, 0x42B, 0x42C, 0x42D, 0x42E, 0x42F,
	0x430, 0x431, 0x432, 0x433, 0x434, 0x435, 0x436, 0x437, 0x438, 0x439, 0x43A, 0x43B, 0x43C, 0x43D, 0x43E, 0x43F,
	0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556, 0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510,
	0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,
	0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B, 0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
	0x440, 0x441, 0x442, 0x443, 0x444, 0x445, 0x446, 0x447, 0x448, 0x449, 0x44A, 0x44B, 0x44C, 0x44D, 0x44E, 0x44F,
	0x401, 0x451, 0x404, 0x454, 0x407, 0x457, 0x40E, 0x45E, 0xB0, 0x2219, 0xB7, 0x221A, 0x2116, 0xA4, 0x25A0, 0xA0,
};

static void codecnv(const char* dst, const unsigned char* src)
{
	io::file file(dst, io::file::Write);
	file << "unsigned char code[256*3] = {\n";
	for(int y = 0; y<16; y++)
	{
		int base = y*8*3;
		file << "\t";
		for(int x = 0; x<16; x++)
		{
			file << *src++ << ", ";
			if(x==15)
				file << "\n";
		}
	}
	file << "};";
}

static void codefill(unsigned char* dst)
{
	for(int i = 0; i<128; i++)
		*dst++ = i;
}

static int codefind(int* c1, int uni)
{
	for(int i = 0; i<128; i++)
	{
		if(c1[i]==uni)
			return i+128;
	}
	return 0;
}

static void codefill(unsigned char* dst, int* c1, int* c2)
{
	for(int i = 0; i<128; i++)
		dst[i] = codefind(c2, c1[i]);
}

static void codecnv()
{
	unsigned char temp[256];
	codefill(temp);
	codefill(temp+128, cp1251, cp866);
	codecnv("D:\\resources\\fallout\\cpp\\code.cpp", temp);
	codefill(temp);
	codefill(temp+128, cp866, cp1251);
	codecnv("D:\\resources\\fallout\\cpp\\codeback.cpp", temp);
}

static void palcnv(const char* src, const char* dst)
{
	unsigned char temp[256*3];
	rmset(temp, 0, sizeof(temp));
	io::file fpal(src, io::file::Read);
	fpal.read(temp, sizeof(temp));
	io::file file(dst, io::file::Write);
	file << "#include \"crt.h\"\n";
	file << "#include \"ui.h\"\n";
	file << "\n";
	file << "unsigned char ui::pallette[256*3] = {\n";
	for(int y = 0; y<32; y++)
	{
		int base = y*8*3;
		file << "\t";
		for(int x = 0; x<8; x++)
		{
			if(x==7)
				file << temp[base+x*3+0] << ", " << temp[base+x*3+1] << ", " << temp[base+x*3+2] << ",\n";
			else
				file << temp[base+x*3+0] << ", " << temp[base+x*3+1] << ", " << temp[base+x*3+2] << ", ";
		}
	}
	file << "};";
}

static void palview()
{
	int index = -1;
	for(int i = 0; i<256; i++)
	{
		rect rc = {(i%16)*16 + 10, (i/16)*16 + 10};
		rc.x2 = rc.x1 + 14;
		rc.y2 = rc.y1 + 14;
		rectf(rc.x1, rc.y1, rc.x2, rc.y2, i);
		if(hot::mouse.in(rc))
			index = i;
	}
	if(index!=-1)
	{
		char temp[32];
		sznum(temp, index, 2, 0, 16);
		//text(320, 10, temp, -1, 1);
	}
}

static void set_caption(int id, int frame)
{
	char temp[260];
	szprint(temp, "%1 : %2i/%3i", res::gname((res::token)id), frame, res::gframes((res::token)id));
	ui::caption(temp);
}

const char* gstr(int id)
{
	return "";
}

void cnvlst(res::token token);

int ui::main(const char* cmdline)
{
	locale = 0;
	//cnvitm("D:\\resources\\fallout\\proto\\ITEMS\\*.pro", "d:\\resources\\fallout\\cpp\\items.cpp");
	//cnvlst();
	//cnvaaf();
	//cnvcrt();
	create(0);
	usepal((unsigned char*)res::get(res::COLOR));
	timer(150);
	int id = res::INTRFACE;
	int frame = 359;
	set_caption(id, frame);
	while(true)
	{
		rectf(0, 0, width, height, 0xA0);
		//palview();
		int xc = width/2;
		int yc = height/2;
		if(id>=res::FirstCreature && id<=res::LastCreature)
			image(xc, yc, (res::token)id, frame, FINoCenter, hot::frame);
		else if(res::codetype((res::token)id, frame, 0)==RAW)
			image(xc-res::gsx((res::token)id, frame)/2,
			yc-res::gsy((res::token)id, frame)/2,
			(res::token)id, frame, FINoOffset, hot::frame);
		else
			image(xc, yc, (res::token)id, frame, 0, hot::frame);
		//line(xc-10, yc, xc+10, yc, gcol());
		//line(xc, yc-10, xc, yc+10, gcol());
		char temp[64]; szprint(temp, "%1i, %2i, w=%3i, h=%4i",
			hot::mouse.x, hot::mouse.y,
			res::gsx(res::token(id), frame),
			res::gsy(res::token(id), frame));
		text(0, 464, temp);
		cursors::normal();
		switch(input())
		{
		case 0:
			return 0;
		case KeyUp:
			if(id)
			{
				id--;
				frame = 0;
				set_caption(id, frame);
			}
			break;
		case KeyDown:
			if(id<res::RESERV)
			{
				id++;
				frame = 0;
				set_caption(id, frame);
			}
			break;
		case KeyRight:
			if(frame<res::gframes((res::token)id)-1)
			{
				frame++;
				set_caption(id, frame);
			}
			break;
		case KeyLeft:
			if(frame)
			{
				frame--;
				set_caption(id, frame);
			}
			break;
		}
	}
}