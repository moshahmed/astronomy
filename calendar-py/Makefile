PYTHON=c:/python36/python.exe

YEAR := $(shell date +%Y)
NEXT_YEAR := $(shell date -d "+1 year" +%Y)
DECADE := $(shell date -d "+10 year" +%Y)

TMP=c:/tmp
OUTDIR=out

# CITY=Mumbai,Sunnyvale,Accra,Kigali,Mangalore,NYC,SanMateo
CITY=Sunnyvale
HOLIDAY=nul

help:: ; make -qps | grep -Pi "^\w+:"

all:
	make cal-pdf
	make ephem-city



cal-pdf:
	# Writes $(OUTDIR)/calendar-2014-CITY-{jan-jun,jul-dec}.pdf
	$(PYTHON) cal-pdf.py $(CITY) $(YEAR)
	ls -ltrn $(OUTDIR)/

ephem-city:: # Planetary text calendar for city
	$(PYTHON) ephem3.py -c monthly -l $(CITY) \
		-f $(OUTDIR)/planetary-cal-$(CITY)-$(YEAR).txt \
		-s $(YEAR)/01 \
		-e $(YEAR)/12 \
		-E $(HOLIDAY) \
		-S ''

ephem-decade::
	$(PYTHON) ephem3.py -c monthly -l $(CITY) \
		-f $(OUTDIR)/planetary-calendar-$(CITY)-$(YEAR)-to-$(DECADE).txt \
		-s $(YEAR)/01 \
		-e $(DECADE)/12

ephem-yearly::
	$(PYTHON) ephem3.py -c yearly -l $(CITY) \
		-f $(TMP)/ \
		-s $(YEAR)/01 \
		-e $(NEXT_YEAR)/12


tags::
	ctags *.py *.pl
