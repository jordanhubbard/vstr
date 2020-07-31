#include "vstr.h"

ROUTINE void v_free(v)
register p_vstr v;
{
	register p_unit temp;

	entry(v_free)

	if (!v)
		freak_out(M_NILVSTR);
	temp = v->head;
	while (temp) {
		free(temp);
		temp = temp->next;
	}
	free(v);
	end
}
