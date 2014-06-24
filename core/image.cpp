#include "f2lib.h"

using namespace ui;

static ui::colori	colorinfos[ColorState][2];
static ui::colori*	current_color;

static int store_v1(unsigned char* d, unsigned char* s, int sn, int w, int h)
{
	unsigned char* ds = d;
	// clip upper line
	if(!s || !h)
		return 0;
	// save body
	int h1 = h;
	while(h1)
	{
		unsigned char* p1 = s;
		unsigned char* p2 = &s[w];
		while(p1<p2)
			*d++ = *p1++;
		s += sn;
		h1--;
	}
	return d - ds;
}

// Return count of repeated 'alpha' values
static int skpe(unsigned char* s, unsigned char* s2, int max)
{
	if(s2> s + max)
		s2 = s + max;
	for(unsigned char* s1 = s; s1<s2; s1++)
	{
		if(*s1)
			return s1-s;
	}
	return s2-s;
}

// Return count of repeated not 'alpha' values
static int skpne(unsigned char* s, unsigned char* s2, int max)
{
	if(s2> s + max)
		s2 = s + max;
	for(unsigned char* s1 = s; s1<s2; s1++)
	{
		if(*s1==0)
			return s1-s;
	}
	return s2-s;
}

// Return true if there is no alpha channel (plain texture)
static bool tstv(unsigned char* s, int sn1, int w, int h)
{
	unsigned char* s1 = &s[h*sn1];
	while(s<s1)
	{
		if(skpne(s, &s[w], w)!=w)
			return false;
		s += sn1;
	}
	return true;
}

// (00) end of line
// (01 - 7F) draw count of pixels
// (80, XX) skip count of XX pixels
// 81 - FF skip count of (b-0x80) pixels
// each pixel has a value
static int store_v4(unsigned char* d, unsigned char* s, int sn, int& w, int& h, int& ox, int& oy)
{
	unsigned char* ds = d;
	unsigned char sk;
	// clip upper line
	if(!s || !h)
		return 0;
	// save body
	int h1 = h;
	do
	{
		unsigned char* p1 = s;
		unsigned char* p2 = &s[w];
		while(p1<p2)
		{
			// 1) skip
			sk = skpe(p1, p2, w);
			if(sk)
			{
				if(p1+sk<p2)
				{
					if(sk>=1 && sk<=(0xFF-0x80))
						*d++ = 0x80 + sk;
					else
					{
						*d++ = 0x80;
						*d++ = sk;
					}
				}
				p1 += sk;
				continue;
			}
			// 2) pixels
			sk = skpne(p1, p2, 0x7F);
			*d++ = sk;
			do
			{
				*d++ = *p1++;
			} while(--sk);
		}
		*d++ = 0;
		s += sn;
	} while(--h1);
	return d - ds;
}

static void blit_v1(unsigned char* d, int d1, unsigned char* s, int s1, int h, int w)
{
	if(h<=0 || w<=0)
		return;
	do
	{
#ifdef __GNUC__
		rmcpy(d, s, w);
#else
		_asm
		{
			mov edi, d
				mov esi, s
				mov ecx, w
				rep movsb
		}
#endif
		s += s1;
		d += d1;
	} while(--h);
}

static void blit_v3(unsigned char* d, int d1, unsigned char* s, int s1, int h, int w)
{
	if(h<=0 || w<=0)
		return;
	do
	{
		int i = w;
		unsigned char* s3 = s;
		unsigned char* d3 = d;
		while(i--)
		{
			if(*s3)
				*d3++ = *s3++;
			else
			{
				d3++;
				s3++;
			}
		}
		s += s1;
		d += d1;
	} while(--h);
}

static unsigned char* skip_v4(unsigned char* s, int h)
{
	if(!s || !h)
		return s;
	while(true)
	{
		unsigned char c = *s++;
		if(c==0)
		{
			if(--h==0)
				return s;
		}
		else if(c<=0x7F)
			s += c;
		else if(c==0x80)
			s++;
	}
}

