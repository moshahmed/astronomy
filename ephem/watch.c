/* these functions allow you to watch the sky or the solar system via a
 * simple character-graphics representation on the screen. 
 * the interaction starts by using the current time. then control with
 *    END returns to table form; or
 *    RETURN advances time by one StpSz; or
 *    h advances once by 1 hour; or
 *    d advances once by 24 hours (1 day); or
 *    w advances once by 7 days (1 week); or
 *    any other key free runs by StpSz until any key is hit.
 */

#include <stdio.h>
#include <math.h>
#include "astro.h"
#include "circum.h"
#include "screen.h"

#define	SSZCOL	1		/* column to show solar system z coords */
#define	PARK_ROW	1	/* cursor park loc after each screen */
#define	PARK_COL	NC	/* cursor park loc after each screen */

#define	DOMESKY		0	/* flags for watch_sky() */
#define	ALTAZSKY	1	/* flags for watch_sky() */

#define	SKYACC	3600.	/* desired sky plot accuracy, in arc seconds */
#define	SSACC	3600.	/* desired solar system plot accuracy, in arc secs */

/* macros to convert row(col) in range 1..NR(1..NC) to fraction in range 0..1 */
#define	NEARONE		0.9999999
#define	r2fr(r)		(((r)-1)/(NEARONE*NR)+1/NC/2)
#define	c2fc(c)		(((c)-1)/(NEARONE*NC)+1/NC/2)
#define	fr2r(fr)	((int)((fr)*(NEARONE*NR))+1)
#define	fc2c(fc)	((int)((fc)*(NEARONE*NC))+1)

/* single-character tag for each body.
 * order must match the #defines in astro.h and screen.h additions.
 */
static char body_tags[] = "evmjsunpSMxy";

/* multiple and single loop prompts */
static char frprompt[] = "Running... press any key to stop.";
static char qprompt[]  =
"q to quit, RETURN/h/d/w to step by StpSz/hr/day/wk, or any other to freerun";

/* used to locate, record and then erase last plotted chars */
typedef struct {
    double l_fr, l_fc;	/* 2d coords as 0..1 (upper left corner is (0,0)) */
    int	 l_r, l_c;	/* screen 2d coords (upper left corner is [1,1]) */
    char l_tag;		/* char to use to print on screen */
} LastDraw;

static int trails;	/* !0 if want to leave trails */

watch (np, tminc, wbodies)
Now *np;	/* time now and on each step */
double tminc;	/* hrs to increment time by each step */
int wbodies;	/* each bit is !=0 if want that body */
{
	static char *flds[4] = {
	    "Sky dome", "Alt/az sky", "Solar system"
	};
	static int fn;	/* begin with 0, then remember for next time */
	int didone = 0;

	while (1) {
	    int nf;
	    flds[3] = trails ? "Leave trails" : "No trails";
	    if ((nf = popup (flds, fn, 4)) < 0)
		break;
	    fn = nf;
	    switch (nf) {
	    case 0: watch_sky (DOMESKY, np, tminc, wbodies); didone = 1; break;
	    case 1: watch_sky (ALTAZSKY, np, tminc, wbodies); didone = 1; break;
	    case 2: watch_solarsystem (np, tminc, wbodies); didone = 1; break;
	    case 3: trails ^= 1; break;
	    }
	}

	if (didone)
	    redraw_screen(2);
}

/* full alt/az or dome sky view (like the popular astro mags).
 * alt/az: north is at left and right of screen, south at center.
 *   0 elevation is at bottom of screen, zenith at the top.
 * dome: east is left, north is up.
 */
