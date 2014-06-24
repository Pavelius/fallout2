#include "crt.h"
#include "ui.h"

ui::counter::counter() : frame(0), tick(timeticks())
{
}

bool ui::counter::pass(unsigned milliseconds)
{
	unsigned n = timeticks();
	if((n-tick)>milliseconds)
	{
		tick = n;
		return true;
	}
	return false;
}