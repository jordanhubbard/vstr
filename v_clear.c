#include "vstr.h"

ROUTINE void v_clear(v, how)
register p_vstr v;
register int how;
{
	register p_unit temp;

	entry(v_clear)

	if (!v && !v->head)
		end
	if (how == END) {
		temp = v->cur.here;
		if (v->cur.u_pos) {
			temp->len = v->cur.u_pos;
			temp = temp->next;
		}
	}
	else
		temp = v->head;
	while (temp) {
		free(temp);
		temp = temp->next;
	}
	v->head = v->tail = v->cur.here = 0;
	v->cur.u_pos = -1;
	end
}
