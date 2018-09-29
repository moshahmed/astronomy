PYTHON=c:/python36/python.exe

YEAR := $(shell date +%Y)
NEXT_YEAR := $(shell date -d "+1 year" +%Y)
YEAR10 := $(shell date -d "+10 year" +%Y)

TMP=c:/tmp
OUTDIR=out
BUNDLEDIR=bundle

# CITY=Mumbai,Sunnyvale,Accra,Kigali,Mangalore,NYC,SanMateo
# See cities.py > city2loc.pl > c:/doc1/astro/bin/CITIES.DAT"
CITY=SanMateo
HOLIDAY=c:/mosh2/addr-pub/holiday.txt
HOLIDAY=nul

help:: ; make -qps | grep -Pi "^[\w-]+:"
help2:: ; grep -Pi "^[\w-]+:" Makefile

tags::
	ctags *.py *.pl

all:
	make caly
	make ephem-decade

caly: # Writes $(OUTDIR)/caly-YYYY-CITY-{01-06,07-12}.pdf
	$(PYTHON) caly.py $(CITY) $(YEAR) $(OUTDIR)/caly-
	ls -ltrn $(OUTDIR)/caly-*

caly-decade::
	for iyear in {$(YEAR)..$(YEAR10)} ;do \
	  $(PYTHON) caly.py $(CITY) $$iyear $(OUTDIR)/caly- ;\
	done
	mkdir -p $(BUNDLEDIR)
	pdftk $(OUTDIR)/caly-*.pdf cat output $(BUNDLEDIR)/caly-$(YEAR)-to-$(YEAR10).pdf
	rm -f $(OUTDIR)/caly-*.pdf
	ls -l $(BUNDLEDIR)

ephem-decade::
	$(PYTHON) ephem3.py -c monthly -l $(CITY) \
		-f $(BUNDLEDIR)/ephem-$(CITY)-$(YEAR)-to-$(YEAR10).txt \
		-s $(YEAR)/01 \
		-e $(YEAR10)/12

ephem-yearly::
	$(PYTHON) ephem3.py -c yearly -l $(CITY) \
		-f $(TMP)/ \
		-s $(YEAR)/01 \
		-e $(NEXT_YEAR)/12

