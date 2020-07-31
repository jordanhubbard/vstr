#include "vstr.h"

ROUTINE int v_move(v, dir, cnt)
register p_vstr v;
register int dir, cnt;
{
	register int actual = 0;

	entry(v_move)

#ifdef DEBUG
yelp("called with vstr at %x, count of %d, direction = %d\n", v, cnt, dir);
#endif
	if (!cnt)
		ret(0)
	switch (dir) {

		case FWD:
			while (cnt && !IS_BACK(v)) {
				if (++v->cur.u_pos >= v->cur.here->len)
					if (!(v->cur.here = v->cur.here->next)) {
						v->cur.here = v->tail;
						v->cur.u_pos = v->tail->len;
						break;
					}
					else
						v->cur.u_pos = 0;
				cnt--;
				actual++;
			}
			break;

		case BACK:
			while (cnt && !IS_FRONT(v)) {
				if (--v->cur.u_pos < 0)
					if (!(v->cur.here = v->cur.here->prev)) {
						v->cur.here = v->head;
						v->cur.u_pos = -1;
						break;
					}
					else
						v->cur.u_pos = v->cur.here->len - 1;
				actual++;
				cnt--;
			}
			break;
	}
	if (!actual)
		ret((dir == FWD) ? ATEND : ATBEG)
	else
		ret(actual)
}
