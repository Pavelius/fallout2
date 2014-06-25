#include "f2lib.h"

const unsigned Impassable = 0xFFF0;
static unsigned short	path_stack[256*256];
static unsigned short	path_push;
static unsigned short	path_pop;
static unsigned short	path_goal;
static unsigned short	path_start;
static unsigned short	path[256*256];

static void snode(unsigned short index, unsigned short cost)
{
	if(index==-1)
		return;
	auto a = path[index];
	if(a>=Impassable)
		return;
	if(a!=0 && cost>=a)
		return;
	path_stack[path_push++] = index;
	path[index] = cost;
}

static void gnext(unsigned short index, short unsigned& level, short unsigned& pos)
{
	if(index==-1)
		return;
	auto nlevel = path[index];
	if(!nlevel)
		return;
	if(nlevel <= level)
	{
		level = nlevel;
		pos = index;
	}
}

static bool ispathable(int index)
{
	return true;
}

short unsigned movecost(int index)
{
	if(index==-1)
		return 0xFFFF;
	return path[index];
}

short unsigned movecost(int index, unsigned char direct, unsigned pathfinding)
{
	return 1;
}

// First, make wave and see what cell on map is passable
static void makewave(int start, int skill, int ship_master)
{
	rmset(path, 0, sizeof(path));
	if(start==-1)
		return;
	int count = map::cols*map::rows;
	for(int i = 0; i<count; i++)
		path[i] = ispathable(i) ? 0 : Impassable;
	path_push = 0;
	path_pop = 0;
	path_stack[path_push++] = start;
	path[start] = 1;
	while(path_push != path_pop)
	{
		auto pos = path_stack[path_pop++];
		auto cost = path[pos];
		if(cost>=Impassable)
			break;
		snode(map::move(pos, Left), cost+1);
		snode(map::move(pos, Right), cost+1);
		snode(map::move(pos, LeftUp), cost+1);
		snode(map::move(pos, LeftDown), cost+1);
		snode(map::move(pos, RightUp), cost+1);
		snode(map::move(pos, RightDown), cost+1);
	}
	path_pop = 0;
	path_push = 0;
	path_goal = -1;
	path_start = start;
}

// Second calculate path to any cell on map use wave result
void makepath(int start, int goal)
{
	path_push = 0;
	path_goal = -1;
	short unsigned pos = goal;
	unsigned short level = Impassable;
	while(pos != start)
	{
		auto n = pos;
		gnext(map::move(pos, Left), level, n);
		gnext(map::move(pos, Right), level, n);
		gnext(map::move(pos, Up), level, n);
		gnext(map::move(pos, Down), level, n);
		gnext(map::move(pos, LeftDown), level, n);
		gnext(map::move(pos, LeftUp), level, n);
		gnext(map::move(pos, RightDown), level, n);
		gnext(map::move(pos, RightUp), level, n);
		if(pos==n)
			return;
		pos = n;
		path_stack[path_push++] = n;
		level = path[pos];
	}
	path_goal = goal;
}