#include "f2lib.h"

res::token				ui::font;
static unsigned char	code[256] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
	32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
	48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
	64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
	80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
	96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
	112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	255, 246, 247, 0, 253, 0, 0, 0, 240, 0, 242, 0, 0, 0, 0, 244,
	248, 0, 0, 0, 0, 0, 0, 250, 241, 252, 243, 0, 0, 0, 0, 245,
	128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
	144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
	160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
	224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
};

int ui::text(int x, int y, const char* string, int count)
{
	if(count==-1)
		count = szlen(string);
	int x1 = x;
	while(count--)
	{
		unsigned char sym = code[*((unsigned char*)string++)];
		image(x1, y, font, sym, FIUseColors);
		x1 += res::gsx(font, sym) + res::gvar(font, 1);
	}
	return x1 -x;
}

int ui::textw(const char* string, int count)
{
	if(count==-1)
		count = szlen(string);
	int x1 = 0;
	while(count--)
	{
		unsigned char sym = code[*((unsigned char*)string++)];
		x1 += res::gsx(font, sym) + res::gvar(font, 1);
	}
	return x1;
}

int ui::textbc(const char* string, int count, int width)
{
	int p = -1;
	if(count==-1)
	{
		const char* p1 = string;
		while(true)
		{
			unsigned char sym = code[*((unsigned char*)string++)];
			if(!sym)
				return string-p1-1;
			if(sym==0x20 || sym==9 || sym=='-')
				p = string-p1;
			else if(sym==10 || sym==13)
				return string-p1;
			width -= res::gsx(font, sym) + res::gvar(font, 1);
			if(width < 0)
			{
				if(p==-1)
					return string-p1;
				return p;
			}
		}
		return string-p1;
	}
	else
	{
		for(int i = 0; i < count; i++)
		{
			unsigned char sym = code[*((unsigned char*)string++)];
			if(sym==0x20 || sym==9 || sym=='-')
				p = i;
			else if(sym==10 || sym==13)
				return i;
			width -= res::gsx(font, sym) + res::gvar(font, 1);
			if(width < 0)
			{
				if(p==-1)
					return i;
				return p;
			}
		}
		return count;
	}
}

int ui::texth()
{
	return res::gvar(font, 0) + res::gvar(font, 2);
}

int ui::texth(const char* string, int width)
{
	int r = 0;
	while(*string)
	{
		r += texth();
		int m = textbc(string, -1, width);
		if(!m)
			return r;
		string = szskipsp(szskipcr(string + m));
	}
	return r;
}

int ui::textm(int x, int y, int width, const char* string)
{
	int y1 = y;
	while(*string)
	{
		int m = textbc(string, -1, width);
		if(!m)
			return y - y1;
		int w = textw(string, m);
		text(x + (width-w)/2, y, string, m);
		string = szskipsp(szskipcr(string + m));
		y += texth();
	}
	return y - y1;
}

int ui::text(int x, int y, int width, const char* string)
{
	int y1 = y;
	while(*string)
	{
		int m = textbc(string, -1, width);
		if(!m)
			return y - y1;
		text(x, y, string, m);
		string = szskipsp(szskipcr(string + m));
		y += texth();
	}
	return y - y1;
}