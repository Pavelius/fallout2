#include "f2lib.h"
#include "ai.h"

int						map::cols;
int						map::rows;
static int				cols2;
static int				rows2;
const int				stx = 16; // logic horizontal part of tile
const int				sty = 12; // logic vetical part of tile. All tile has 5x3 logic size.
const int				mvc = 24;
const int				mvx = mvc+1;
const int				mvy = mvc+1;
static short unsigned	floors[200*200];
static short unsigned	scenery[200*200];

static short unsigned* tileset(map::tokens t)
{
	static short unsigned causeway[16] = {237, 244, 237, 236, 237, 238, 237, 240, 237};
	return causeway;
}

void map::create()
{
	cols = 100; cols2 = cols*2;
	rows = 100; rows2 = rows*2;
	int m = cols*rows;
	for(int i = 0; i<=m; i++)
		floors[i] = 172 + xrand()%(190-172);
	for(int i = 0; i<=m; i++)
		scenery[i] = 0;
	int i1 = map::pos(0, 40);
	map::generate::highway(i1, 100, 5);
	for(int i = 0; i<20; i++)
	{
		int x = xrand()%cols;
		int y = xrand()%rows;
		int t = y*cols + x;
		if(map::group(t)==Waste)
			map::sscene(t, 49+(xrand()%2));
	}
}

// Получить координату у по карте шестиугольников
int map::gy(int t)
{
	return t/cols2;
}

// Получить координату х по карте шестиугольников
int map::gx(int t)
{
	return t%cols2;
}

// Получить координату х тайла с индексом t на экране
int map::t2x(int t)
{
	int tx = t%cols;
	int ty = t/cols;
	return 2*stx*ty - 3*stx*tx;
}

// Получить координату y тайла с индексом t на экране
int map::t2y(int t)
{
	int tx = t%cols;
	int ty = t/cols;
	return 2*sty*ty + sty*tx;
}

int map::xy2t(int x, int y)
{
	x -= 8;
	y += 20;
	int tx = (x-4*y/3)/64;
	int ty = (x+4*y)/128;
	return ty*cols - tx;
}

int map::h2x(int t)
{
	int tx = t%cols2;
	int ty = t/cols2;
	return stx*(ty - tx - tx/2) - 8;
}

int map::h2y(int t)
{
	int tx = t%cols2;
	int ty = t/cols2;
	return sty*(ty + tx - tx/2) - 32;
}

int map::xy2h(int x, int y)
{
	x += 8;
	y += 32;
	int tx = (y*stx - x*sty)/(stx*sty*2);
	int ty = (3*y*stx + x*sty)/(4*stx*sty);
	return ty*cols2 + tx;
}

void map::sfloor(int i, short unsigned fid)
{
	if(i==-1)
		return;
	floors[i] = fid;
}

short unsigned map::floor(int i)
{
	if(i==-1)
		return 0;
	return floors[i];
}

void map::sscene(int i, short unsigned fid)
{
	if(i==-1)
		return;
	scenery[i] = fid;
}

short unsigned map::scene(int i)
{
	if(i==-1)
		return 0;
	return scenery[i];
}

int map::pos(int tx, int ty)
{
	return ty*cols + tx;
}

int map::pos2(int tx, int ty)
{
	return ty*cols2 + tx;
}

map::tokens map::group(short unsigned tile)
{
	if(tile>=236 && tile<=274)
		return map::Causeway;
	return map::Waste;
}

int map::move(int index, int d)
{
	switch(d)
	{
	case Left:
		if((index%cols)==0)
			return -1;
		return index-1;
	case Right:
		if(((index+1)%cols)==0)
			return -1;
		return index+1;
	case Up:
		if((index/cols)==0)
			return -1;
		return index-cols;
	case Down:
		if((1+index/cols)==rows)
			return -1;
		return index+cols;
	case RightDown:
		if(((index+1)%cols)==0)
			return -1;
		if((1+index/cols)==rows)
			return -1;
		return index+cols+1;
	case RightUp:
		if(((index+1)%cols)==0)
			return -1;
		if((index/cols)==0)
			return -1;
		return index-cols+1;
	case LeftUp:
		if((index%cols)==0)
			return -1;
		if((index/cols)==0)
			return -1;
		return index-cols-1;
	case LeftDown:
		if((index%cols)==0)
			return -1;
		if((index/cols)==0)
			return -1;
		return index+cols-1;
	default:
		return -1;
	}
}

short unsigned map::lookup(int i, map::tokens tile)
{
	const static directions ar[] = {Left, Right, Up, Down};
	unsigned char m = 0;
	for(auto e : ar)
	{
		int i1 = move(i, e);
		if(i1!=-1 && group(floor(i1))!=tile)
			m |= 1;
		m <<= 1;
	}
	short unsigned* a = tileset(tile);
	return a[m];
}

