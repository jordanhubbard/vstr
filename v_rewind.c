#include "vstr.h"

ROUTINE void v_rewind(v)
register p_vstr v;
{
	entry(v_rewind)

	v->cur.here = v->head;
	v->cur.u_pos = -1;
	end
}
