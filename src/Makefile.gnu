# This is a plain Makefile for general Phorward Foundation Toolkit development. 
# It should compile libphorward.a quickly and out of the box without autotools.

CFLAGS += -DUNICODE -DUTF8 -I../src

LIBPHORWARD = libphorward.a
PROTOFILE =	proto.h
LIBHEADER = phorward.h
PATHEXT = PATH="../run:$(PATH)"

SRC = \
	base/bitset.c \
	base/dbg.c \
	base/hashtab.c \
	base/llist.c \
	base/plist.c \
	base/memory.c \
	base/stack.c \
	ccl/ccl.c \
	regex/dfa.c \
	regex/direct.c \
	regex/misc.c \
	regex/nfa.c \
	regex/pattern.c \
	regex/pregex.c \
	regex/range.c \
	regex/ref.c \
	string/convert.c \
	string/string.c \
	string/utf8.c \
	util/base64.c \
	util/system.c \
	union/union.c \
	union/union.conv.c \
	union/union.get.c \
	union/union.set.c \
	xml/xml.c

HSRC = \
	base/bitset.h \
	base/dbg.h \
	base/hashtab.h \
	base/llist.h \
	base/plist.h \
	base/stack.h \
	ccl/ccl.h \
	phorward.tpl.h \
	regex/pregex.h \
	string/string.h \
	string/utf8.h \
	union/union.h \
	xml/xml.h \
	$(PROTOFILE)

OBJ = $(patsubst %.c,%.o,$(SRC))

$(LIBPHORWARD): $(LIBHEADER) $(OBJ)
	ar rv $@ $(OBJ)

clean:
	-rm -f $(OBJ)
	-rm -f $(LIBPHORWARD)

# Prototypes Files
$(PROTOFILE): $(SRC)
	$(PATHEXT) pproto $(SRC) >$@

# Library Header
$(LIBHEADER): $(HSRC)
	$(PATHEXT) pinclude phorward.tpl.h >$@

# Variant Data Type Modules (generated from definitions in var.h comments)
union/union.get.c: union/union.h union/union.gen.awk
	$(AWK) -f union/union.gen.awk -vwith_get=1 union/union.h >$@

union/union.set.c: union/union.h union/union.gen.awk
	$(AWK) -f union/union.gen.awk -vwith_set=1 union/union.h >$@

union/union.conv.c: union/union.h union/union.gen.awk
	$(AWK) -f union/union.gen.awk -vwith_conv=1 union/union.h >$@
