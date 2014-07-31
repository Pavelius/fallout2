#include "f2lib.h"
#include "formats.h"

const char* szcpp(const char* p, int count)
{
	static char temp[2048];
	if(count==-1)
		count = szlen(p);
	const char* p2 = p + count;
	char* p3 = temp;
	while(p<p2)
	{
		if(*p=='\n')
		{
			p++;
			if(*p=='\r')
				p++;
			continue;
		}
		else if(*p=='\r')
		{
			p++;
			if(*p=='\n')
				p++;
			continue;
		}
		else if(*p=='\"')
		{
			*p3++ = '\\';
			*p3++ = '\"';
			p++;
		}
		else
			*p3++ = *p++;
	}
	*p3 = 0;
	return temp;
}

void cnvmsg(const char* dst, const char* url)
{
	const char* content = szdecode((char*)io::file::load(url, 0, 1));
	io::file file(dst, io::file::Write);
	file << "#include \"ai.h\"\n";
	file << "#include \"formats.h\"\n\n";
	file << "msgitem elements[] = {\n";
	const char* p = content;
	while(*p)
	{
		// element id
		p = szskip(p, '{'); if(*p==0) break;
		int id = sz2num(p, &p);
		p = szskip(p, '}'); if(*p==0) break;
		file << "\t{" << id << ",";
		// element sound
		p = szskip(p, '{'); if(*p==0) break;
		p = szskip(p, '}'); if(*p==0) break;
		// element content
		p = szskip(p, '{'); if(*p==0) break;
		const char* p1 = p;
		p = szskip(p, '}'); if(*p==0) break;
		int m = p - p1 - 1;
		file << "\"" << szcpp(p1,m) << "\"},\n";
	}
	file << "};";
}

const char*	szname(int id, msgitem* items)
{
	for(int i = 0; items[i].id; i++)
		if(items[i].id==id)
			return items[i].text;
	if(id==-1)
		return "";
	return "<?>";
}

void cnvper(const char* dst, msgitem* ms, int b1, int b2, int d1)
{
	io::file file(dst, io::file::Write);
	file << "#include \"ai.h\"\n\n";
	file << "object_perk objects[] = {\n";
	for(int i = b1; i<=b2; i++)
	{
		file << "\t{-1, ";
		file << "{\"" << szcpp(szname(i, ms)) << "\"}, ";
		file << "{\"" << szcpp(szname(i-b1+d1, ms)) << "\"}";
		file << "},\n";
	}
	file << "};";
}