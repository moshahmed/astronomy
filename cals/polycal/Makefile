# moshahmed_at_gmail
YEAR:=$(shell date +%Y)
NEXT_YEAR:=$(shell date -d "+10 year" +%Y)
FILEPREFIX=polycal
OUTDIR=out
BUNDLEDIR=bundle

all::
	make decade
	make bundle

decade::
	# Make polyhedreal pdf calendar for years in outdir, deskcal2 > ps > pdf
	-rm    -fv $(OUTDIR)/$(FILEPREFIX)-????.{ps,pdf}
	mkdir -p $(OUTDIR)
	for iyear in {$(YEAR)..$(NEXT_YEAR)} ;do \
	  FILENAME=$(OUTDIR)/$(FILEPREFIX)-$$iyear ;\
	  bash   deskcal2.sh $$iyear > $$FILENAME.ps ;\
	  ps2pdf $$FILENAME.ps $$FILENAME.pdf ;\
	  rm     $$FILENAME.ps ;\
	  ls  -l $$FILENAME.pdf ;\
	done

bundle::
	mkdir -p $(BUNDLEDIR)
	pdftk  $(OUTDIR)/$(FILEPREFIX)-????.pdf cat output $(BUNDLEDIR)/$(FILEPREFIX)-$(YEAR)-to-$(NEXT_YEAR).pdf
	rm -rf $(OUTDIR)/$(FILEPREFIX)-????.pdf
	ls -l $(BUNDLEDIR)