static void blit_v2(unsigned char* dst, int dstw, const unsigned char* src, int srch, const unsigned char* s1, const unsigned char* s2)
{
	unsigned char* d = dst;
	while(true)
	{
		unsigned char c = *src++;
		if(c==0)
		{
			dst += dstw;
			s1 += dstw;
			s2 += dstw;
			d = dst;
			if(--srch==0)
				break;
		}
		else if(c<=0x7F)
		{
			// clip left invisible part
			if(d+c<=s1 || d>s2)
			{
				d += c;
				src += c;
				continue;
			}
			else if(d<s1)
			{
				unsigned char sk = (s1-d);
				d += sk;
				src += sk;
				c -= sk;
			}
			// visible part
			do
			{
				if(d>=s2)
					break;
				*d++ = *src++;
			} while(--c);
			// right clip part
			if(c)
			{
				d += c;
				src += c;
			}
		}
		else
		{
			if(c==0x80)
				d += (*src++);
			else
				d += (c-0x80);
		}
	}
}

static void blit_v5(unsigned char* p1, int d1mc, const unsigned char* s, int h, const unsigned char* s1, const unsigned char* s2, int flags, ui::colori* table)
{
	unsigned char* d = p1;
	while(true)
	{
		unsigned char c = *s++;
		if(c==0)
		{
			p1 += d1mc;
			s1 += d1mc;
			s2 += d1mc;
			d = p1;
			if(--h==0)
				break;
		}
		else if(c<=0x7F)
		{
			// clip left invisible part
			if(d+c<=s1 || d>s2)
			{
				d += c;
				s += c;
				continue;
			}
			else if(d<s1)
			{
				unsigned char sk = (s1-d);
				d += sk;
				s += sk;
				c -= sk;
			}
			// visible part
			do
			{
				if(d>=s2)
					break;
				switch(*s++)
				{
				case 1: *d = table->palet[5][*d]; break;
				case 2: *d = table->palet[4][*d]; break;
				case 3: *d = table->palet[3][*d]; break;
				case 4: *d = table->palet[2][*d]; break;
				case 5: *d = table->palet[1][*d]; break;
				case 6: *d = table->palet[0][*d]; break;
				case 7: *d = table->color[0]; break;
				case 8: *d = table->color[1]; break;
				case 9: *d = table->color[2]; break;
				}
				d++;
			} while(--c);
			// right clip part
			if(c)
			{
				d += c;
				s += c;
			}
		}
		else
		{
			if(c==0x80)
				d += (*s++);
			else
				d += (c-0x80);
		}
	}
}

void ui::store(sprite* image, int id, unsigned char* src, int w, int h, int ox, int oy, formats ft,
	int animation_count, int animation, int action,
	int psx, int psy, int msx, int msy)
{
	int sn = w;
	if(ft==AUTO)
	{
		if(tstv(src, w, w, h))
			ft = RAW;
		else
			ft = RLE;
	}
	frame* frames;
	if(!animation)
	{
		image->cicles[id].count = animation_count;
		image->cicles[id].offset = image->size;
		image->cicles[id].active = action;
		frames = (frame*)image->ptr();
		image->size += image->cicles[id].count*sizeof(frame);
		rmset(frames, 0, image->cicles[id].count*sizeof(frame));
	}
	else
		frames = image->cicles[id].ptr(image) + animation;
	frames->encode = ft;
	frames->cx = psx; frames->cy = psy;
	frames->ox = ox; frames->oy = oy;
	frames->sx = w; frames->sy = h;
	frames->mx = msx; frames->my = msy;
	int sz;
	switch(ft)
	{
	case RAW: sz = store_v1(image->ptr(), src, sn, w, h); break;
	case RLE: sz = store_v4(image->ptr(), src, sn, w, h, ox, oy); break;
	default: return;
	}
	unsigned p = image->find(image->ptr(), sz);
	if(p)
		frames->offset = p;
	else
	{
		frames->offset = image->size;
		image->size += sz;
	}
}

