ARCH ?= $(shell getconf LONG_BIT)
DMD ?= dmd
NAMES = main http server
FILES = $(addsuffix .d, $(NAMES))
SRCS  = $(addprefix src/, $(FILES))
DFLAGS = -Isrc -m$(ARCH) -w -d -property -O -release -nofloat -inline

main: $(SRCS)
	$(DMD) $(DFLAGS) -unittest -ofmain $(SRCS)

test: main
	./main test

