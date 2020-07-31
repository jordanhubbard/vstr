/* The number of units to scan ahead for holes when inserting characters */
/* (as opposed to just creating a new unit). Scanning isn't so */
/* much overhead as is copying characters to the hole, so it's best not */
/* to define COST too large unless you're using large units and/or doing */
/* a fair number of deletions. Perhaps some more clever way of doing */
/* insertions will be implemented some day. (I welcome any suggestions) */

#define COST 4

/* To turn on routine tracing and general annoying messages #define
 * DEBUG, as usual. Modify _end(), _entry() and _ret() for more advanced
 * debugging.
 */

/* #define DEBUG 1  */

/*
 * Define this to be the type of variable you want to store the length
 * and index variables in. Read OPERATION for more details..
 */

#define LENTYPE short

#ifdef DEBUG

#define entry(s) _entry("s");
#define ret(s) { _ret(s); return(s); }
#define end { _end(); return; }

#else

#define entry(s) _Curr_rtn = "s";
#define ret(s) return(s);
#define end return;

#endif

/*
 * utility macros
 */

#define IMPORT extern
#define EXPORT
#define ROUTINE
#define forever for(;;)
#define IS_FRONT(v) (v->cur.here == v->head && v->cur.u_pos == -1)
#define IS_BACK(v) (v->cur.here == v->tail \
 && (v->cur.u_pos == v->cur.here->len))

typedef unsigned char boolean;

/*
 * messages/error codes/constants
 */

#define M_NOSPACE "out of memory/arena corrupted\n"
#define M_NILVSTR "null virtual string passed\n"

#define ATEND		-1
#define ATBEG		-2

#define NIL		0
#define BACK	1
#define FWD		2
#define HERE	3
#define BEG		4
#define END		5
#define INS		6

/*
 * declarations
 */

struct unit {
	struct unit *prev, *next;
	LENTYPE len;
	char *data;
};

typedef struct unit *p_unit;

struct cursor {
	struct unit *here;
	LENTYPE u_pos;
};

typedef struct cursor *p_cursor;

struct vstr {
	p_unit head, tail;
	LENTYPE unit_size;
	struct cursor cur;
};

typedef struct vstr *p_vstr;

#ifndef DEBUG
IMPORT char *_Curr_rtn;
#endif

IMPORT char *v_nvtos();
IMPORT char *v_vtos();
IMPORT int v_delete();
IMPORT int v_get();
IMPORT int v_length();
IMPORT int v_move();
IMPORT int v_span();
IMPORT p_cursor v_find();
IMPORT p_cursor v_savecursor();
IMPORT p_unit new_unit();
IMPORT p_vstr new_vstr();
IMPORT void freak_out();
IMPORT void v_append();
IMPORT void v_clear();
IMPORT void v_copycursor();
IMPORT void v_free();
IMPORT void v_put();
IMPORT void v_rewind();
IMPORT void v_setcursor();
IMPORT void v_show();
IMPORT void v_stov();
