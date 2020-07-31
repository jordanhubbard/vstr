/*
 * V_DEBUG Version 1.0 4/16/85
 * Jordan K. Hubbard
 */

#include "vstr.h"
#include <stdio.h>
#include <stdlib.h>

#define PUSH	1
#define POP		2
#define LOOK	3

#ifdef DEBUG
char *id();

ROUTINE void yelp(fmt, p1, p2, p3, p4, p5, p6)
register char *fmt;
register unsigned char *p1, *p2, *p3, *p4, *p5, *p6;
{
	fprintf(stderr, "DEBUG: [%s] ", id(0, LOOK));
	fprintf(stderr, fmt, p1, p2, p3, p4, p5, p6);
	return;
}

ROUTINE char *id(s, mode)
register char *s;
register int mode;
{
	/* if we get more than 50 subroutine levels deep, something's wrong */
	/* with our program anyway (or we've taken modular programming too far!) */
	static char *stack[50];
	static int pointer = 0;
	
	switch (mode) {

		case PUSH:
			stack[pointer++] = s;
			if (pointer == 50) {
				yelp("stack overflow in id!\n");
				exit(1);
			}
			break;

		case POP:
			if (pointer - 1 < 0) {
				yelp("stack underflow in id!\n");
				exit(1);
			}
			return(stack[--pointer]);
			break;

		case LOOK:
			if (pointer - 1 < 0) {
				yelp("empty stack for 'look' in id!\n");
				exit(1);
			}
			return(stack[pointer - 1]);
			break;
	}
}

ROUTINE void _end()
{
	yelp("Exiting with no returned value\n");
	id(0, POP);
	return;
}

ROUTINE void _entry(s)
register char *s;
{
	id(s, PUSH);
	yelp("Entering\n");
	return;
}

ROUTINE void _ret(l)
unsigned char *l;
{
	yelp("Exiting with %u (hex: %x)\n", l, l);
	id(0, POP);
	return;
}

#else

EXPORT char *_Curr_rtn;

#endif

/* Makes character 'visible' by turning it into a printable control */
/* character if it is one or its octal \nnn form if it is greater than */
/* the upper limit for printable ascii */

ROUTINE char *visible(ch)
register char ch;
{
	static char buf[5];

	entry(visible)

	buf[0] = '\0';
	if (ch < 32) {
		buf[0] = '^';
		ch += 64;
	}
	if (ch > 31 && ch < 127) {
		if (buf[0]) {
			buf[1] = ch;
			buf[2] = '\0';
		}
		else {
			buf[0] = ch;
			buf[1] = '\0';
		}
	}
	else if (ch > 126) {
		sprintf(buf, "\\%03o", ch);
		buf[4] = '\0';
	}
	ret(buf)
}

ROUTINE void v_show(v)
register p_vstr v;
{
	register p_unit temp;
	register int n_units = 0, n_chars = 0, l;
	register int n_funits = 0;

	entry(v_show)

	if (!v) {
		fprintf(stderr, "v_show: vstr is NIL\n");
		end
	}
	fprintf(stderr, "Debugging information for vstr at %p\n", v);
	fprintf(stderr, "Head is at %p, Tail at %p\n", v->head, v->tail);
	fprintf(stderr, "Unit size is %d\n", v->unit_size);
	fprintf(stderr, "Cursor points to unit %p, position %d\n\n",
	  v->cur.here, v->cur.u_pos);
	temp = v->head;
	while (temp) {
		fprintf(stderr, "prev     unit     next\n");
		fprintf(stderr, "(%p) <= [%p] => (%p)   ", temp->prev, temp,
		  temp->next);
		if (temp == v->head)
			fprintf(stderr, "HEAD  ");
		if (temp == v->tail)
			fprintf(stderr, "TAIL  ");
		fprintf(stderr, "#%d\n", n_units);
		fprintf(stderr, "\tLength = %d", temp->len);
		if (temp->len) {
			n_chars += temp->len;
			fprintf(stderr, "  Data = '");
			for (l = 0; l < temp->len; l++)
				fprintf(stderr, "%s", visible(temp->data[l]));
			fprintf(stderr, "'\n\n");
			if (temp->len == v->unit_size)
				n_funits++;
		}
		else
			fprintf(stderr, "\n\n");
		temp = temp->next;
		n_units++;
	}
	fprintf(stderr, "\n\nTotal: %d characters in %d units. %d full, %d not.\n",
	    n_chars, n_units, n_funits, n_units - n_funits);
	end
}

ROUTINE void freak_out(fmt, p1, p2, p3, p4, p5, p6)
register char *fmt;
register int p1, p2, p3, p4, p5, p6;
{
#ifdef DEBUG
	fprintf(stderr, "%s: ", id(0, LOOK));
#else
	fprintf(stderr, "%s: ", _Curr_rtn);
#endif
	fprintf(stderr, fmt, p1, p2, p3, p4, p5, p6);
	exit(1);
}