static void blit_sm1(unsigned char* dst, unsigned char* src, int sw, int tw)
{
	int NumPixels = tw;
	int IntPart = sw / tw;
	int FractPart = sw % tw;
	int E = 0;
	while(NumPixels-- > 0) {
		*dst++ = *src;
		src += IntPart;
		E += FractPart;
		if(E >= tw) {
			E -= tw;
			src++;
		}
	}
}

static void scale_line(unsigned char* dst, unsigned char* src, int sw, int tw)
{
	int NumPixels = tw;
	int IntPart = sw / tw;
	int FractPart = sw % tw;
	int E = 0;
	while(NumPixels-- > 0) {
		*dst++ = *src;
		src += IntPart;
		E += FractPart;
		if(E >= tw) {
			E -= tw;
			src++;
		}
	}
}

static void scale(unsigned char* dst, int TgtWidth, int TgtHeight, unsigned char* src, int SrcWidth, int SrcHeight)
{
	int NumPixels = TgtHeight;
	int IntPart = (SrcHeight / TgtHeight) * SrcWidth;
	int FractPart = SrcHeight % TgtHeight;
	int E = 0;
	unsigned char* PrevSource = 0;
	while(NumPixels-- > 0)
	{
		if(src == PrevSource)
			rmcpy(dst, dst-TgtWidth, TgtWidth*sizeof(*dst));
		else
		{
			scale_line(dst, src, SrcWidth, TgtWidth);
			PrevSource = src;
		}
		dst += TgtWidth;
		src += IntPart;
		E += FractPart;
		if(E >= TgtHeight)
		{
			E -= TgtHeight;
			src += SrcWidth;
		}
	}
}

void ui::image(int x, int y, res::token icn, int frame, unsigned flags, int animation)
{
	int x2, y2;
	sprite* p = (sprite*)res::get(icn);
	if(!p)
		return;
	if((unsigned)frame>=(unsigned)p->count)
		frame = ((unsigned)frame) % p->count;
	ui::cicle* c = &p->cicles[frame];
	if(!c->count)
		return;
	ui::frame* f = (ui::frame*)((char*)p + sizeof(res::header) + c->offset);
	f = &f[animation%c->count];
	if(!f->offset)
		return;
	unsigned char* s = (unsigned char*)p + sizeof(res::header) + f->offset;
	if(flags&FIScaleToClip)
	{
		x = f->sx;
		y = f->sy;
		unsigned char* si = new unsigned char[y*x];
		rmset(si, 0, x*y);
		x2 = clipping.width();
		y2 = clipping.height();
		unsigned char* si2 = new unsigned char[x2*y2];
		blit_v2(si, x, s, y, si, si+x);
		scale(si2, x2, y2, si, x, y);
		blit_v3(ptr(clipping.x1, clipping.y1), scanline, si2, x2, y2, x2);
		delete si;
		delete si2;
		return;
	}
	if(flags&FIReal)
		x = x - f->sx/2 + f->mx;
	else if((flags&FINoOffset)==0)
	{
		x = x - f->sx/2 + f->ox;
		if((flags&FINoCenter)==0)
			x += f->cx;
	}
	x2 = x + f->sx;
	if(flags&FIMirrorV)
	{
		y2 = y;
		if(flags&FIReal)
			y2 = y + f->sy - f->my - f->cy;
		else if((flags&FINoOffset)==0)
		{
			y2 = y + f->sy - f->oy;
			if((flags&FINoCenter)==0)
				y2 -= f->cy;
		}
		y = y2 - f->sy;
	}
	else
	{
		if(flags&FIReal)
			y = y - f->sy + f->my + f->cy;
		else if((flags&FINoOffset)==0)
		{
			y = y - f->sy + f->oy;
			if((flags&FINoCenter)==0)
				y += f->cy;
		}
		y2 = y + f->sy;
	}
	if(y2<clipping.y1 || y>clipping.y2 || x2<clipping.x1 || x>clipping.x2)
		return;
	if(y<clipping.y1)
	{
		if((flags&FIMirrorV)==0)
		{
			switch(f->encode)
			{
			case RAW: s += (clipping.y1-y)*f->sx; break;
			case RLE: s = skip_v4(s, clipping.y1-y); break;
			default: break;
			}
		}
		y = clipping.y1;
	}
	if(y2>clipping.y2)
	{
		if(flags&FIMirrorV)
		{
			switch(f->encode)
			{
			case RAW: s += (y2-clipping.y2)*f->sx; break;
			case RLE: s = skip_v4(s, y2-clipping.y2); break;
			default: break;
			}
		}
		y2 = clipping.y2;
	}
	if(y>=y2)
		return;
	int wd = (flags&FIMirrorV) ? -scanline : scanline;
	int sy = (flags&FIMirrorV) ? y2-1 : y;
	switch(f->encode)
	{
	case RAW:
		if(x<clipping.x1)
		{
			s += (clipping.x1-x);
			x = clipping.x1;
		}
		if(x2>clipping.x2)
			x2 = clipping.x2;
		if(x>=x2)
			return;
		blit_v1(ptr(x, y), wd, s, f->sx, y2-y, x2-x);
		break;
	case RLE:
		if(flags&FIUseColors)
			blit_v5(ptr(x, sy), wd, s, y2-y, ptr(clipping.x1, sy), ptr(clipping.x2, sy), flags, current_color);
		else
			blit_v2(ptr(x, sy), wd, s, y2-y, ptr(clipping.x1, sy), ptr(clipping.x2, sy));
		break;
	default:
		break;
	}
}

