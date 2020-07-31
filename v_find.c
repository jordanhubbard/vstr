#include "vstr.h"

ROUTINE p_cursor v_find(v, ch, dir)
register p_vstr v;
register char ch;
register int dir;
{
	static struct cursor found;
	struct cursor save;
	register int i;

	entry(v_find)

	if (dir != FWD && dir != BACK)
		freak_out("called with bad mode (not BACK or FWD)\n");
	v_savecursor(v, &save);
	while ((i = v_get(v, dir)) != (dir == FWD ? ATEND : ATBEG))
		if (i == ch) {
			v_savecursor(v, &found);
			v_setcursor(v, &save);
			ret(&found)
		}
	v_setcursor(v, &save);
	ret(0)
}
