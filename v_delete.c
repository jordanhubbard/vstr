#include "vstr.h"
#include <string.h>

IMPORT void linkout();

ROUTINE int v_delete(v, cnt)
register p_vstr v;
register int cnt;
{
	register p_unit temp;
	register int actual = 0, n;

	entry(v_delete)

	if (!cnt)
		ret(0)
	while (cnt) {
		temp = v->cur.here;
		n = temp->len - v->cur.u_pos;
		if (!n) {
			if (!temp->next)
				ret(actual)
			else {
				v->cur.here = temp->next;
				v->cur.u_pos = 0;
			}
			continue;
		}
		if (n <= cnt) {
			if (!v->cur.u_pos) {
				cnt -= temp->len;
				actual += temp->len;
				linkout(v, temp);
				if (v->cur.here == temp->prev)
					cnt = 0;
			}
			else {
				temp->len -= n;
				cnt -= n;
				actual += n;
				if (temp->next) {
					v->cur.here = temp->next;
					v->cur.u_pos = 0;
				}
				else
					cnt = 0;
			}
		}
		else {
			strncpy(temp->data + v->cur.u_pos,
				temp->data + v->cur.u_pos + cnt, n);
			temp->len -= cnt;
			actual += cnt;
			cnt = 0;
		}
	}
	ret(actual)
}
