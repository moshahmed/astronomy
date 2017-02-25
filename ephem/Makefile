# Makefile for ephem, v 4.28

CFLAGS = -O

EPHEM=	aa_hadec.o altj.o altmenus.o anomaly.o cal_mjd.o circum.o comet.o \
	compiler.o constel.o eq_ecl.o flog.o formats.o io.o listing.o main.o \
	mainmenu.o moon.o moonnf.o nutation.o objx.o obliq.o parallax.o \
	pelement.o plans.o plot.o popup.o precess.o reduce.o refract.o \
	riset.o riset_c.o sel_fld.o sex_dec.o srch.o sun.o time.o utc_gst.o \
	version.o watch.o

clean::
	rm -fv *.o *.obj *.OBJ ephem/debug/*.* ephem/*.{opt,ncb,plg}

patch::
	diff -rwb . ../ephem.orig/ > win32.dif ;\
	echo zip win32.zip c:/tmp/win32.dif main.c io.c ephem/ephem.dsp ephem/ephem.dsw

ephem:	$(EPHEM)
	cc -o $@ $(EPHEM) -ltermcap -lm

aa_hadec.o:	astro.h

altj.o:		astro.h circum.h screen.h

altmenus.o:	astro.h circum.h screen.h

anomaly.o:	astro.h

cal_mjd.o:	astro.h

circum.o:	astro.h circum.h screen.h

comet.o:	astro.h

compiler.o:	screen.h

constel.o:	astro.h circum.h screen.h

eq_ecl.o:	astro.h

flog.o:		screen.h

formats.o:	astro.h screen.h

io.o:		screen.h

listing.o:	screen.h

main.o:		astro.h circum.h screen.h

mainmenu.o:	astro.h circum.h screen.h

moon.o:		astro.h

moonnf.o:	astro.h

nutation.o:	astro.h

objx.o:		astro.h circum.h screen.h

obliq.o:	astro.h

parallax.o:	astro.h

pelement.o:	astro.h

plans.o:	astro.h

plot.o:		screen.h

popup.o:	screen.h

precess.o:	astro.h

reduce.o:	astro.h

refract.o:	astro.h

riset.o:	astro.h

riset_c.o:	astro.h circum.h screen.h

sel_fld.o:	screen.h

srch.o:		screen.h

sun.o:		astro.h

time.o:		astro.h circum.h

utc_gst.o:	astro.h

version.o:	screen.h

watch.o:	astro.h circum.h screen.h