int	map::d2o(directions d)
{
	static int dt[] = {4, 5, 0, 0, 1, 2, 2, 3, 0};
	return dt[d];
}

directions map::direction(int from, int to)
{
	static const directions orientations[25] =
	{
		LeftUp, LeftUp, RightUp, RightUp, RightUp,
		LeftUp, LeftUp, RightUp, RightUp, RightUp,
		Left, Left, Center, Right, Right,
		LeftDown, LeftDown, RightDown, RightDown, RightDown,
		LeftDown, LeftDown, LeftDown, RightDown, RightDown
	};
	int dx = map::h2x(to) - map::h2x(from);
	int dy = map::h2y(to) - map::h2y(from);
	int div = imax(iabs(dx), iabs(dy));
	if(!div)
		return Center; //default
	if(div>3)
		div /= 2;
	int ax = dx/div;
	int ay = dy/div;
	return orientations[(ay+2)*5+ax+2];
}

// Рисует хексагон по указанным координатам мышки.
void ui::show::hexagon(int ox, int oy, int t)
{
	if(t==-1)
		return;
	int x = map::h2x(t) - ox;
	int y = map::h2y(t) - oy;
	image(x, y, res::INTRFACE, 1, FINoOffset);
}

// Рисует сетку на экране. Используется для отладки.
void ui::show::grid(int ox, int oy, int mvc)
{
	int last_view = map::xy2t(ox, oy);
	int x1 = (last_view%map::cols) - mvc; int x2 = x1 + mvc*2;
	int y1 = (last_view/map::cols) - mvc; int y2 = y1 + mvc*2;
	for(int y = y1; y<y2; y++)
	{
		if(y<0 || y>=map::rows)
			continue;
		for(int x = x1; x<x2; x++)
		{
			if(x<0 || x>=map::cols)
				continue;
			int i1 = y*map::cols + x;
			int x2 = map::t2x(i1) - ox;
			int y2 = map::t2y(i1) - oy;
			rectb(x2-map::tilew/2, y2-map::tileh/2, x2+map::tilew/2, y2+map::tileh/2, gcol());
		}
	}
}

// Рисует тайлы ландшафта
void ui::show::tiles(int ox, int oy, int mvc)
{
	int i = map::xy2t(ox, oy);
	int x1 = (i%map::cols) - mvc; int x2 = x1 + mvc*2;
	int y1 = (i/map::cols) - mvc; int y2 = y1 + mvc*2;
	for(int y = y1; y<y2; y++)
	{
		if(y<0 || y>=map::rows)
			continue;
		for(int x = x1; x<x2; x++)
		{
			if(x<0 || x>=map::cols)
				continue;
			int i1 = y*map::cols + x;
			int x2 = map::t2x(i1) - ox;
			int y2 = map::t2y(i1) - oy;
			int t1 = map::floor(i1);
			if(t1>1)
				image(x2, y2, res::TILES, t1);
		}
	}
}

void ui::show::map(int ox, int oy, int mvc)
{
	int last_view = map::xy2t(ox, oy);
	int x1 = (last_view%map::cols) - mvc; int x2 = x1 + mvc*2;
	int y1 = (last_view/map::cols) - mvc; int y2 = y1 + mvc*2;
	// Объекты сцен
	for(int y = y1; y<y2; y++)
	{
		if(y<0 || y>=map::rows)
			continue;
		for(int x = x1; x<x2; x++)
		{
			if(x<0 || x>=map::cols)
				continue;
			int i1 = y*map::cols + x;
			int x2 = map::t2x(i1) - ox;
			int y2 = map::t2y(i1) - oy;
			int t1 = map::scene(i1);
			if(t1>1)
				zsprites.add({{x2, y2}, res::SCENERY, t1, 0, 0});
		}
	}
	// Существа
	for(int mid = FirstCreature; mid<=Player; mid++)
	{
		res::token rs = (res::token)creatures::get(mid, Frame);
		int x2 = creatures::get(mid, PositionX) - ox;
		int y2 = creatures::get(mid, PositionY) - oy;
		int ac = creatures::get(mid, Action);
		int dr = creatures::get(mid, Direction);
		int fr = creatures::get(mid, Animation);
		int c1 = res::gframes(rs, ac) / 6; // вычислим количество спрайтов на каждую анимацию
		switch(ac)
		{
		case ActionRun:
		case ActionWalk:
			zsprites.add({{x2, y2}, rs, ac, dr*c1 + fr, FIReal});
			break;
		default:
			zsprites.add({{x2, y2}, rs, ac, dr*c1 + fr, 0});
			break;
		}
	}
}