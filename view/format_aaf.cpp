#include "f2lib.h"
#include "ai.h"
#include "formats.h"

using namespace ui;

#pragma optimize("t", on)
#pragma pack(push)
#pragma pack(1)
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

void cnvaaf(const char* dst, const char* src)
{
	sprite* image = (sprite*)new char[1024*1024];
	image->setup(256);
	aaf_header* e = (aaf_header*)io::file::load(src);
	if(e)
	{
		swap(e->height);
		swap(e->gap_hor);
		swap(e->gap_ver);
		swap(e->space);
		image->vars[0] = e->height;
		image->vars[1] = e->gap_hor;
		image->vars[2] = e->gap_ver;
		int offset = 0;
		for(int i = 0; i<256; i++)
		{
			swap(e->frames[i].sx);
			swap(e->frames[i].sy);
			swap(e->frames[i].offset);
			e->frames[i].offset = offset;
			offset += e->frames[i].sx*e->frames[i].sy;
		}
		for(int i = 0; i<256; i++)
		{
			if(e->frames[i].sx && e->frames[i].sy)
			{
				store(image, i,
					((unsigned char*)e) + e->frames[i].offset + sizeof(aaf_header),
					e->frames[i].sx, e->frames[i].sy,
					e->frames[i].sx/2, e->height,
					AUTO, 1, 0, 0,
					0, 0,
					0, 0);
			}
			else
			{
				image->cicles[i].count = 1;
				image->cicles[i].offset = image->size;
				image->cicles[i].active = 0;
				frame* frames = (frame*)image->ptr();
				image->size += image->cicles[i].count*sizeof(frame);
				frames[0].encode = AUTO;
				frames[0].ox = 0;
				frames[0].oy = 0;
				frames[0].sx = e->space;
				frames[0].sy = 0;
				frames[0].offset = 0;
			}
		}
		image->write(dst);
		delete e;
	}
	delete image;
}

void cnvaaf(const char* name)
{
	char t1[260];
	char t2[260];
	cnvaaf(szmkpath(t1, "D:\\resources\\fallout\\pma\\", name, "pma"),
		szmkpath(t2, "D:\\resources\\fallout\\", name, "aaf"));
}

void cnvaaf()
{
	int s1 = sizeof(aaf_frame);
	int s2 = sizeof(aaf_header);
	cnvaaf("FONT1");
	cnvaaf("FONT2");
	cnvaaf("FONT3");
	cnvaaf("FONT4");
}