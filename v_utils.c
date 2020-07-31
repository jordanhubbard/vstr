#include "vstr.h"
#include <stdio.h>
#include <stdlib.h>

ROUTINE p_unit linkin(v, mode)
register p_vstr v;
register int mode;
{
	register p_unit temp, p1, u = v->cur.here;

	entry(linkin)

#ifdef DEBUG
yelp("called with vstr at %x, mode = %d\n", v, mode);
#endif
	switch(mode) {

		case FWD:
			temp = new_unit(v);
			p1 = u->next;
			temp->prev = u;
			u->next = temp;
			temp->next = p1;
			if (p1)
				p1->prev = temp;
			break;

		case BACK:
			temp = new_unit(v);
			p1 = u->prev;
			temp->next = u;
			u->prev = temp;
			temp->prev = p1;
			if (p1)
				p1->next = temp;
			break;

		default:
			freak_out("passed weird mode %d\n", mode);
			ret(NULL)
			break;
	}
	ret(temp)
}

ROUTINE void linkout(v, u)
register p_vstr v;
register p_unit u;
{
	register p_unit p1, p2;

	entry(linkout)

#ifdef DEBUG
yelp("linking out unit at %x from vstr at %x\n", u, v);
#endif
	p1 = u->prev;
	p2 = u->next;
	if (p1)
		p1->next = p2;
	else
		v->head = p2;
	if (p2)
		p2->prev = p1;
	else
		v->tail = p1;
	if (p2) {
		v->cur.here = p2;
		v->cur.u_pos = 0;
	}
	else {
		v->cur.here = p1;
		v->cur.u_pos = (p1 ? p1->len : 0);
	}
	free(u);
	end
}

ROUTINE void shift(u, pos)
register p_unit u;
register int pos;
{
	register int i = u->len;

	entry(shift)

#ifdef DEBUG
yelp("shifting from %d to %d in unit at %x\n", i, pos, u);
#endif
	if (!i)
		freak_out("passed an empty unit at %u\n", u);
	while(i != pos) {
		u->data[i] = u->data[i - 1];
		i--;
	}
	u->len++;
	end
}

ROUTINE boolean ripple(v)
register p_vstr v;
{
	struct cursor pos;
	register int i = COST;
	register p_unit tmp = v->cur.here;
	register p_unit dest_u = tmp;
	register int dest_p = v->cur.u_pos;
	
	entry(ripple)

	/* Scan for a hole */
	pos.here = 0;
	while (i--) {
		if (!tmp->next) {
			v->tail = tmp->next = new_unit(v);
			tmp->next->prev = tmp;
			pos.here = tmp->next;
			pos.u_pos = 0;
#ifdef DEBUG
yelp("found end while scanning, linked in unit at %x\n", pos.here);
#endif
			break;
		}
		else if (tmp->len < v->unit_size) {
			pos.u_pos = tmp->len;
			pos.here = tmp;
#ifdef DEBUG
yelp("Found hole at %x(%d)\n", pos.here, pos.u_pos);
#endif
			break;
		}
		else
			tmp = tmp->next;
	}
	if (!pos.here)
		ret(0)
	pos.here->len++;
#ifdef DEBUG
yelp("rippling from %x(%d) to %x(%d)\n", pos.here, pos.u_pos, dest_u, dest_p);
#endif
	while (!(pos.here == dest_u && pos.u_pos == dest_p)) {
		if (!pos.u_pos) {
			tmp = pos.here->prev;
			pos.u_pos = tmp->len - 1;
			*(pos.here->data) = tmp->data[pos.u_pos];
			pos.here = tmp;
		}
		else {
			pos.here->data[pos.u_pos] = pos.here->data[pos.u_pos - 1];
			pos.u_pos--;
		}
	}
	ret(1)
}

ROUTINE int v_length(v, how)
register p_vstr v;
register int how;
{
	register int i = 0;
	register p_unit temp;

	entry(v_length)

#ifdef DEBUG
yelp("Getting length for vstr at %x, how = %d\n", v, how);
#endif
	if (!v || !v->head)
		ret(0)
	switch(how) {

		case HERE:
			temp = v->head;
			while (temp) {
				i += temp->len;
				temp = temp->next;
			}
			break;

		case BEG:
			if (IS_FRONT(v))
				break;
			temp = v->cur.here;
			if (v->cur.u_pos < temp->len) {
				i = v->cur.u_pos + 1;
				temp = temp->prev;
			}
			while(temp) {
				i += temp->len;
				temp = temp->prev;
			}
			break;

		case END:
			if (IS_BACK(v))
				break;
			temp = v->cur.here;
			i = temp->len - v->cur.u_pos;
			while (temp) {
				i += temp->len;
				temp = temp->next;
			} 
			break;
	}
	ret(i)
}