unsigned char ui::gcol(int n)
{
	return current_color->color[n];
}

void ui::scol(colors color_index)
{
	if(!current_color)
	{
		static unsigned char indexes[] = {0x60, 0xD7, 0x03, 0xE4, 0x3C, 0x90};
		for(int h = 0; h<2; h++)
		{
			for(int id = 0; id<=ColorState; id++)
			{
				unsigned char index = indexes[id];
				color c1 = color::get(index);
				if(h==1)
				{
					c1 = c1.bright(140);
					index = c1.index();
				}
				colorinfos[id][h].color[0] = index;
				colorinfos[id][h].color[1] = c1.bright(110).index();
				colorinfos[id][h].color[2] = c1.bright(120).index();
				for(int j = 0; j<6; j++)
				{
					unsigned char m = 0x20*(j+1);
					for(int i = 1; i<256; i++)
						colorinfos[id][h].palet[j][i] = color::get(i).mix(c1, m).index();
				}
			}
		}
	}
	current_color = &colorinfos[color_index][0];
}

void ui::highlight()
{
	current_color++;
}

frame* ui::gframe(res::token id, int animation, int fid)
{
	sprite* p = (sprite*)get(id);
	if(!p)
		return 0;
	cicle* c = p->cicles + (animation%p->count);
	if(!c->count)
		return 0;
	return (frame*)((char*)p + sizeof(res::header) + c->offset) + (fid%c->count);
}

int res::gframes(res::token icn, int n)
{
	sprite* p = (sprite*)get(icn);
	if(!p)
		return 0;
	cicle* c = &p->cicles[n%p->count];
	return c->count;
}

int res::gaction(res::token icn, int n)
{
	sprite* p = (sprite*)get(icn);
	if(!p)
		return 0;
	cicle* c = &p->cicles[n%p->count];
	return c->active;
}

int res::gvar(res::token icn, int n)
{
	sprite* p = (sprite*)get(icn);
	if(!p)
		return 0;
	if(n>=sizeof(p->vars)/sizeof(p->vars[0]))
		return 0;
	return p->vars[n];
}

ui::states::states()
{
	this->fore = current_color;
	this->font = ui::font;
	this->clipping = ui::clipping;
}

ui::states::~states()
{
	ui::font = this->font;
	ui::clipping = this->clipping;
	current_color = this->fore;
}