VERSION = 0.1

AR = ar
CC  = cc
LD = ld
PP = cpp
PROVE = prove

FLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_XOPEN_SOURCE=700L -DVERSION=\"$(VERSION)\"
#CFLAGS = -Iinclude -g -std=c99 -pedantic -Wall -O0 $(FLAGS)
CFLAGS = -DNDEBUG -Iinclude -std=c99 -pedantic -Wall -Wno-deprecated-declarations -Os $(FLAGS)

BUILDDIR = build
