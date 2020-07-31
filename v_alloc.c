/*
 * V_ALLOC Version 1.0, 4/16/85
 * Jordan K. Hubbard
 */

#include "vstr.h"

IMPORT char *malloc();

ROUTINE p_unit new_unit(v)
register p_vstr v;
{
	register p_unit tmp;

	entry(new_unit)

	tmp = (p_unit)malloc(sizeof *tmp);
	if (!tmp)
		freak_out(M_NOSPACE);
	tmp->next = tmp->prev = NIL;
	tmp->len = 0;
	tmp->data = malloc(v->unit_size);
	if (!tmp->data)
		freak_out(M_NOSPACE);
	ret(tmp)
}

ROUTINE p_vstr new_vstr(i)
register int i;
{
	register p_vstr tmp;

	entry(new_vstr)

	tmp = (p_vstr)malloc(sizeof *tmp);
	if (!tmp)
		freak_out(M_NOSPACE);
	tmp->head = tmp->tail = tmp->cur.here = NIL;
	tmp->cur.u_pos = -1;
	tmp->unit_size = i;
	ret(tmp)
}