static
watch_sky (style, np, tminc, wbodies)
int style;	/* DOMESKY or ALTAZSKY */
Now *np;	/* time now and on each step */
double tminc;	/* hrs to increment time by each step */
int wbodies;	/* each bit is !=0 if want */
{
	static char east[] = "East";
	static char west[] = "West";
	static char north[] = "North";
	static char south[] = "South";
	double tminc0 = tminc;	/* remember the original */
	/* two draw buffers so we can leave old up while calc new then
	 * erase and draw in one quick operation. always calc new in newp
	 * buffer and erase previous from lastp. buffers alternate roles.
	 */
	LastDraw ld0[NOBJ], ld1[NOBJ], *lp, *lastp = ld0, *newp = ld1;
	int nlast = 0, nnew;
	int once = 1;
	double lmjd, tmp;
	Sky s;
	int p;

	/* clear screen and put up the permanent labels */
	c_erase();
	if (style == DOMESKY) {
	    double a;
	    for (a = 0.0; a < 2*PI; a += PI/8)
		f_char (fr2r(.5-sin(a)/2.),
		    fc2c(.5+cos(a)/2./ASPECT) + ((a>PI/2 && a<3*PI/2) ? -1 : 1),
		    '*');
	    f_string (fr2r(.5), fc2c(.5-.5/ASPECT)-7, "East");
	    f_string (fr2r(1.), fc2c(.5)-2, south);
	    f_string (fr2r(.5), fc2c(.5+.5/ASPECT)+4, "West");
	    f_string (2, NC/2-2, north);
	} else {
	    f_string (NR, 1, north);
	    f_string (NR, NC/4, east);
	    f_string (NR, NC/2, south);
	    f_string (NR, 3*NC/4, west);
	    f_string (NR, NC-5, north);   /* -1 more to avoid scrolling */
	    f_string (2, NC/2-3, "Zenith");
	}
	f_string (2, 1, tznm);
	f_string (3, 1, "LST");

	while (1) {
	    if (once)
		print_updating();

	    /* calculate desired stuff into newp[] */
	    nnew = 0;
	    for (p = nxtbody(-1); p != -1; p = nxtbody(p))
		if (wbodies & (1<<p)) {
		    (void) body_cir (p, SKYACC, np, &s);
		    if (s.s_alt > 0.0) {
			LastDraw *lnp = newp + nnew;
			if (style == DOMESKY) {
			    tmp = 0.5 - s.s_alt/PI;
			    lnp->l_fr = 0.5 - tmp*cos(s.s_az);
			    lnp->l_fc = 0.5 - tmp*sin(s.s_az)/ASPECT;
			} else {
			    lnp->l_fr = 1.0 - s.s_alt/(PI/2);
			    lnp->l_fc = s.s_az/(2*PI);
			}
			lnp->l_tag = body_tags[p];
			nnew++;
		    }
		}
	    set_screencoords (newp, nnew);

	    /* unless we want trails,
	     * erase any previous tags (in same order as written) from lastp[].
	     */
	    if (!trails)
		for (lp = lastp; --nlast >= 0; lp++)
		    f_char (lp->l_r, lp->l_c, ' ');

	    /* print LOCAL time and date we will be using */
	    lmjd = mjd - tz/24.0;
	    f_time (2, 5, mjd_hr(lmjd));
	    f_date (2, 14, mjd_day(lmjd));
	    now_lst (np, &tmp);
	    f_time (3, 5, tmp);

	    /* now draw new stuff from newp[] and park the cursor */
	    for (lp = newp; lp < newp + nnew; lp++)
		f_char (lp->l_r, lp->l_c, lp->l_tag);
	    c_pos (PARK_ROW, PARK_COL);
	    fflush (stdout);

	    /* swap new and last roles and save new count */
	    if (newp == ld0)
		newp = ld1, lastp = ld0;
	    else
		newp = ld0, lastp = ld1;
	    nlast = nnew;

	    if (!once)
		slp_sync();

	    if (once || (chk_char()==0 && read_char()!=0)) {
		if (readwcmd (tminc0, &tminc, &once) < 0)
		    break;
	    }

	    /* advance time */
	    inc_mjd (np, tminc);
	}
}

