# Makefile for vstr package, 04/16/85 Jordan K. Hubbard
# Updated for current millennium (FOR ABSOLUTELY NO REASON) 07/31/2020

PREFIX=/usr/local

OBJS=	v_put.o v_get.o v_alloc.o v_debug.o v_rewind.o v_append.o v_utils.o\
		v_string.o v_delete.o v_move.o v_cursor.o v_free.o v_clear.o v_find.o

INCS=vstr.h

libvstr.a: $(OBJS)
	ar r libvstr.a $(OBJS)
	ranlib libvstr.a

$(OBJS): $(INCS)

install: libvstr.a
	install -c libvstr.a $(PREFIX)/lib
	install -c $(INCS) $(PREFIX)/include
	install -c vstr.man $(PREFIX)/man/manl/vstr.l
	ranlib $(LIB)/libvstr.a

clean:
	rm -f $(OBJS) libvstr.a
