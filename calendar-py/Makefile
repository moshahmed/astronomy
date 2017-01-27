
PYTHON=c:/python27/python.exe
YEAR := $(shell date +%Y)
NEXT_YEAR := $(shell date -d "+1 year" +%Y)
DECADE := $(shell date -d "+10 year" +%Y)

# CITY=Mumbai,Sunnyvale,Accra,Kigali,Mangalore
CITY=NYC
HOLIDAY=nul

help:: ; grep -Pi "^[\w-]+:" Makefile

clean::
	rm -fv *.pyc *.pdf planetary*.txt c:/tmp/planet*.txt

test::
	c:/python27/python.exe ephem3.py -c lines -l NYC

cal-pdf:
	# Writes data/calendar-2014-CITY-{jan-jun,jul-dec}.pdf
	$(PYTHON) cal-pdf.py $(CITY) $(YEAR)
	ls -ltrn data/

city::
	# Planetary text calendar for city
	make clean
	$(PYTHON) ephem3.py -c monthly -l $(CITY) \
		-f data/planetary-cal-$(CITY)-$(YEAR).txt \
		-s $(YEAR)/01 \
		-e $(YEAR)/12 \
		-S ''

decade::
	$(PYTHON) ephem3.py -c monthly -l $(CITY) \
		-f data/planetary-calendar-$(CITY)-$(YEAR)-to-$(DECADE).txt \
		-s $(YEAR)/01 \
		-e $(DECADE)/12

yearly::
	$(PYTHON) ephem3.py -c yearly -l $(CITY) \
		-f c:/tmp/ \
		-s $(YEAR)/01 \
		-e $(NEXT_YEAR)/12

tags::
	ctags *.py
