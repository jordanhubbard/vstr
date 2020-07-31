# Makefile for vstr package, 04/16/85 Jordan K. Hubbard

INC=/usr/include/local
LIB=/usr/lib
OBJS=	v_put.o v_get.o v_alloc.o v_debug.o v_rewind.o v_append.o v_utils.o\
		v_string.o v_delete.o v_move.o v_cursor.o v_free.o v_clear.o v_find.o

INCS=vstr.h

.c.o:
	cc -O -c $<

libvstr.a: $(OBJS)
	ar r libvstr.a $(OBJS)
	ranlib libvstr.a

$(OBJS): $(INCS)

install: libvstr.a
	cp libvstr.a $(LIB)
	cp $(INCS) $(INC)
	cp vstr.man /usr/man/manl/vstr.l
	ranlib $(LIB)/libvstr.a
