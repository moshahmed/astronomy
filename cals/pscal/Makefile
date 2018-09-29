# moshahmed_at_gmail

YEAR=$(shell date +%Y)
NEXT_YEAR := $(shell date -d "+10 year" +%Y)
MONTH=$(shell date +%m)
FILEPREFIX=pscal

OUTDIR=out
BUNDLEDIR=bundle

all::
	make decade
	make bundle

decade::
	# Make 12 page lunar calender for each month and year.
	-rm -fv $(OUTDIR)/$(FILEPREFIX)-*.pdf
	for iyear in {$(YEAR)..$(NEXT_YEAR)} ;do \
	  for imon in $$(seq -f %02g 1 12) ;do \
	      FILENAME=$(OUTDIR)/$(FILEPREFIX)-$$iyear-$$imon ;\
	      bash  pscal.sh -d 4 -m $$imon $$iyear > $$FILENAME.ps ;\
	      ps2pdf   $$FILENAME.ps $$FILENAME.pdf ;\
	      ls -l    $$FILENAME.pdf ;\
	  done;\
	done
	rm       $(OUTDIR)/$(FILEPREFIX)-*.ps
	ls -l    $(OUTDIR)/$(FILEPREFIX)-*.pdf

bundle::
	pdftk  $(OUTDIR)/$(FILEPREFIX)-*.pdf cat output $(BUNDLEDIR)/$(FILEPREFIX)-$(YEAR)-to-$(NEXT_YEAR).pdf
	rm -fv $(OUTDIR)/$(FILEPREFIX)-*.pdf
	ls -l  $(BUNDLEDIR)

