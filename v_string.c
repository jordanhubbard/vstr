#include "vstr.h"
#include <stdlib.h>
#include <string.h>

ROUTINE void v_stov(s, v)
register char *s;
register p_vstr v;
{
	register int i, l;

	entry(v_stov)

	l = strlen(s);
#ifdef DEBUG
yelp("Putting string at %x, length of %d into vstr at %x\n", s, l, v);
#endif
	for (i = 0; i < l; i++)
		v_put(v, s[i], FWD);
	end
}

ROUTINE char *v_vtos(v)
register p_vstr v;
{
	register char *s;
	register int i = 0;
	register char ch;

	entry(v_vtos)

	s = malloc(v_length(v, HERE) + 1);
#ifdef DEBUG
yelp("making string at %x, %d chars long for vstr at %x", s, v_length(v, HERE) +
  1, v);
#endif
	if (!s)
		freak_out(M_NOSPACE);
	while ((ch = v_get(v, FWD)) != ATEND)
		s[i++] = ch;
	s[i] = '\0';
	ret(s)
}

ROUTINE char *v_nvtos(v, n)
register p_vstr v;
register int n;
{
	register char *s;
	register int i = 0;
	register char ch;

	entry(v_nvtos)

	s = malloc(n + 1);
#ifdef DEBUG
yelp("making string at %x, %d chars long for vstr at %x", s, n + 1, v);
#endif
	if (!s)
		freak_out(M_NOSPACE);
	while ((ch = v_get(v, FWD)) != ATEND && n) {
		s[i++] = ch;
		n--;
	}
	s[i] = '\0';
	ret(s)
}
