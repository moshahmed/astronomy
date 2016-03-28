# Makefile for Astro Lib & DarkCal Program, for gnu make and gcc.

ALL_SRCS=$(wildcard *.c *.cpp *.c++ *.cxx *.h *.hpp *.hxx *.cfg Makefile)

LIB_SRC=AstroOps.cpp DateOps.cpp DateOpsIntl.cpp Lunar.cpp MathOps.cpp PlanetData.cpp Pluto.cpp RiseSet.cpp Vsop.cpp

LIBOBJS = AstroOps.o DateOps.o DateOpsIntl.o PlanetData.o Lunar.o Pluto.o MathOps.o RiseSet.o Vsop.o

ASTRO_LIB = Astro.a
MAIN_PROG = DarkCal
# EXE_NAME = $(MAIN_PROG).exe
#DEBUG = -g
# CPPFLAGS = $(DEBUG) -ffast-math -Wall
EXE_NAME=riser.exe

CPPFLAGS = $(DEBUG) -Wall
TMPOUT=$(TMP)/m.txt

YEAR := $(shell date +%Y)
YEAR2 := $(shell date -d "+2 year" +%Y)

vc6::
	@echo Building 32bit $(EXE_NAME) with msdev/vc6
	cl.exe /W3 mosh.cpp ConfigFile.cpp AstroOps.cpp DateOps.cpp DateOpsIntl.cpp PlanetData.cpp Lunar.cpp Pluto.cpp MathOps.cpp RiseSet.cpp Vsop.cpp /link /out:$(EXE_NAME)

# =================================================

build:: Astro.a ConfigFile.o
	@echo Building 64bit $(EXE_NAME) with gcc
	$(CXX) $(CPPFLAGS) -o $(EXE_NAME) mosh.cpp Astro.a ConfigFile.o

today:: build
	./$(EXE_NAME) -v -debug -today

year:: build
	./$(EXE_NAME) -from $(YEAR)/01/01 -to $(YEAR2)/05/09 > $(TMPOUT) ;\
	echo "File $(TMPOUT) is" ;\
	head -8 $(TMPOUT) ; echo ... ; \
	grep -A 1 -B 1 $$(date +%Y/%m/%d) $(TMPOUT) ; echo ... ; \
	tail -2 $(TMPOUT) ;\

years:: years2

test:: #build
	./$(EXE_NAME) -debug -cfg DarkCal.cfg \
	  -from 2015/01/01 -to 2015/01/02 > test1.tmp
	./$(EXE_NAME) -debug \
	  -lat 12.887261 -lon 74.831657 -tz +5.5  -city Mangalore,India \
	  -from 2015/01/01 -to 2015/01/02 > test2.tmp
	diff3 test*.tmp

exe: $(ASTRO_LIB) $(EXE_NAME) years2

$(LIBOBJS): $(LIB_SRC)

$(ASTRO_LIB) : $(LIBOBJS)
	ar -r $(ASTRO_LIB) $(LIBOBJS)

$(EXE_NAME): $(MAIN_PROG).o ConfigFile.o $(ASTRO_LIB)
	$(CXX) -o $(EXE_NAME) $(MAIN_PROG).o ConfigFile.o $(ASTRO_LIB)

AstroOps.o: AstroOps.h
ConfigFile.o: ConfigFile.h
$(MAIN_PROG).o: AstroOps.h PlanetData.h DateOps.h RiseSet.h ConfigFile.h
DateOps.o: DateOps.h
DateOpsIntl.o: DateOps.h
PlanetData.o: PlanetData.h AstroOps.h MathOps.h Lunar.h Pluto.h Vsop.h
Lunar.o: Lunar.h LunarTerms.h
Pluto.o: Pluto.h PlutoTerms.h
MathOps.o: MathOps.h AstroOps.h
RiseSet.o: RiseSet.h AstroOps.h PlanetData.h Vsop.h
Vsop.o: Vsop.h VsopData.cpp
DarkCal.o: DarkCal.cpp DarkCal.h

tags::
	ctags -R .

clean:
	rm -fv $(ASTRO_LIB) $(EXE_NAME) $(wildcard *.o) *.a # tags g++
	rm -rfv *.obj *.exe *.stackdump *.tmp
	rm -rfv vs9/Debug/

years2:: $(EXE_NAME)
	# ./DarkCal $$(date +"%m") $$(date +"%Y")
	@output=mlr-$(YEAR)-2020.txt ;\
	echo "Writing $$output" ;\
	for year in `seq $(YEAR) $(YEAR2)` ;do \
	  echo DarkCal for year $$year 1>&2 ;\
	  echo "{{ YEAR-$$year" ;\
	  for month in `seq 1 12` ;do \
	    echo "{ $$year-$$month" ;\
	    ./DarkCal $$month $$year ;\
	    echo "} $$year-$$month" ;\
	  done ;\
	  echo "}} $$year" ;\
	done > $$output ;\
	echo "vim:fdm=marker:fmr={,}:fen:fcl=all:" >> $$output ;\
	echo "File $$output is" ;\
	head -8 $$output ;\
	echo "..." ;\
	tail -5 $$output ;\

