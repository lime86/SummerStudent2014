## Makefile to build and install Rivet analyses

CC=g++
WFLAGS= -Wall -Wextra
CFLAGS=-m64 -pg -I$(INCDIR) -I$(RIVETINCDIR) -O2 $(WFLAGS) -pedantic -ansi

INCDIR=$(PWD)/include
LIBDIR:=$(shell rivet-config --libdir)
PREFIX:=$(shell rivet-config --prefix)
RIVETINCDIR:=$(shell rivet-config --includedir)
LDFLAGS:=$(shell rivet-config --ldflags)


## Get the first part of the string passed
ANALYSISPATH = $(firstword $(subst :, ,$1))

all: Razor
	
Razor: Razor.cc
	#$(CC) -shared -fPIC $(CFLAGS) $(HORRIBLE_STRING_OF_BULLSHIT_DUE_TO_LXPLUS) -o "RivetCERNLingxin.so" Razor.cc -L ./ $(LDFLAGS)
	#rivet-buildplugin RivetCERNLingxin.so Razor.cc -L/afs/cern.ch/sw/lcg/experimental/rivet/local/lib/
	rivet-buildplugin RivetCERNLingxin.so Razor.cc -L/afs/cern.ch/user/l/lmeng/local/lib/

install: Razor

	cp -f RivetCERNLingxin.so $(call ANALYSISPATH, $(RIVET_ANALYSIS_PATH))
	cp -f Razor.plot $(call ANALYSISPATH, $(RIVET_REF_PATH))

clean:
	rm -f *.o  *.so
	rm -f ../PythiaNeutralino/main42.exe
	rm -f ../PythiaShowering/main32.exe
	rm -f ../PythiaDijet/main42.exe
