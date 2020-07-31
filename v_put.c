/*
 *  V_PUT Version 1.0, 4/16/85
 * Jordan K. Hubbard
 */

#include "vstr.h"

IMPORT p_unit linkin();
IMPORT void shift();
IMPORT boolean ripple();

ROUTINE void v_put(v, c, mode)
register p_vstr v;
register char c;
register int mode;
{
	register int n;

	entry(v_put)

#ifdef DEBUG
yelp("called with vstr at %x, char = %c, mode = %d\n", v, c, mode);
if (!v->head)
	yelp("vstr is cleared\n");
#endif
	if (!v)
		freak_out(M_NILVSTR);
	if (!v->head) {
		v->head = v->tail = v->cur.here = new_unit(v);
		v->cur.u_pos = -1;
	}

	/*
	 * The awful nasty evil label here is so that some cases can mung
	 * around with the text and then change the mode and goto. It's
	 * crude, but it seems better that a recursive function call.. Look
	 * at the 'END' or 'BEG' case and you'll see what I mean..
	 */
sw:	switch(mode) {

		case FWD:
			if (v_move(v, FWD, 1) == ATEND) {
				if (v->cur.here->len < v->unit_size) {
					v->cur.here->data[v->cur.here->len] = c;
					v->cur.u_pos++;
				}
				else {
					v->cur.here = v->tail = linkin(v, FWD);
					*(v->cur.here->data) = c;
					v->cur.u_pos = 0;
				}
			}
			else
				v->cur.here->data[v->cur.u_pos] = c;
			if (v->cur.here->len <= v->cur.u_pos)
				v->cur.here->len++;
			break;

		case BACK:
			if (v_move(v, BACK, 1) == ATBEG) {
				if (v->cur.here->len < v->unit_size) {
					shift(v->cur.here, 0);
					*(v->cur.here->data) = c;
					v->cur.u_pos = 0;
				}
				else {
					v->cur.here = v->head = linkin(v, BACK);
					v->cur.here->len = 1;
					v->cur.u_pos = 0;
					*(v->cur.here->data) = c;
				}
			}
			else
				v->cur.here->data[v->cur.u_pos] = c;
			break;

		case HERE:
			if (IS_BACK(v))
				end;
			v->cur.here->data[v->cur.u_pos] = c;
			break;

		case BEG:
			v_rewind(v);
			mode = BACK;
			goto sw;
			break;

		case END:
			v_append(v);
			mode = FWD;
			goto sw;
			break;

		case INS:
			/*
			 * If we're at the end, then this isn't really an insert.
			 * I know it's kludge, but I don't wanna rewrite the append
			 * code!
			 */
			if (v_move(v, FWD, 1) == ATEND) {
				mode = FWD;
				goto sw;
			}
			if (v->cur.here->len < v->unit_size) {
				if (v->cur.u_pos < v->cur.here->len) 
					shift(v->cur.here, v->cur.u_pos);
				else 
					v->cur.here->len++;
			}
			else if (!ripple(v)) {
				linkin(v, FWD);
				ripple(v);
			}
			v->cur.here->data[v->cur.u_pos] = c;
			break;

		default:
			freak_out("Bad mode passed, mode = %d, char = %d\n", mode, c);
			break;
	}
	end
}
