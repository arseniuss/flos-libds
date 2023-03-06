#!env make
.POSIX:

LIB = lib/libds.a

HDRS = \
    include/ds/common.h \
    include/ds/hashset.h \
    include/ds/pair.h \
    include/ds/vec.h \
    include/ds.h

SRCS = \
    source/common.c \
    source/hashset.c \
    source/vec.c

TESTSRCS = \
    test/hashset_test.c \
    test/pair_test.c \
    test/vec_test.c

include config.mk

OBJS != echo $(SRCS:.c=.o) | sed -e 's/source\//$(BUILDDIR)\//g'
DEPS != echo $(SRCS:.c=.d) | sed -e 's/source\//$(BUILDDIR)\//g'
PPS != echo $(SRCS:.c=.c.pp) | sed -e 's/source\//$(BUILDDIR)\//g'
TESTS != echo $(TESTSRCS:.c=) | sed -e 's/test\//$(BUILDDIR)\//g'

.SUFFIXES:
.PHONY: all options clean

all: $(LIB)

$(LIB): $(OBJS)
	mkdir -p lib
	rm -f $@
	$(AR) -rcs $@ $^

$(BUILDDIR)/%.o: source/%.c
	@mkdir -p $(BUILDDIR)/$(*D)
	$(PP) $(CFLAGS) $< > $(BUILDDIR)/$*.c.pp
	$(CC) $(CFLAGS) -MMD -MF $(BUILDDIR)/$*.d -c -o $@ $<

tests: $(TESTS) $(LIB)
	$(PROVE) $(PROVE_FLAGS) $(TESTS)

build/%: test/%.c $(LIB)
	@mkdir -p $(BUILDDIR)/$(*D)
	$(PP) $(CFLAGS) $< > $(BUILDDIR)/$*.c.pp
	$(CC) $(CFLAGS) -MMD -MF $(BUILDDIR)/$*.d -o $@ $^

clean:
	rm -rf $(BUILDDIR) lib libds-$(VERSION).tar.gz

dist: clean
	mkdir -p libds-$(VERSION)
	cp --parents $(HDRS) $(SRCS) $(TESTSRCS) test/tap.h LICENSE README.md config.mk makefile libds-$(VERSION)
	tar -cf libds-$(VERSION).tar libds-$(VERSION)
	gzip libds-$(VERSION).tar
	rm -rf libds-$(VERSION)

install: all

-include $(DEPS)
