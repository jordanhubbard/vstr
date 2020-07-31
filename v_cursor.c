#include "vstr.h"

ROUTINE p_cursor v_savecursor(v, c)
register p_vstr v;
register p_cursor c;
{
	entry(v_savecursor)

	c->here = v->cur.here;
	c->u_pos = v->cur.u_pos;
#ifdef DEBUG
yelp("saving cursor at unit %x, pos %d\n", c->here, c->u_pos);
#endif
	ret(c)
}

ROUTINE void v_setcursor(v, c)
register p_vstr v;
register p_cursor c;
{
	entry(v_setcursor)

	v->cur.here = c->here;
	v->cur.u_pos = c->u_pos;
#ifdef DEBUG
yelp("setting cursor to unit %u, pos %d\n", c->here, c->u_pos);
#endif
	end
}

ROUTINE void v_copycursor(c1, c2)
register p_cursor c1, c2;
{
	entry(v_copycursor)

	c1->here = c2->here;
	c1->u_pos = c2->u_pos;
	end
}

ROUTINE int v_span(c1, c2)
register p_cursor c1, c2;
{
	register int cnt = 0;
	register p_unit tmp = c1->here;

	entry(v_span)

	while (tmp && (tmp != c2->here)) {
		cnt += tmp->len;
		tmp = tmp->next;
	}
	if (tmp)
		cnt += c2->u_pos;
	ret(cnt)
}
