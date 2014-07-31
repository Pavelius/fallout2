#include "f2lib.h"
#include "formats.h"

static unsigned char code[256] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
	32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
	48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
	64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
	80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
	96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
	112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
	192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207,
	208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
	224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255,
	168, 184, 170, 186, 175, 191, 161, 162, 176, 0, 183, 0, 185, 164, 0, 160,
};

const char* szdecode(char* p)
{
	unsigned char* p1 = (unsigned char*)p;
	while(*p1)
	{
		unsigned char sym = code[*p1];
		if(!sym)
			return p;
		*p1++ = sym;
	}
	return p;
}

const char* szskip(const char* p, char sym)
{
	while(*p && *p!=sym)
		p++;
	if(*p==sym)
		p++;
	return p;
}

void cnvurl(const char* dst, const char* url, const char* ext, void (*cnv)(const char*, const char*))
{
	char tsrc[260];
	char tdst[260];
	for(const char* p = io::file::find(url); p; p = io::file::find(url))
	{
		if(szcmp(p, "..") == 0)
			continue;
		if(szcmp(p, ".") == 0)
			continue;
		// source
		szfpath(tsrc, url);
		szcat(tsrc, p);
		// destination
		szcpy(tdst, dst);
		szcat(tdst, "\\");
		szcat(tdst, p);
		((char*)szext(tdst))[-1] = 0;
		szcat(tdst, ".");
		szcat(tdst, ext);
		cnv(tdst, tsrc);
	}
	io::file::find(0);
}