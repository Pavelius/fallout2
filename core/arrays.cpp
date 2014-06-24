#include "f2lib.h"

int arrays::count(int* a)
{
	if(!a)
		return 0;
	int* a1 = a;
	while(*a++);
	return a - a1 - 1;
}

int arrays::random(int* a)
{
	int c = count(a);
	if(!c)
		return 0;
	return a[xrand()%c];
}