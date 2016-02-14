# Makefile for Astro Lib & DarkCal Program, for gnu make and gcc.

ALL_SRCS=$(wildcard *.c *.cpp *.c++ *.cxx *.h *.hpp *.hxx *.cfg Makefile)

LIBOBJS = AstroOps.o DateOps.o DateOpsIntl.o PlanetData.o Lunar.o Pluto.o MathOps.o RiseSet.o Vsop.o
#VsopData.o
LIB = Astro.a
DC_BASE = DarkCal
DC = $(DC_BASE).exe
#DEBUG = -g
CPPFLAGS = $(DEBUG) -ffast-math -Wall
TMPOUT=$(TMP)/m.txt

YEAR := $(shell date +%Y)
YEAR2 := $(shell date -d "+2 year" +%Y)

cal2:: $(ALL_SRCS) Astro.a ConfigFile.o
	rm -fv ./sun-rise-set.exe
	$(CXX) $(CPPFLAGS) -o sun-rise-set.exe mosh.cpp Astro.a ConfigFile.o ;\
	./sun-rise-set.exe -from $(YEAR)/01/01 -to $(YEAR2)/05/09 > $(TMPOUT) ;\
	echo "File $(TMPOUT) is" ;\
	head -8 $(TMPOUT) ; echo ... ; \
	grep -A 1 -B 1 $$(date +%Y/%m/%d) $(TMPOUT) ; echo ... ; \
	tail -2 $(TMPOUT) ;\
	./sun-rise-set.exe -days 1


cal:: test

exe: $(LIB) $(DC) test

$(LIBOBJS):

$(LIB) : $(LIBOBJS)
	ar -r $(LIB) $(LIBOBJS)

$(DC): $(DC_BASE).o ConfigFile.o $(LIB)
	$(CXX) -o $(DC) $(DC_BASE).o ConfigFile.o $(LIB)

AstroOps.o: AstroOps.h
ConfigFile.o: ConfigFile.h
$(DC_BASE).o: AstroOps.h PlanetData.h DateOps.h RiseSet.h ConfigFile.h
DateOps.o: DateOps.h
DateOpsIntl.o: DateOps.h
PlanetData.o: PlanetData.h AstroOps.h MathOps.h Lunar.h Pluto.h Vsop.h
Lunar.o: Lunar.h LunarTerms.h
Pluto.o: Pluto.h PlutoTerms.h
MathOps.o: MathOps.h AstroOps.h
RiseSet.o: RiseSet.h AstroOps.h PlanetData.h Vsop.h
Vsop.o: Vsop.h VsopData.cpp
DarkCal.o: DarkCal.cpp DarkCal.h

tags:: $(ALL_SRCS)
	ctags -R .

clean:
	rm -fv $(LIB) $(DC) $(wildcard *.o) *.a # tags g++
	rm -rfv *.obj *.exe *.stackdump
	rm -rfv vs9/Debug/

test:: $(DC) tags $(wildcard *.cfg)
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
	

