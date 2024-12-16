#!env make
.POSIX:

TARGETS = \
	lib/libds.a

HDRS = \
    include/flos/ds/common.h \
    include/flos/ds/hashset.h \
    include/flos/ds/pair.h \
    include/flos/ds/vec.h \
    include/flos/ds.h

SRCS = \
    source/common.c \
    source/hashset.c \
    source/vec.c

TESTSRCS = \
    test/hashset_test.c \
    test/pair_test.c \
    test/vec_test.c

include config.mk

CFLAGS += -Iinclude

OBJS != echo $(SRCS:.c=.o) | sed -e 's/source\//$(builddir)\//g'
DEPS != echo $(SRCS:.c=.d) | sed -e 's/source\//$(builddir)\//g'
PPS != echo $(SRCS:.c=.c.pp) | sed -e 's/source\//$(builddir)\//g'
TESTS != echo $(TESTSRCS:.c=) | sed -e 's/test\//$(builddir)\//g'

.SUFFIXES:
.PHONY: all check clean dist install

all: $(TARGETS)

$(TARGETS): $(OBJS)
	mkdir -p lib
	rm -f $@
	$(AR) -rcs $@ $^

$(builddir)/%.o: source/%.c
	@mkdir -p $(builddir)/$(*D)
	$(PP) $(CFLAGS) $< > $(builddir)/$*.c.pp
	$(CC) $(CFLAGS) -MMD -MF $(builddir)/$*.d -c -o $@ $<

check: $(TESTS) $(TARGETS)
	$(PROVE) $(PROVE_FLAGS) $(TESTS)

build/%: test/%.c $(TARGETS)
	@mkdir -p $(builddir)/$(*D)
	$(PP) $(CFLAGS) $< > $(builddir)/$*.c.pp
	$(CC) $(CFLAGS) -MMD -MF $(builddir)/$*.d -o $@ $^

clean:
	rm -rf $(builddir) lib libds-$(VERSION).tar.gz

dist: clean
	mkdir -p libds-$(VERSION)
	cp --parents $(HDRS) $(SRCS) $(TESTSRCS) test/tap.h LICENSE README.md config.mk makefile libds-$(VERSION)
	tar -cf libds-$(VERSION).tar libds-$(VERSION)
	gzip libds-$(VERSION).tar
	rm -rf libds-$(VERSION)

install: all

-include $(DEPS)
