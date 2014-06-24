#include "f2lib.h"

extern char		chupper(char c); // to upper reg
int				locale; // curret locale

int rmcmp(const void* p1, const void* p2, int count)
{
	const unsigned char* e1 = (const unsigned char*)p1;
	const unsigned char* e2 = (const unsigned char*)p2;
	while(count--)
	{
		if(*e1 != *e2)
			return *e1 - *e2;
		e1++;
		e2++;
	}
	return 0;
}

char* szend(char* text)
{
	while(*text)
		text++;
	return text;
}

void szcpy(char* p1, const char* p2)
{
	while(*p2)
		*p1++ = *p2++;
	*p1 = 0;
}

void szcpy(char* p1, const char* p2, int max_count)
{
	while(*p2 && max_count-->0)
		*p1++ = *p2++;
	*p1 = 0;
}

int szcmp(const char* p1, const char* p2)
{
	for(; *p2 && *p1; p1++, p2++)
	{
		if(*p1 == *p2)
			continue;
		return *p1 - *p2;
	}
	return *p1 - *p2;
}

int szcmp(const char* p1, const char* p2, int max_count)
{
	for(; *p2 && *p1 && max_count>0; p1++, p2++, max_count--)
	{
		if(*p1 == *p2)
			continue;
		return *p1 - *p2;
	}
	if(!max_count)
		return 0;
	return *p1 - *p2;
}

int szcmpi(const char* p1, const char* p2)
{
	for(; *p2 && *p1; p1++, p2++)
	{
		if(*p1==*p2)
			continue;
		if(chupper(*p1) == chupper(*p2))
			continue;
		return chupper(*p1) - chupper(*p2);
	}
	return chupper(*p1) - chupper(*p2);
}

int szcmpi(const char* p1, const char* p2, int max_count)
{
	for(; *p2 && *p1 && max_count>0; p1++, p2++, max_count--)
	{
		if(*p1 == *p2)
			continue;
		if(chupper(*p1) == chupper(*p2))
			continue;
		return chupper(*p1) - chupper(*p2);
	}
	if(!max_count)
		return 0;
	return chupper(*p1) - chupper(*p2);
}

char* sznum(char* p1, int num, int precision, const char* empthy, int radix)
{
	if(num == 0)
	{
		if(empthy)
			szcpy(p1, empthy);
		else
		{
			szcpy(p1, "0");
			while(--precision>0)
				szcat(p1, "0");
		}
	}
	else
	{
		char temp[32];
		int p = 0;
		if(num < 0)
		{
			*p1++ = '-';
			num = -num;
		}
		switch(radix)
		{
		case 16:
			while(num)
			{
				int a = (num%radix);
				if(a>9)
					temp[p++] = 'A' - 10 + a;
				else
					temp[p++] = '0' + a;
				num /= radix;
			}
			break;
		default:
			while(num)
			{
				temp[p++] = '0' + (num%radix);
				num /= radix;
			}
			break;
		}
		while(precision-- > p)
			*p1++ = '0';
		while(p)
			*p1++ = temp[--p];
		p1[0] = 0;
	}
	return p1;
}

int sz2num(const char* p1, const char** pp1)
{
	int result = 0;
	bool sign = false;
	const int radix = 10;
	while(*p1 && *p1!='-' && (*p1 < '0' || *p1 > '9'))
		p1++;
	while(*p1)
	{
		char a = *p1;
		if(a < '0' || a > '9')
			break;
		if(a == '-')
		{
			sign = true;
			p1++;
			continue;
		}
		result = result*radix;
		result += a - '0';
		p1++;
	}
	if(sign)
		result = -result;
	if(pp1)
		*pp1 = p1;
	return result;
}

const char* szchr(const char* p1, char ch)
{
	for(; *p1; p1++)
	{
		if(*p1==ch)
			return p1;
	}
	return 0;
}

const char* szext(const char* path)
{
	for(const char* r = szend((char*)path); r > path; r--)
	{
		if(*r == '.')
			return r + 1;
		else if(*r == '\\')
			return 0;
	}
	return 0;
}

const char* szfname(const char* path)
{
	for(const char* r = szend((char*)path); r > path; r--)
	{
		if(*r == '\\')
			return r + 1;
	}
	return path;
}

