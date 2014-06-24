#include "f2lib.h"

unsigned char* ui::sprite::ptr()
{
	return (unsigned char*)this + size + sizeof(header);
}

void ui::sprite::setup(int count)
{
	set("SPR");
	this->count = count;
	this->size = sizeof(sprite) - sizeof(header) + (count-1)*sizeof(cicle);
	rmset(cicles, 0, count*sizeof(cicle));
}

void ui::sprite::write(const char* url)
{
	io::file file(url, io::file::Write);
	if(!file.isvalid())
		return;
	file.write(this, sizeof(header) + size);
}

ui::frame* ui::cicle::ptr(ui::sprite* p) const
{
	if(!offset)
		return 0;
	return (ui::frame*)((char*)p + sizeof(res::header) + offset);
}

unsigned char* ui::frame::ptr(ui::sprite* p) const
{
	if(!offset)
		return 0;
	return (unsigned char*)p + sizeof(res::header) + offset;
}

unsigned ui::sprite::find(unsigned char* p, int count)
{
	for(int i = 0; i<this->count; i++)
	{
		cicle* c = &cicles[i];
		for(int j = 0; j<c->count; j++)
		{
			frame* f = c->ptr(this);
			if(!f->offset)
				continue;
			int max_size = size - f->offset;
			if(max_size<count)
				continue;
			unsigned char* c1 = f->ptr(this);
			if(rmcmp(c1, p, count)==0)
				return f->offset;
		}
	}
	return 0;
}