#include "f2lib.h"
#include "ai.h"
#include "formats.h"

#pragma pack(push)
#pragma pack(1)
struct map_header
{
	unsigned	version;
	char		name[16];
	unsigned	start;
	unsigned	elevation;
	unsigned	orientation;
	unsigned	locale_count;
	unsigned	script_id;
	unsigned	flags;
	unsigned	unknown;
	unsigned	global_count;
	unsigned	map_id;
	unsigned	times;
	unsigned	reserv[44];
};
#pragma pack(pop)

void cnvmap(const char* dst, const char* src)
{
	map_header* p = (map_header*)io::file::load(src);
	swap(p->version);
	swap(p->global_count);
	swap(p->locale_count);
	swap(p->script_id);
	swap(p->elevation);
	swap(p->flags);
	swap(p->map_id);
	swap(p->orientation);
	swap(p->start);
	swap(p->times);
	if(p->version==20)
	{

	}
	delete p;
}