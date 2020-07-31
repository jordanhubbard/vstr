/*
 * V_GET Version 1.0, 4/16/85
 * Jordan K. Hubbard
 */

#include "vstr.h"

ROUTINE int v_get(v, mode)
register p_vstr v;
register int mode;
{
	register int i;

	entry(v_get)

#ifdef DEBUG
yelp("called with vstr at %x, mode = %d\n", v, mode);
#endif
	if (!v)
		freak_out(M_NILVSTR);
	if (!v->head)
		ret((mode == BACK) ? ATBEG : ATEND)
	switch(mode) {

		case FWD:
			if (v_move(v, FWD, 1) == ATEND)
				ret(ATEND)
			i = (int)v->cur.here->data[v->cur.u_pos];
			break;

		case BACK:
			if (v_move(v, BACK, 1) == ATBEG)
				ret(ATBEG)
			i = (int)v->cur.here->data[v->cur.u_pos];
			break;

		case HERE:
			if (IS_BACK(v))
				ret(ATEND)
			i = (int)v->cur.here->data[v->cur.u_pos];
			break;
	}
	ret(i)
}
