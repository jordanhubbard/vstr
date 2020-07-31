#include "vstr.h"

ROUTINE void v_append(v)
register p_vstr v;
{
	entry(v_append)

	v->cur.here = v->tail;
	v->cur.u_pos = v->cur.here->len;
	end
}