const char* szfpath(char* path, const char* fullpath)
{
	szcpy(path, fullpath, szfname(fullpath) - fullpath);
	return path;
}

const char* szmkpath(char* result, const char* fullpath, const char* filename, const char* ext)
{
	szcpy(result, fullpath);
	if(filename)
		szcat(result, filename);
	if(ext)
	{
		szcat(result, ".");
		szcat(result, ext);
	}
	return result;
}

char* szprint(char* result, const char* src, ...)
{
	char** a = (char**)((char*)&src + sizeof(src));
	char* dst = result;
	while(true)
	{
		switch(*src)
		{
		case 0:
			*dst = 0;
			return result;
		case '%':
			src++;
			if(*src == '%')
			{
				*dst++ = *src++;
				continue;
			}
			if(*src >= '0' && *src <= '9')
			{
				int pn = 0;
				while(*src >= '0' && *src <= '9')
					pn = pn * 10 + (*src++) - '0';
				char* pt = 0;
				if(*src == 'i')
				{
					src++;
					static char temp[16];
					sznum(temp, *((int*)((char*)a + sizeof(char*)*(pn - 1))), 0, 0, 10);
					pt = temp;
				}
				else
					pt = a[pn-1];
				if(pt)
				{
					while(*pt)
						*dst++ = *pt++;
				}
			}
			break;
		default:
			*dst++ = *src++;
			break;
		}
	}
	return result;
}

const char* szt(const char* en, const char* ru)
{
	if(locale==1)
		return ru;
	return en;
}

void rmcpy(void* p1, const void* p2, int size)
{
	if(size>sizeof(int))
	{
		unsigned* u1 = (unsigned*)p1;
		unsigned* u2 = (unsigned*)p2;
		while(size>sizeof(int))
		{
			*u1++ = *u2++;
			size -= sizeof(int);
		}
	}
	for(int i = 0; i < size; i++)
		((char*)p1)[i] = ((char*)p2)[i];
}

void rmmov(void *dst, const void *src, int count)
{
	if(dst <= src || (char*)dst >= ((char*)src + count))
	{
		while(count--)
		{
			*(char*)dst = *(char*)src;
			dst = (char*)dst + 1;
			src = (char*)src + 1;
		}
	}
	else
	{
		dst = (char*)dst + count - 1;
		src = (char*)src + count - 1;
		while(count--)
		{
			*(char*)dst = *(char*)src;
			dst = (char*)dst - 1;
			src = (char*)src - 1;
		}
	}
}

void rmset(void* p1, int value, int size)
{
	for(int i = 0; i < size; i++)
		((unsigned char*)p1)[i] = ((unsigned char)value);
}

const char* upper(const char* string)
{
	static char temp[256];
	szcpy(temp, string, sizeof(temp));
	temp[0] = chupper(string[0]);
	return temp;
}

const char* szskipcr(const char* p)
{
	if(*p=='\n')
	{
		p++;
		if(*p=='\r')
			p++;
	}
	else if(*p=='\r')
	{
		p++;
		if(*p=='\n')
			p++;
	}
	return p;
}

const char* szline(char* result, const char* p)
{
	result[0] = 0;
	while(*p && *p!='\n' && *p!='\r')
		*result++ = *p++;
	*result = 0;
	return szskipcr(p);
}

const char* szpercent(int percent)
{
	static char temp[16];
	szprint(temp, "%1i%%", percent);
	return temp;
}

const char* sznumber(int value, int percision)
{
	static char temp[16];
	sznum(temp, value, percision, 0, 10);
	return temp;
}

void swap(unsigned short& u)
{
	u = (u<<8)|(u>>8);
}

void swap(unsigned& u)
{
	u = (u<<24)|((u&0xFF0000)>>16)|((u&0xFF0000)>>8)|((u&0xFF00)<<8)|(u>>24);
}

const char* sznameof(const char* text)
{
	static char temp[128];
	int n = szlen(text);
	if(!n)
		return text;
	szcpy(temp, text);
	switch(text[n-1])
	{
	case 'ã':
	case 'í':
	case 'ñ':
		szcat(temp, "à");
		return temp;
	case 'à':
		temp[n-1] = 'û';
		return temp;
	}
	return text;
}