/* solar system view, "down from the top", first point of aries to the right.
 * always include earth.
 */
static
watch_solarsystem (np, tminc, wbodies)
Now *np;	/* time now and on each step */
double tminc;	/* hrs to increment time by each step */
int wbodies;
{
	/* max au of each planet from sun; in astro.h #defines order */
	static double auscale[] = {.38, .75, 1.7, 5.2, 11., 20., 31., 50.};
	double tminc0 = tminc;	/* remember the original */
	/* two draw buffers so we can leave old up while calc new then
	 * erase and draw in one quick operation. always calc new in newp
	 * buffer and erase previous from lastp. buffers alternate roles.
	 */
	LastDraw ld0[2*NOBJ], ld1[2*NOBJ], *lp, *lastp = ld0, *newp = ld1;
	int nlast = 0, nnew;
	int once = 1;
	double lmjd;
	double scale;
	Sky s;
	int p;

	/* set screen scale: largest au we will have to plot.
	 * never make it less than 1 au (with fudge) since we always show earth.
	 */
	scale = 1.1;
	for (p = MARS; p <= PLUTO; p++)
	    if ((wbodies & (1<<p)) && auscale[p] > scale)
		scale = auscale[p];

	/* clear screen and put up the permanent labels */
	c_erase();
	f_string (2, 1, tznm);

	while (1) {
	    if (once)
		print_updating();

	    /* calculate desired stuff into newp[].
	     * fake a sun at center and add earth first.
	     * (we get earth's loc when ask for sun)
	     */
	    nnew = 0;
	    set_ss (newp+nnew, 0.0, 0.0, 0.0, 'S');
	    nnew += 2;
	    (void) body_cir (SUN, SSACC, np, &s);
	    set_ss (newp+nnew, s.s_edist/scale, s.s_hlong, 0.0, 'E');
	    nnew += 2;
	    for (p = MERCURY; p <= PLUTO; p++)
		if (p != MOON && (wbodies & (1<<p))) {
		    (void) body_cir (p, SSACC, np, &s);
		    set_ss (newp+nnew, s.s_sdist/scale, s.s_hlong, s.s_hlat,
							    body_tags[p]);
		    nnew += 2;
		}
	    for (p = OBJX; p != -1; p = (p == OBJX) ? OBJY : -1)
		if (wbodies & (1<<p)) {
		    (void) body_cir (p, SSACC, np, &s);
		    if (s.s_hlong != NOHELIO && s.s_sdist <= scale) {
			set_ss (newp+nnew, s.s_sdist/scale, s.s_hlong, s.s_hlat,
								body_tags[p]);
			nnew += 2;
		    }
		}

	    set_screencoords (newp, nnew);

	    /* unless we want trails,
	     * erase any previous tags (in same order as written) from lastp[].
	     */
	    if (!trails)
		for (lp = lastp; --nlast >= 0; lp++)
		    safe_f_char (lp->l_r, lp->l_c, ' ');

	    /* print LOCAL time and date we will be using */
	    lmjd = mjd - tz/24.0;
	    f_time (2, 5, mjd_hr(lmjd));
	    f_date (2, 14, mjd_day(lmjd));

	    /* now draw new stuff from newp[] and park the cursor */
	    for (lp = newp; lp < newp + nnew; lp++)
		safe_f_char (lp->l_r, lp->l_c, lp->l_tag);
	    c_pos (PARK_ROW, PARK_COL);
	    fflush (stdout);

	    /* swap new and last roles and save new count */
	    if (newp == ld0)
		newp = ld1, lastp = ld0;
	    else
		newp = ld0, lastp = ld1;
	    nlast = nnew;

	    if (!once)
		slp_sync();

	    if (once || (chk_char()==0 && read_char()!=0)) {
		if (readwcmd (tminc0, &tminc, &once) < 0)
		    break;
	    }

	    /* advance time */
	    inc_mjd (np, tminc);
	}
}

