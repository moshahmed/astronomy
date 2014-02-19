PYTHON=c:/python25/python.exe
YEAR := $(shell date +%Y)
NEXT_YEAR := $(shell date -d "+1 year" +%Y)
# CITY=Mumbai,Sunnyvale,Accra,Kigali,Mangalore
# CITY=Mumbai
CITY=Mangalore

help:: ; grep -Pi "^[\w-]+:" Makefile

clean::
	rm -fv *.pyc *.pdf planetary*.txt c:/tmp/planet*.txt

cal-pdf:
	# Writes data/calendar-2014-CITY-{jan-jun,jul-dec}.pdf
	$(PYTHON) cal-pdf.py $(CITY) $(YEAR)
	ls -ltrn data/

mangalore::
	# Planetary text calendar for mangalore
	$(PYTHON) ephem3.py -c monthly -l $(CITY) \
		-f data/planetary-cal-$(CITY)-$(YEAR).txt \
		-s $(YEAR)/01 \
		-e $(YEAR)/12 \
		-S ''

decade::
	export YEAR=2014 NEXT_YEAR=2023 CITY=Mangalore ;\
	$(PYTHON) ephem3.py -c monthly -l $$CITY \
		-f data/planetary-calendar-$$CITY-$$YEAR-to-$$NEXT_YEAR.txt \
		-s $$YEAR/01 \
		-e $$NEXT_YEAR/12

yearly::
	$(PYTHON) ephem3.py -c yearly -l $(CITY) \
		-f c:/tmp/ \
		-s $(YEAR)/01 \
		-e $(NEXT_YEAR)/12

tags::
	ctags *.py
