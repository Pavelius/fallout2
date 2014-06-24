#include "f2lib.h"

namespace io
{

	stream&	stream::operator<<(const char* t)
	{
		write(t, szlen(t));
		return *this;
	}

	stream& stream::operator<<(const int n)
	{
		char temp[32];
		sznum(temp, n);
		write(temp, szlen(temp));
		return *this;
	}

	unsigned stream::gsize()
	{
		unsigned pos = seek(0, Current);
		unsigned siz = seek(0, End);
		seek(pos, Begin);
		return siz;
	}

	unsigned short stream::getLE16()
	{
		unsigned char bin[2];
		read(&bin, sizeof(bin));
		return (bin[0]<<8)|bin[1];
	}

	unsigned stream::getLE32()
	{
		unsigned char bin[4];
		read(&bin, sizeof(bin));
		return (bin[0]<<24)|(bin[1]<<16)|(bin[2]<<8)|(bin[3]);
	}

}