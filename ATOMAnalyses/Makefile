## Makefile to build and install ATOM analyses

CC=g++
#WFLAGS= -Wall -Wextra
CFLAGS=-std=c++11 -m64 -pg -O2 $(WFLAGS) -pedantic -ansi -shared -fPIC

INCDIR=$(shell atom-config --cppflags) $(shell root-config --cflags)
LDFLAGS=$(shell atom-config --ldflags)  -lAtomCore -lAtomBase -lAtomData -lAtomDetAtlas -lAtomDetCms -lAtomHepMC_IO -lAtomLHE_IO -lAtomProjections -lAtomSimulation -lAtomStdHep_IO -lAtomTools -lAtomWeight_IO -lRivetBase -lRivetProjections -lRivetTools -lfastjet -lfastjettools -lfastjetplugins $(shell root-config --libs)

## Get the first part of the string passed
ANALYSISPATH = $(firstword $(subst :, ,$1))

all: libAtom_ATLAS_razor_0.so	libAtom_ATLAS_razor_1.so
##libAtom_Razor.so: Razor.cc
##	$(CC) $(CFLAGS) $(INCDIR) -o "libAtom_Razor.so" Razor.cc $(LDFLAGS)
libAtom_ATLAS_razor_0.so: ATLAS_razor_0.cc
	$(CC) $(CFLAGS) $(INCDIR) -o "libAtom_ATLAS_razor_0.so" ATLAS_razor_0.cc $(LDFLAGS)
	
libAtom_ATLAS_razor_1.so: ATLAS_razor_1.cc
	$(CC) $(CFLAGS) $(INCDIR) -o "libAtom_ATLAS_razor_1.so" ATLAS_razor_1.cc $(LDFLAGS)

install: libAtom_ATLAS_razor_0.so libAtom_ATLAS_razor_1.so
	cp -f libAtom_ATLAS_razor_0.so $(call ANALYSISPATH, $(ATOM_ANALYSIS_PATH))
#	cp -f ATLAS_razor_0.plot $(call ANALYSISPATH, $(RIVET_PLOT_PATH))
#	cp -f ATLAS_razor_0.yoda $(call ANALYSISPATH, $(RIVET_REF_PATH))
#	cp -f ATLAS_razor_0.yoda $(call ANALYSISPATH, $(ATOM_REF_PATH))
#	cp -f ATLAS_razor_0.info $(call ANALYSISPATH, $(RIVET_INFO_PATH))
	
	cp -f libAtom_ATLAS_razor_1.so $(call ANALYSISPATH, $(ATOM_ANALYSIS_PATH))
#	cp -f ATLAS_razor_1.plot $(call ANALYSISPATH, $(RIVET_PLOT_PATH))
#	cp -f ATLAS_razor_1.yoda $(call ANALYSISPATH, $(RIVET_REF_PATH))
#	cp -f ATLAS_razor_1.yoda $(call ANALYSISPATH, $(ATOM_REF_PATH))
#	cp -f ATLAS_razor_1.info $(call ANALYSISPATH, $(RIVET_INFO_PATH))

clean:
	rm -f *.o  *.so
	rm -f ../PythiaNeutralino/main42.exe
	rm -f ../PythiaShowering/main32.exe
	rm -f ../PythiaDijet/main42.exe
