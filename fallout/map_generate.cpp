#include "f2lib.h"

static void terrain(int i1, int width, int t1, int t2, int t3, int* ra = 0)
{
	map::sfloor(i1, t1);
	i1 = map::move(i1, Down);
	for(int i = 0; i<width; i++)
	{
		if(ra && game::d100()<3)
			map::sfloor(i1, arrays::random(ra));
		else
			map::sfloor(i1, t2);
		i1 = map::move(i1, Down);
	}
	map::sfloor(i1, t3);
}

// 0 - lu
// 1 - u
// 2 - u2
// 3 - ru
// 4 - r
// 5 - r2
// 6 - rd
// 7 - d
// 8 - d2
// 9 - dl
// 10 - l
// 11 - l2
void map::generate::highway(int i1, int count, int width)
{
	static int randoms[] = {272, 271, 270, 0};
	const int base = 257;
	const int base1 = 237;
	terrain(i1, width, base+3, base+4, base+6);
	i1 = map::move(i1, Right);
	for(int i = 0; i<count; i++)
	{
		terrain(i1, width, base+2, base1, base+7, randoms);
		i1 = map::move(i1, Right);
	}
	terrain(i1, width, base+0, base+10, base+9);
}