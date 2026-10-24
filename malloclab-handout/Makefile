#
# Students' Makefile for the Malloc Lab
#
TEAM = bovik
VERSION = 1
HANDINDIR = /afs/cs.cmu.edu/academic/class/15213-f01/malloclab/handin

CC = gcc
CXX = g++
CFLAGS = -Wall -Og -m32
CXXFLAGS = -Wall -Og -m32 -std=c++23

OBJS = mdriver.o memlib.o fsecs.o fcyc.o clock.o ftimer.o
MM_TARGETS = $(basename $(wildcard mm*.cc))

MAKEFLAGS += --no-builtin-rules
.SUFFIXES:

.PHONY: $(MM_TARGETS)
$(MM_TARGETS): %: %.out

%.out: %.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cc mm.h memlib.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

mdriver.o: mdriver.c fsecs.h fcyc.h clock.h memlib.h config.h mm.h
memlib.o: memlib.c memlib.h
fsecs.o: fsecs.c fsecs.h config.h
fcyc.o: fcyc.c fcyc.h
ftimer.o: ftimer.c ftimer.h config.h
clock.o: clock.c clock.h

handin:
	cp mm.c $(HANDINDIR)/$(TEAM)-$(VERSION)-mm.c

clean:
	rm -f *~ *.o *.out


