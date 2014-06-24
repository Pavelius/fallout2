#include "f2lib.h"
#include "ai.h"

static unsigned	current_round;
static char		console[256*256];

int world::get(int id)
{
	switch(id)
	{
	case Year:
		return year(current_round);
	case Month:
		return month(current_round);
	case Day:
		return day(current_round);
	case CurrentRound:
		return current_round;
	default:
		return 0;
	}
}

bool world::set(int id, int value)
{
	switch(id)
	{
	case Round:
		current_round = (unsigned)value;
		break;
	default:
		return false;
	}
	return true;
}

unsigned world::round(int year, int month, int day)
{
	return ((1461 * (year - 1600 + (month - 14) / 12)) / 4
		+ (367 * (month - 2 - 12 * ((month - 14) / 12))) / 12
		- (3 * ((year - 1600 + 100 + (month - 14) / 12) / 100)) / 4
		+ day - 32075) * 1440;
}

int world::year(unsigned d)
{
	unsigned ell, n, i, j;
	ell = (d / 1440) + 68569;
	n = (4 * ell) / 146097;
	ell = ell - (146097 * n + 3) / 4;
	i = (4000 * (ell + 1)) / 1461001;
	ell = ell - (1461 * i) / 4 + 31;
	j = (80 * ell) / 2447;
	ell = j / 11;
	return (int)(100 * (n - 49) + i + ell + 6400);
}

int world::month(unsigned d)
{
	unsigned ell, n, i, j;
	ell = (d / 1440) + 68569;
	n = (4 * ell) / 146097;
	ell = ell - (146097 * n + 3) / 4;
	i = (4000 * (ell + 1)) / 1461001;
	ell = ell - (1461 * i) / 4 + 31;
	j = (80 * ell) / 2447;
	ell = j / 11;
	return (int)(j + 2 - (12 * ell));
}

int world::day(unsigned d)
{
	unsigned ell, n, i, j;
	ell = (d / 1440) + 68569;
	n = (4 * ell) / 146097;
	ell = ell - (146097 * n + 3) / 4;
	i = (4000 * (ell + 1)) / 1461001;
	ell = ell - (1461 * i) / 4 + 31;
	j = (80 * ell) / 2447;
	return (int)(ell - (2447 * j) / 80);
}

int world::hour(unsigned d)
{
	return (d / 60) % 24;
}

void world::clear()
{
	current_round = round(2200, 1, 15);
	rmset(console, 0, sizeof(console));
}