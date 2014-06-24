#include "crt.h"

bool point::in(const point p1, const point p2, const point p3) const
{
	int a = (p1.x - x) * (p2.y - p1.y) - (p2.x - p1.x) * (p1.y - y);
	int b = (p2.x - x) * (p3.y - p2.y) - (p3.x - p2.x) * (p2.y - y);
	int c = (p3.x - x) * (p1.y - p3.y) - (p1.x - p3.x) * (p3.y - y);
	return (a >= 0 && b >= 0 && c >= 0)
		|| (a < 0 && b < 0 && c < 0);
}