/* fill in two LastDraw solar system entries,
 * one for the x/y display, one for the z.
 */
static
set_ss (lp, dist, lg, lt, tag)
LastDraw *lp;
double dist, lg, lt;	/* scaled heliocentric distance, longitude and lat */
char tag;
{
	lp->l_fr = 0.5 - dist*sin(lg)*0.5;
	lp->l_fc = 0.5 + dist*cos(lg)*0.5/ASPECT;
	lp->l_tag = tag;
	lp++;
	/* row is to show course helio altitude but since we resolve collisions
	 * by adjusting columns we can get more detail by smaller variations
	 * within one column.
	 */
	lp->l_fr = 0.5 - dist*sin(lt)*0.5;
	lp->l_fc = c2fc(SSZCOL) + (1 - lp->l_fr)/NC;
	lp->l_tag = tag;
}

/* given a list of LastDraw structs with their l_{fr,fc} filled in,
 * fill in their l_{r,c}.
 * TODO: better collision avoidance.
 */
static
set_screencoords (lp, np)
LastDraw lp[];
int np;
{
	LastDraw *lpi;	/* the current basis for comparison */
	LastDraw *lpj;	/* the sweep over other existing cells */
	int i;		/* index of the current basis cell, lpi */
	int j;		/* index of sweep cell, lpj */
	int n;		/* total cells placed so far (ie, # to check) */

	/* idea is to place each new item onto the screen.
	 * after each placement, look for collisions.
	 * if find a colliding pair, move the one with the greater l_fc to
	 * the right one cell, then rescan for more collisions.
	 * this will yield a result that is sorted by columns by l_fc.
	 * TODO: don't just move to the right, try up too for true 2d adjusts.
	 */
	for (n = 0; n < np; n++) {
	    lpi = lp + n;
	    i = n;
	    lpi->l_r = fr2r(lpi->l_fr);
	    lpi->l_c = fc2c(lpi->l_fc);
	  chk:
	    for (j = 0; j < n; j++) {
		lpj = lp + j;
		if (i!=j && lpi->l_r == lpj->l_r && lpi->l_c == lpj->l_c) {
		    if (lpj->l_fc > lpi->l_fc) {
			/* move lpj and use it as basis for checks now */
			lpi = lpj;
			i = j;
		    }
		    if (++lpi->l_c > NC)
			lpi->l_c = 1;
		    goto chk;
		}
	    }
	}
}

/* since the solar system scaling is only approximate, and doesn't include
 * object x/y at all, characters might get mapped off screen. this funtion
 * guards against drawing chars off screen. it also moves a char being drawn
 * on the lower right corner of the screem left one to avoid scrolling.
 */
static
safe_f_char (r, c, tag)
int r, c;
char tag;
{
	if (r >= 1 && r <= NR && c >= 1 && c <= NC) {
	    if (r == NR && c == NC)
		c -= 1;
	    f_char (r, c, tag);
	}
}

/* see what the op wants to do now and update prompt/times accordingly.
 * return -1 if we are finished, else 0.
 */
static int
readwcmd (tminc0, tminc, once)
double tminc0;
double *tminc;
int *once;
{
	f_prompt (qprompt);

	switch (read_char()) {
	case END: 		/* back to table */
	    return (-1);
	case '\r': case ' ':	/* one StpSz step */
	    *tminc = tminc0;
	    *once = 1;
	    break;
	case 'h':		/* one 1-hour step */
	    *tminc = 1.0;
	    *once = 1;
	    break;
	case 'd':		/* one 24-hr step */
	    *tminc = 24.0;
	    *once = 1;
	    break;
	case 'w':		/* 7 day step */
	    *tminc = 7*24.0;
	    *once = 1;
	    break;
	default:		/* free-run */
	    *once = 0;
	    f_prompt (frprompt);
	}
	return (0);
}
