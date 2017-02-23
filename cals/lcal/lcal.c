static char VERSION_STRING[] = "@(#)lcal v2.0.0 - generate full-year Postscript lunar (moon phases) calendars";
/* ---------------------------------------------------------------------------

   lcal.c
   
   Notes:

      This file contains routines to generate a PostScript file to print a
      lunar (moon phase) calendar for any year.

      For information on this application, see the 'ReadMe.txt' file.

      Note: The 'man' page for 'lcal' is the most up-to-date source of authors
      and contributors to 'lcal'.
     
   Revision history:

	2.0.0
		B.Marr		2006-07-20
		
		Reformatted comments and code to match my standards.
		
		B.Marr		2006-06-24
		
		Major overhaul!
		
		Update version number in version string.
		
		Put PostScript code inline, with conditional checks for
		'portrait' or 'landscape' orientation.  This greatly
		simplifies the build process and eliminates several files.
		For more detail, see the large block comment in the
		'write_psfile()' routine, below.
		
		Support building in the MS-DOS and in the DOS+DJGPP
		environments.  Eliminate Vax/VMS and Amiga support.
		
		Fix a long-standing bug whereby the help text for several of
		the options was never being displayed when 'lcal' was invoked
		with the '-h' option.
		
		Add new format: single-page, odd-days-only ('-O').  This
		format allows the display of a full year without using
		compression, so the moon graphics are the same (full) size as
		on the normal 2-page calendar.  This is achieved by only
		displaying odd days of the month.
		
		Rename 'rotate' variable to 'page_orientation' for clarity.
		
		Remove somewhat-obsolete list of options from the comments at
		the start of this file.
		
		Get rid of all the '#ifdef PROTOS' checks.  All 3 build
		environments work fine without this old compatibility check.
		
		Remove definition of 'PI' and use standard 'M_PI' from
		'math.h'.  This prevents a warning under the DOS+DJGPP build
		environment.
		
		Changed a variable from 'long' to 'time_t' to prevent warnings
		under the DOS+DJGPP build environment.
		
		Provide comment blocks to identify the major sections and each
		routine.
		
		Re-indent all the code to be consistent in style.
		
		Remove 'Ctl-L' (page-feed) characters from 'lcal' source code.
		
		Move some tables that were defined in C header files into this
		C source file.
		
		Use a format for the timestamp in the PostScript comment
		output which is more thorough and less ambiguous.
		
		Add the 'pcal'/'lcal' website to the PostScript comment header
		block.
		
		Eliminate unneeded macros for 'printf()' and 'fprintf()'.

*/

/* ---------------------------------------------------------------------------

   Header Files

*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <time.h>

/* Some headers needed for 'user account/real name' (via 'getuid()' etc) in
   PostScript comments */
#if defined (BUILD_ENV_UNIX) || defined (BUILD_ENV_DJGPP)
#include <pwd.h>
#include <unistd.h>
#endif

/*
 * Lcal-specific definitions:
 */
#include "lcaldefs.h"

/* ---------------------------------------------------------------------------

   Type, Struct, & Enum Declarations

   */

/* ---------------------------------------------------------------------------

   Constant Declarations

   */

/* ---------------------------------------------------------------------------

   Macro Definitions

   */

/* split string into two substrings at separator 'c' */
#define SPLIT(str, str1, str2, c) \
   do { char *p; \
   str2 = ((p = strrchr(str, c))) ? (*p = '\0', ++p) : ""; \
   if ((p = strchr(str1 = str, c))) *p = '\0'; } while (0)

/* ---------------------------------------------------------------------------

   Data Declarations (including externals)

   */

char *months[12] = {
   "January", "February", "March", "April", "May", "June",
   "July", "August", "September", "October", "November", "December"
};

char *days[7] = {
   "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

static int init_year;

char *words[MAXWORD];   /* maximum number of words per date file line */
char lbuf[LINSIZ];   /* date file source line buffer */
char progname[STRSIZ];   /* program name (for error messages) */
char version[20];   /* program version (for info messages) */

/* lengths and offsets of months in common year */
char month_len[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
short month_off[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

/*
 * Flag usage information - not strictly language-dependent, but here anyway
 * (N.B.: all flags must be represented by an entry in this table!)
 *
 * The table below supplies the following information about each flag:
 *
 *   - Its name (cf. symbolic definitions above)
 *
 *   - Whether or not it can take an (optional) argument
 *
 */
flag_usage_str_typ flag_tbl[] = {
   /* flag name, arg? */
   { F_DAY_FONT, TRUE },
   { F_TITLE_FONT, TRUE },
   
   { F_OUT_FILE, TRUE },
   
   { F_LANDSCAPE, FALSE },
   { F_PORTRAIT, FALSE },
   
   { F_SHADING, TRUE },
   
   { F_INVERT, FALSE },
   
   { F_HELP, FALSE },
   { F_USAGE, FALSE },
   { F_VERSION, FALSE },
   
   { F_WEEKDAYS, FALSE },
   
   { F_TIMEZONE, TRUE },
   
   { F_COMPR_1PAGE, FALSE },
   
   { F_ODD_DAYS_1PAGE, FALSE },
   
   { F_XOFFSET, TRUE },
   { F_YOFFSET, TRUE },
   
   { '-', FALSE },
   { '\0', FALSE },
};

/*
 * Message strings to be printed by usage() - translate as necessary
 */

flag_msg_str_typ flag_msg[] = {

/* flag name, metasyntax, description, default */

	{ F_DAY_FONT,	W_FONT,		"specify alternate font for weekday names",		DATEFONT },
	{ F_TITLE_FONT,	W_FONT,		"specify alternate font for year/months/dates",		TITLEFONT },
	{ END_GROUP },

	{ F_OUT_FILE,	W_FILE,		"specify alternate output file",			"stdout" },
	{ END_GROUP },

	{ F_LANDSCAPE,	NULL,		"generate landscape-style calendar",			NULL },
	{ F_PORTRAIT,	NULL,		"generate portrait-style calendar",			NULL },
#if ROTATE == LANDSCAPE
	{ GROUP_DEFAULT,									"landscape" },
#else	
	{ GROUP_DEFAULT,									"portrait" },
#endif
	{ END_GROUP },

	{ F_SHADING,	W_SHADING,	"specify back/foreground colors (grayscale or r:g:b)",	NULL },
	{ ' ',		NULL,		" ",							SHADING },
	{ END_GROUP },

	{ F_INVERT,	NULL,		"invert colors (white moons on black background)",	NULL },
	{ END_GROUP },

	{ F_TIMEZONE,	W_VALUE,	"specify alternate time zone",				TIMEZONE },
	{ END_GROUP },

	{ F_COMPR_1PAGE,	NULL,	"compress output to fit on single page",		NULL },
	{ END_GROUP },

	{ F_ODD_DAYS_1PAGE,	NULL,	"print odd days only (fits on single page)",		NULL },
	{ END_GROUP },

	{ F_XOFFSET,	W_VAL2,		"specify X offset (pg1/pg2)",				NULL },
	{ GROUP_DEFAULT,									X_OFFSET },
	{ F_YOFFSET,	W_VAL2,		"specify Y offset (pg1/pg2)",				NULL },
	{ GROUP_DEFAULT,									Y_OFFSET },
	{ END_GROUP },

	{ F_WEEKDAYS,	NULL,		"print day-of-week names inside moons",	NULL },
	{ END_GROUP },

	{ F_HELP,	NULL,		"print this help message",				NULL },
	{ F_USAGE,	NULL,		"print usage message",					NULL },
	{ F_VERSION,	NULL,		"print version stamp",					NULL },
	{ END_GROUP },			/* must precede END_LIST */

	{ END_LIST }   /* must be last */
};


/* Numeric parameter descriptions and text */

param_msg_str_typ param_msg[] = {
   { "YY", "generate calendar for year YY (CCYY if YY < 100)" },
   { "(default)", "generate calendar for current year" },
   { NULL, NULL }   /* must be last */
};

/*
 * Default values for command-line options:
 */

int rotate = ROTATE;   /* -l, -p */

char dayfont[STRSIZ] = DATEFONT;   /* -d, -t, -n */
char titlefont[STRSIZ] = TITLEFONT;

char outfile[STRSIZ] = OUTFILE;   /* -o */

char x_offset[STRSIZ] = X_OFFSET;   /* -X, -Y */
char y_offset[STRSIZ] = Y_OFFSET;

int draw_day_of_week_inside_moon = WEEKDAYS;   /* -W */

char shading[STRSIZ] = SHADING;   /* -i, -s */

char time_zone[STRSIZ] = TIMEZONE;   /* -z */

int compressed_singlepage = FALSE;   /* -S */
int odd_days_singlepage = FALSE;   /* -O */

/* ---------------------------------------------------------------------------

   External Routine References & Function Prototypes

   */

/* ---------------------------------------------------------------------------

   get_flag

   Notes:

      Look up flag in 'flag_tbl'.  Return pointer to its entry (or NULL if not
      found).

*/
flag_usage_str_typ * get_flag (char flag)
{
   flag_usage_str_typ *pflag;
   
   for (pflag = flag_tbl; pflag->flag; pflag++) {
      if (flag == pflag->flag) return pflag;
   }
   return flag ? NULL : pflag;   /* '\0' is a valid flag */
}

/* ---------------------------------------------------------------------------

   usage

   Notes:

      This routine prints a message explaining correct usage of the
      command-line arguments and flags.

      The 1st parameter is the destination of the usage message.

      The 2nd parameter determines whether the complete usage message is
      displayed.

*/
void usage (FILE *fp, int fullmsg)
{
   flag_msg_str_typ *pflag;
   param_msg_str_typ *ppar;
   char buf[30], *p, flag, *meta;
   int nchars, first, i, indent, n;
   
   sprintf(buf, "Usage: %s", progname);
   nchars = indent = strlen(buf);
   first = TRUE;
   meta = p = NULL;
   fprintf(fp, "\n%s", buf);
   
   /* loop to print command-line syntax message (by group of flags) */

   for (pflag = flag_msg; (flag = pflag->flag) != '\0'; pflag++) {
      if (flag == '\n') {   /* end of group? */
         if (p) *p = '\0';
         if (meta) {   /* append metavariable name */
            strcat(buf, " ");
            strcat(buf, meta);
         }
         strcat(buf, "]");
         n = strlen(buf);
         if (nchars + n > SCREENWIDTH) {   /* does it fit on line? */
            fprintf(fp, "\n");   /* no - start new one */
            for (nchars = 0; nchars < indent; nchars++) fprintf(fp, " ");
         }
         fprintf(fp, "%s", buf);
         nchars += n;
         first = TRUE;
      }
      else if (flag != ' ') {   /* accumulate flags for group */
         if (first) {
            sprintf(buf, " [");
            p = buf + strlen(buf);
         }
         else *p++ = '|';
         *p++ = '-';
         *p++ = flag;
         meta = pflag->meta;   /* save metavariable name */
         first = FALSE;
      }
   }

   /* loop to print selected numeric parameter descriptions */

   for (i = 0; i < PARAM_MSGS; i++) {
      sprintf(buf, " [%s]%s", param_msg[i].desc, i < PARAM_MSGS - 1 ? " |" : "");
      n = strlen(buf);
      if (nchars + n > SCREENWIDTH) {   /* does it fit on line? */
         fprintf(fp, "\n");   /* no - start new one */
         for (nchars = 0; nchars < indent; nchars++) fprintf(fp, " ");
      }
      fprintf(fp, "%s", buf);
      nchars += n;
   }

   fprintf(fp, "\n\n");
   if (! fullmsg) {
      fprintf(fp, "Enter \"%s -%c\" for full description of parameters.\n", progname, F_HELP);
      return;
   }
   
   /* loop to print the full flag descriptions */

   for (pflag = flag_msg; (flag = pflag->flag) != '\0'; pflag++) {
      if (flag == '\n') {   /* newline?  print and quit */
         fprintf(fp, "\n");
         continue;
      }
      p = buf;   /* copy flag and metavariable to buffer */
      if (flag != ' ') *p++ = '-';

      *p++ = flag;

      *p = '\0';
      if (pflag->meta) sprintf(p, " %s", pflag->meta);
      fprintf(fp, "\t%-16.16s", buf);
      if (pflag->text) fprintf(fp, "%s", pflag->text);

      /* print default value if specified */
      if (pflag->def) fprintf(fp, " (default: %s)", pflag->def[0] ? pflag->def : "\"\"" );
      fprintf(fp, "\n");

      
   }
   
   /* now print the information about the numeric parameters */
   
   for (ppar = param_msg; ppar->desc; ppar++) {
      fprintf(fp, "\t%-16.16s%s\n", ppar->desc, ppar->text);
   }
   
   fprintf(fp, "\n");
   return;
}

/* ---------------------------------------------------------------------------

   is_bg_darker

   Notes:

      This routine uses the shading string to determine if the background is
      darker than the foreground (requiring that light/dark portions of moons
      be reversed by shifting the phase by 1/2 cycle).

      It returns 'TRUE' if the background is darker than the foreground and
      'FALSE' otehrwise.

*/
int is_bg_darker (void)
{
   static char tmp[STRSIZ];
   char *p, *p1, *p2;
   double bval, fval;
   int n;
   
   bval = fval = 0.0;
   
   strcpy(tmp, shading);
   *(p = strchr(tmp, '/')) = '\0';
   
   /* get average of 1 - 3 floating-point background values */
   for (n = 1, p1 = tmp; n <= 3; n++, p1 = p2 + 1) {
      bval += atof(p1);
      if ((p2 = strchr(p1, RGB_CHAR)) == NULL) break;
   }
   bval /= (n > 3 ? 3 : n);

   /* get average of 1 - 3 floating-point foreground values */
   for (n = 1, p1 = ++p; n <= 3; n++, p1 = p2 + 1) {
      fval += atof(p1);
      if ((p2 = strchr(p1, RGB_CHAR)) == NULL) break;
   }
   fval /= (n > 3 ? 3 : n);
   
   /* return 'TRUE' if background is darker (lower value; 0:0:0 is white,
      1:1:1 is black) than foreground */
   return bval < fval;
}

/* ---------------------------------------------------------------------------

   set_rgb

   Notes:

      This routine converts "<r>:<g>:<b>" to "r g b setrgbcolor" or "<gray>"
      to "gray setgray". It returns a pointer to a static buffer containing
      the converted string.

      This utility routine was borrowed from 'pcal'.

*/
char * set_rgb (char *s)
{
   static char buf[STRSIZ];
   char *p1, *p2;
   double val[3];
   int n;
   
   val[0] = val[1] = val[2] = 0;   /* defaults */
   
   /* extract 1 - 3 floating-point values from string */
   for (n = 1, p1 = s; n <= 3; n++, p1 = p2 + 1) {
      val[n-1] = atof(p1);
      if ((p2 = strchr(p1, RGB_CHAR)) == NULL) break;
   }
   
   /* single value is gray scale; assume anything else is [r g b] */
   if (n > 1) {
      sprintf(buf, "%.3f %.3f %.3f setrgbcolor", val[0], val[1], val[2]);
   }
   else sprintf(buf, "%.3f setgray", val[0]);
   
   return buf; 
}

/* ---------------------------------------------------------------------------

   define_shading

   Notes:

      This utility routine was borrowed from 'pcal'.

*/
void define_shading (register char *orig_shading,
                     register char *new_shading,
                     register char *dflt_shading)
{
   char *orig_bg, *new_bg, *dflt_bg, *orig_fg, *new_fg, *dflt_fg;
   char tmp1[STRSIZ], tmp2[STRSIZ], tmp3[STRSIZ];
   double v;
   
   /* use default date/fill if new _shading is null */
   if (new_shading == NULL || new_shading[0] == '\0') {
      strcpy(orig_shading, dflt_shading);
      return;
   }

   /* split old, new, and default shadings into background/foreground
    * components
    */
   SPLIT(orig_shading, orig_bg, orig_fg, '/');
   
   new_shading = strcpy(tmp1, new_shading);
   SPLIT(new_shading, new_bg, new_fg, '/');
   
   dflt_shading = strcpy(tmp2, dflt_shading);
   SPLIT(dflt_shading, dflt_bg, dflt_fg, '/');
   
   /* replace invalid fields from new shading with default values */
   if (new_fg[0] && strchr(new_fg, RGB_CHAR) == NULL && 
       ((v = atof(new_fg)) <= 0.0 || v > 1.0)) {
      new_fg = dflt_fg;
   }
   if (new_bg[0] && strchr(new_bg, RGB_CHAR) == NULL &&
       ((v = atof(new_bg)) <= 0.0 || v > 1.0)) {
      new_bg = dflt_bg;
   }

   /* replace fields of old shading with specified fields from new */
   
   strcpy(tmp3, new_bg[0] ? new_bg : orig_bg);
   strcat(tmp3, "/");
   strcat(tmp3, new_fg[0] ? new_fg : orig_fg);
   
   strcpy(orig_shading, tmp3);

   return;
}

/* ---------------------------------------------------------------------------

   calc_weekday

   Notes:

      This routine returns the weekday (0-6) of mm/dd/yy (mm: 1-12).

      This utility routine was borrowed from 'pcal'.

*/
int calc_weekday (int mm, int dd, int yy)
{
   return (yy + (yy-1)/4 - (yy-1)/100 + (yy-1)/400 + OFFSET_OF(mm, yy) + (dd-1)) % 7;
}

/* ---------------------------------------------------------------------------

   loadwords

   Notes:

      This routine tokenizes the buffer 'buf' into array 'words' and returns
      word count.

      It differs from old 'loadwords()' in that it handles quoted (" or ')
      strings and removes escaped quotes.

      This utility routine was borrowed from 'pcal'.

*/
int loadwords (char **words, char *buf)
{
   register char *ptok;
   char *delim, **ap, *p1, *p2, c;
   int nwords;
   
   for (ptok = buf, ap = words; TRUE; ap++) {
      
      ptok += strspn(ptok, WHITESPACE); /* find next token */
      
      if (! *ptok) {   /* end of buf? */
         *ap = NULL;   /* add null ptr at end */
         nwords = ap - words;   /* number of non-null ptrs */
         break;   /* exit loop */
      }
      
      delim = *ptok == '"'  ? "\"" :   /* set closing delimiter */
         *ptok == '\'' ? "'"  :
         WHITESPACE;
      
      /* split flag followed by quoted string into two words */
      if (*ptok == '-' && isalpha(ptok[1]) && ((c = ptok[2]) == '"' || c == '\'')) {
         delim = c == '"' ? "\"" : "'";
         *ap++ = ptok;
         ptok[2] = '\0';   /* terminate first token */
         ptok += 3;   /* start second token */
      }
      else if (*ptok == *delim) ptok++;   /* skip opening quote */
      
      *ap = ptok;   /* save token ptr */

      /* find first unescaped string delimiter - handle special case where
       * preceding backslash is itself escaped
       */
      do {
         ptok += strcspn(ptok, delim);
         if ((c = ptok[-1]) == '\\') {
            if (ptok[-2] == '\\') break;   /* stop on \\" or \\' */
            else ptok++;
         }
      } while (c == '\\');
      
      if (*ptok) { /* terminate token */
         *ptok++ = '\0';
      }
   }
   
   /* now reprocess the word list, removing remaining escapes */
   for (ap = words; *ap; ap++) {
      for (p1 = p2 = *ap; (c = *p2 = *p1++); p2++) {
         if (c == '\\') *p2 = *p1++;
      }
   }
   
   return nwords;   /* return word count */
}

/* ---------------------------------------------------------------------------

   The following suite of routines ('julday()', 'kepler()', and
   'calc_phase()') are used to calculate the phase of the moon for a given
   month, day, year.  They compute the phase of the moon for noon (UT) on the
   day requested, the start of the Julian day.
 
   Revision history:
 
 	1.2	AWR	01/25/00	fix calculation of default year
 
 	1.0	AWR	01/26/92	from Pcal v4.4

   -------------------------------------------------------

   Routines to accurately calculate the phase of the moon
 
   Originally adapted from "moontool.c" by John Walker, Release 2.0.
 
       This routine (calc_phase) and its support routines were adapted from
       phase.c (v 1.2 88/08/26 22:29:42 jef) in the program "xphoon" (v 1.9
       88/08/26 22:29:47 jef) by Jef Poskanzer and Craig Leres.  The necessary
       notice follows...
 
       Copyright (C) 1988 by Jef Poskanzer and Craig Leres.
 
       Permission to use, copy, modify, and distribute this software and its
       documentation for any purpose and without fee is hereby granted,
       provided that the above copyright notice appear in all copies and that
       both that copyright notice and this permission notice appear in
       supporting documentation.  This software is provided "as is" without
       express or implied warranty.
 
       These were added to "pcal" by RLD on 19-MAR-1991
 
*/

/*  Astronomical constants. */

#define epoch   2444238.5   /* 1980 January 0.0 */

/*  Constants defining the Sun's apparent orbit. */

#define elonge   278.833540   /* ecliptic longitude of the Sun at epoch 1980.0 */
#define elongp   282.596403   /* ecliptic longitude of the Sun at perigee */
#define eccent   0.016718   /* eccentricity of Earth's orbit */

/*  Elements of the Moon's orbit, epoch 1980.0. */

#define mmlong   64.975464   /* moon's mean lonigitude at the epoch */
#define mmlongp   349.383063   /* mean longitude of the perigee at the
                                        epoch */
#define mlnode   151.950429   /* mean longitude of the node at the epoch */
#define synmonth   29.53058868   /* synodic month (new Moon to new Moon) */


/*  Handy mathematical functions. */

#define sgn(x) (((x) < 0) ? -1 : ((x) > 0 ? 1 : 0))   /* extract sign */
#ifndef abs
#define abs(x) ((x) < 0 ? (-(x)) : (x))   /* absolute val */
#endif
#define fixangle(a) ((a) - 360.0 * (floor((a) / 360.0)))  /* fix angle */
#define torad(d) ((d) * (M_PI / 180.0))   /* deg->rad */
#define todeg(d) ((d) * (180.0 / M_PI))   /* rad->deg */
#define dsin(x) (sin(torad((x))))   /* sin from deg */
#define dcos(x) (cos(torad((x))))   /* cos from deg */
#define FNITG(x) (sgn (x) * floor (abs (x)))

/* ---------------------------------------------------------------------------

   julday

   Notes:

      This routine calculates the julian date from input month, day, year

      N.B. - The Julian date is computed for noon UT.

      This utility routine was borrowed from 'pcal'.

      Adopted from Peter Duffett-Smith's book `Astronomy With Your Personal
      Computer' by Rick Dyson 18-MAR-1991

*/
static double julday (int month, int day, int year)
{
   int mn1, yr1;
   double a, b, c, d, djd;
   
   mn1 = month;
   yr1 = year;
   if ( yr1 < 0 ) yr1 = yr1 + 1;
   if ( month < 3 ) {
      mn1 = month + 12;
      yr1 = yr1 - 1;
   }
   if (( year < 1582 ) ||
       ( year == 1582  && month < 10 ) ||
       ( year == 1582  && month == 10 && day < 15.0 )) {
      b = 0;
   }
   else {
      a = floor (yr1 / 100.0);
      b = 2 - a + floor (a / 4);
   }
   if ( yr1 >= 0 ) c = floor (365.25 * yr1) - 694025;
   else c = FNITG ((365.25 * yr1) - 0.75) - 694025;

   d = floor (30.6001 * (mn1 + 1));
   djd = b + c + d + day + 2415020.0;
   return djd;
}

/* ---------------------------------------------------------------------------

   kepler

   Notes:

      This routine solves the equation of Kepler.

      This utility routine was borrowed from 'pcal'.

*/
static double kepler (double m, double ecc)
{
   double e, delta;
#define EPSILON 1E-6
   
   e = m = torad(m);
   do {
      delta = e - ecc * sin(e) - m;
      e -= delta / (1 - ecc * cos(e));
   } while (abs(delta) > EPSILON);
   return e;
}

/* ---------------------------------------------------------------------------

   calc_phase

   Notes:

      This routine calculates the phase of moon as a fraction.

      This utility routine was borrowed from 'pcal'.

      The argument is the time for which the phase is requested, expressed as
      the month, day, and year.  It returns the phase of the moon (0.0 ->
      0.99) with the ordering as New Moon, First Quarter, Full Moon, and Last
      Quarter.
      
      Converted from the subroutine phase.c used by "xphoon.c" (see above
      disclaimer) into calc_phase() for use in "moonphas.c" by Rick Dyson
      18-MAR-1991
      
*/
double calc_phase (int month, int inday, int year)
{
   double Day, N, M, Ec, Lambdasun, ml, MM;
   double Ev, Ae, A3, MmP, mEc, A4, lP, V, lPP, MoonAge, pdate, moon_phase;
   static int first = TRUE;
   static double utc_offset_days;
   
   /* Get the UTC offset on the first pass. 

      The original code used to normalize the UTC offset to +/- 12 hours.  But
      it was bug-ridden and also failed to take into account that some parts
      of the world have offsets from UTC greater than 12 hours!  Therefore,
      beginning with v2.0.0, we don't attempt to normalize the user-specified
      UTC timezone offset at all.
   */
   if (first) {

      utc_offset_days = atof(time_zone) / 24.0;

      /* fprintf(stderr, "time_zone='%s'  utc_offset_days = %.5lf\n", time_zone, utc_offset_days); */

      first = FALSE;
   }

   /*  need to convert month, day, year into a Julian pdate */
   pdate = julday(month, inday, year) + utc_offset_days;
   
   /*  Calculation of the Sun's position. */

   Day = pdate - epoch;   /* date within epoch */
   N = fixangle((360 / 365.2422) * Day);   /* mean anomaly of the Sun */
   M = fixangle(N + elonge - elongp);      /* convert from perigee
                                              co-ordinates to epoch 1980.0 */
   Ec = kepler(M, eccent);   /* solve equation of Kepler */
   Ec = sqrt((1 + eccent) / (1 - eccent)) * tan(Ec / 2);
   Ec = 2 * todeg(atan(Ec));   /* true anomaly */
   Lambdasun = fixangle(Ec + elongp);   /* Sun's geocentric ecliptic longitude */

   /*  Calculation of the Moon's position. */
   
   /*  Moon's mean longitude. */
   ml = fixangle(13.1763966 * Day + mmlong);
   
   /*  Moon's mean anomaly. */
   MM = fixangle(ml - 0.1114041 * Day - mmlongp);
   
   /*  Moon's ascending node mean longitude. */
   /*  Not used -- commented out. */
   /* MN = fixangle(mlnode - 0.0529539 * Day); */
   
   /*  Evection. */
   Ev = 1.2739 * sin(torad(2 * (ml - Lambdasun) - MM));
   
   /*  Annual equation. */
   Ae = 0.1858 * sin(torad(M));
   
   /*  Correction term. */
   A3 = 0.37 * sin(torad(M));
   
   /*  Corrected anomaly. */
   MmP = MM + Ev - Ae - A3;
   
   /*  Correction for the equation of the centre. */
   mEc = 6.2886 * sin(torad(MmP));
   
   /*  Another correction term. */
   A4 = 0.214 * sin(torad(2 * MmP));
   
   /*  Corrected longitude. */
   lP = ml + Ev + mEc - Ae + A4;
   
   /*  Variation. */
   V = 0.6583 * sin(torad(2 * (lP - Lambdasun)));
   
   /*  True longitude. */
   lPP = lP + V;
   
   /*  Calculation of the phase of the Moon. */
   
   /*  Age of the Moon in degrees. */
   MoonAge = lPP - Lambdasun;

   moon_phase = fixangle(MoonAge) / 360.0;
   if (moon_phase < 0.0) moon_phase += 1.0;

   /* fprintf(stderr, "Moon phase on %04d-%02d-%02d: %.5lf\n", year, month, inday, moon_phase); */

   return (moon_phase);
}

/* ---------------------------------------------------------------------------

   write_psfile

   Notes:

      This routine writes the PostScript code to 'stdout'.

      The parameter is the year for which the calendar should be generated.

      The actual output of the PostScript code is straightforward.  This
      routine writes a PostScript header followed by declarations of all the
      PostScript variables affected by command-line flags and/or language
      dependencies.  It then generates the remaining PostScript routines, and
      finally prints the moon phase information for the year.

*/
void write_psfile (int year)
{
   int month, day, fudge1, fudge2, invert_moons;
   double phase;
   char *off, *p, tmp[STRSIZ];
   static char *cond[2] = {"false", "true"};
   char time_str[50];
   time_t curr_tyme;

#if defined (BUILD_ENV_UNIX) || defined (BUILD_ENV_DJGPP)
   struct passwd *pw;
#endif

   /*
    * Write out PostScript prolog
    */
   
   /* comment block at top */
   
   printf("%%!%s\n", PS_RELEASE);   /* PostScript release */
   
   /* Get the current date/time so that we can write it into the output file
      as a timestamp...  */
   time(&curr_tyme);

   /* It seems that neither MS-DOS (Borland C) nor DOS+DJGPP support the '%P'
      (lowercase 'am'/'pm') specifier, so we'll use '%p' (uppercase 'AM'/'PM')
      instead. */
#if defined (BUILD_ENV_MSDOS) || defined (BUILD_ENV_DJGPP)
   strftime(time_str, sizeof(time_str), "%d %b %Y (%a) %I:%M:%S%p", localtime(&curr_tyme));
#else
   strftime(time_str, sizeof(time_str), "%d %b %Y (%a) %I:%M:%S%P", localtime(&curr_tyme));
#endif
   
   printf("%%%%CreationDate: %s\n", time_str);
   
   printf("%%%%Creator: Generated by %s %s (%s)\n", progname, version, LCAL_WEBSITE);

   /* Generate "For" and "Routing" comments if user name is known... */

#if defined (BUILD_ENV_UNIX) || defined (BUILD_ENV_DJGPP)
   if ((pw = getpwuid(getuid())) != NULL && strcmp(pw->pw_name, "nobody" /* anonymous account */) != 0) {
      printf("%%%%For: %s\n", pw->pw_name);
#ifdef BUILD_ENV_UNIX
      /* The 'pw->pw_gecos' element ('real' user name) is not available in
         MS-DOS or DOS+DJGPP build environments... */
      strcpy(tmp, pw->pw_gecos);
      if ((p = strchr(tmp, ',')) != NULL) *p = '\0';
      printf("%%%%Routing: %s\n", tmp);
#endif
   }
#endif

   /* Miscellaneous other identification */
   
   printf("%%%%Title: Lunar phase calendar for %d\n", init_year);
   printf("%%%%Pages: %d\n", (compressed_singlepage || odd_days_singlepage) ? 1 : 2);
   printf("%%%%PageOrder: Ascend\n");
   printf("%%%%Orientation: %s\n", rotate == LANDSCAPE ? "Landscape" : "Portrait");
   printf("%%%%BoundingBox: 0 0 612 792\n");
   printf("%%%%ProofMode: NotifyMe\n");
   printf("%%%%EndComments\n");
   
   /* advertisement for original inspiration */
   
   printf("%%\n");
   printf("%% Lcal was inspired by \"Moonlight 1996\", a 16\" x 36\" full-color (silver\n");
   printf("%% moons against a midnight blue background) lunar phase calendar marketed\n");
   printf("%% by Celestial Products, Inc., P.O. Box 801, Middleburg VA  22117.  Send\n");
   printf("%% for their catalog to see (and, hopefully, order) this as well as some\n");
   printf("%% even more amazing stuff - particularly \"21st Century Luna\", a lunar\n");
   printf("%% phase calendar for *every day* of the upcoming century.\n");
   printf("%%\n");
   printf("%% Or visit Celestial Products' site:\n");
   printf("%%\n");
   printf("%%   http://www.celestialproducts.com\n");
   printf("%%\n\n");

   /* font names and sizes */
   
   printf("/titlefont /%s def\n/dayfont /%s def\n", titlefont, dayfont);
   
   printf("/titlefontsize %d def\n", 
          odd_days_singlepage ? TITLEFONTSIZE_ODD_DAYS : TITLEFONTSIZE_NORMAL);

   printf("/datefontsize  %d def\n", compressed_singlepage ? DATEFONTSIZE_S : DATEFONTSIZE);
   printf("/monthfontsize %d def\n", compressed_singlepage ? MONTHFONTSIZE_S : MONTHFONTSIZE);
   printf("/weekdayfontsize     %d def\n", WKDFONTSIZE);
   printf("/sm_weekdayfontsize  %d def\n", compressed_singlepage ? SMWKDFONTSIZE_S : SMWKDFONTSIZE);

   /* month names */
   
   printf("/month_names [");
   for (month = JAN; month <= DEC; month++) {
      printf(" (%-3.3s)", months[month-JAN]);
   }
   printf(" ] def\n");
   
   /* day names - abbreviate if printing entire year on page */
   
   printf("/day_names [");
   for (day = SUN; day <= SAT; day++) {
      printf(" (%-2.2s)", days[day-SUN]);
   }
   printf(" ] def\n");
   
   /* weekday flag */

   printf("/inmoon_labels %s def\n", cond[draw_day_of_week_inside_moon]);
   
   /* fudge factors for X origin (landscape), Y origin (portrait) -
    * theoretically unnecessary, but useful if your printer isn't aligned
    * quite right and clips the edges
    */
   off = rotate == LANDSCAPE ? x_offset : y_offset;
   fudge1 = atoi(off);
   fudge2 = (p = strchr(off, '/')) ? atoi(++p) : fudge1;
   printf("/fudge1 %d def\n", compressed_singlepage ? 0 : fudge1);
   printf("/fudge2 %d def\n", compressed_singlepage ? 0 : fudge2);
   
   /* misc. constants */
   printf("/xsval %.1f def\n", compressed_singlepage ? HALF_SIZE : FULL_SIZE);
   printf("/ysval %.1f def\n", compressed_singlepage ? HALF_SIZE : FULL_SIZE);
   printf("/pagebreak %d def\n", compressed_singlepage ? PAGEBREAK_S : PAGEBREAK);
   
   /* background and foreground colors */
   
   strcpy(tmp, shading);
   *(p = strchr(tmp, '/')) = '\0';
   printf("/setbackground { %s } def\n", set_rgb(tmp));
   printf("/setforeground { %s } def\n", set_rgb(++p));
   
   /* disable duplex mode (if supported) */
   
   printf("statusdict (duplexmode) known {\n");
   printf("statusdict begin false setduplexmode end\n");
   printf("} if\n");
   
   /* PostScript boilerplate */

   /* 
    * Like the 'lcal' application itself, this entire block of PostScript code
    * was originally authored by Andrew Rogers.  It used to reside in 2 very
    * similar PostScript source files ('lcal_{p,l}.ps'), from which C header
    * files ('lcal_{p,l}.h') were created, using the same method
    * ('pcalinit.c', which generated a stand-alone executable) as used by the
    * 'pcal' application.
    * 
    * Beginning with 'lcal' version 2.0.0, a change was made (by Bill Marr) to
    * incorporate the PostScript code here, directly into the C source file.
    * 
    * This has the advantage of making the 'Makefile' much simpler.  It also
    * allows us to reduce the overall code needed to support both portrait and
    * landscape orientations, since it eliminates the duplicate PostScript
    * code, thereby allowing us to more easily see the exact difference
    * between the PostScript code needed to support those two page
    * orientations.
    * 
    * This change also puts out more-readable (properly-indented) PostScript,
    * unlike the old method which had no indentation whatsoever.
    * 
    * Lastly, this change, along with a breakup of the old '/calendar'
    * PostScript routine into the more-aptly named '/draw_page_1' and
    * '/draw_page_2' routines, allowed a proper enumeration of the PostScript
    * page numbers, thereby allowing PostScript viewer applications (like
    * Unix's 'gv' and similar) to finally show both pages when previewing and
    * allowing one to go back and forth between the 2 pages.
    * 
    * Several other tweaks to the PostScript code were performed during this
    * transition.  Mostly, some of the PostScript routines were commented to
    * assist future users of this application and/or code.
    * 
    */

   printf("\n");
   printf("/width 43 def\n");
   printf("/height 43 def\n");
   printf("/negwidth width neg def\n");
   printf("/negheight height neg def\n");
   printf("/halfwidth width 2 div def\n");
   printf("/halfheight height 2 div def\n");
   printf("/neghalfwidth halfwidth neg def\n");
   printf("/neghalfheight halfheight neg def\n");
   
   printf("/%s 612 def\n", 
          rotate == PORTRAIT ? "pagewidth" : "pageheight");

   printf("/%s 792 %s div dup 1584 gt { pop 1584 } if def\n", 
          rotate == PORTRAIT ? "pageheight" : "pagewidth",
          rotate == PORTRAIT ? "ysval" : "xsval");
   
   
   printf("/margin %s 12 mul sub 2 div def\n",
          rotate == PORTRAIT ? "pagewidth width" : "pageheight height");
   
   printf("/%s pagebreak ",
          rotate == PORTRAIT ? "topmargin pageheight height" : "leftmargin pagewidth width");
   /* Move the left margin to the left (for landscape) and the top margin up
      (for portrait) whenever we're doing odd-days-only, 1-page output... */
   printf("%s ",
          odd_days_singlepage ? "1.7 add" : "");
   printf("mul sub def\n");
   
   printf("/Xnext %s def\n",
          rotate == PORTRAIT ? "width" : "0");
   
   printf("/Ynext %s def\n",
          rotate == PORTRAIT ? "0" : "negheight");
   
   printf("/rval %s def\n",
          rotate == PORTRAIT ? "0" : "90");
   
   printf("/halfperiod 0.5 def\n");
   printf("/quartperiod 0.25 def\n");
   printf("/radius 15 def\n");
   printf("/rect radius 2 sqrt mul quartperiod div def\n");
   printf("\n");
   printf("/center {\n");
   printf("  /wid exch def\n");
   printf("  /str exch def\n");
   printf("  wid str stringwidth pop sub 2 div 0 rmoveto str\n");
   printf("} def\n");
   printf("\n");

   printf("%% \n");
   printf("%% This routine draws the year of the calendar as a 'title' of sorts.\n");
   printf("%% \n");
   printf("/drawtitle {\n");
   printf("  titlefont findfont titlefontsize scalefont setfont\n");
   printf("  /yearstring year 10 string cvs def\n");
   
   if (rotate == PORTRAIT) {
      printf("  margin neg 40 moveto\n");
      printf("  yearstring pagewidth center show\n");
   }
   else {
      /* The odd-days-only 1-page calendar in landscape orientation is a bit
         of a special case.  There's not enough room to display the 'title'
         where it normall goes, so we move it to the upper left corner
         instead... */
      if (odd_days_singlepage) {
         printf("  radius width 1.2 mul sub neghalfwidth titlefontsize 1.3 mul add moveto\n");
         printf("  yearstring show\n");
      }
      else {
         /* This code handles landscape orientation for the normal 2-page
            setup and for the compressed 1-page setup... */
         printf("  /w titlefontsize 0.6 mul def\n");
         printf("  leftmargin neg margin add\n");
         printf("  margin pageheight titlefontsize 2.25 mul sub 2 div sub moveto\n");
         printf("  1 1 4 {\n");
         printf("    /i exch def\n");
         printf("    /c yearstring i 1 sub 1 getinterval def\n");
         printf("    gsave\n");
         printf("    c w center show\n");
         printf("    grestore\n");
         printf("    0 titlefontsize neg rmoveto\n");
         printf("  } for\n");
      }
   }
   
   printf("} def\n");
   printf("\n");
   
   printf("%% \n");
   printf("%% This routine draws the abbreviated names of all 12 months.\n");
   printf("%% \n");
   printf("%% It takes a single parameter ('R','L', or 'C') to indicate\n");
   printf("%% the text justification -- Right, Left, or Center.\n");
   printf("%% \n");
   printf("/drawmonths {\n");
   
   if (rotate == LANDSCAPE) {
      printf("  /justify exch def\n");
   }
   
   printf("  titlefont findfont monthfontsize scalefont setfont\n");
   printf("  0 1 11 {\n");
   printf("    /i exch def\n");
   printf("    gsave\n");
   
   printf("    month_names i get %s\n",
          rotate == PORTRAIT ? "width center show" : "");
   
   if (rotate == LANDSCAPE) {
      printf("    justify (R) eq {\n");
      printf("      dup stringwidth pop neg 0 rmoveto\n");
      printf("    } if\n");
      printf("    justify (C) eq {\n");
      printf("      dup stringwidth pop neg 2 div 0 rmoveto\n");
      printf("    } if\n");
      printf("    show\n");
   }
   
   printf("    grestore\n");
   
   printf("    %s rmoveto\n",
          rotate == PORTRAIT ? "width 0" : "Xnext Ynext");
   
   printf("  } for\n");
   printf("} def\n");
   printf("\n");
   printf("/startpage {\n");
   
   printf("  /xtval %s add def\n",
          rotate == PORTRAIT ? "pagewidth 1 xsval sub mul margin" : "leftmargin fudge");
   
   printf("  /ytval pageheight %s def\n",
          rotate == PORTRAIT ? "topmargin sub fudge add" : "1 ysval sub mul margin add neg");
   
   printf("  rval rotate\n");
   printf("  xsval ysval scale\n");
   printf("  xtval ytval translate\n");
   printf("  newpath\n");
   
   printf("  %s neg %s fudge sub %s moveto\n",
          rotate == PORTRAIT ? "margin" : "leftmargin",
          rotate == PORTRAIT ? "topmargin" : "",
          rotate == PORTRAIT ? "" : "margin");
   
   printf("  pagewidth 0 rlineto\n");
   printf("  0 pageheight neg rlineto\n");
   printf("  pagewidth neg 0 rlineto closepath clip\n");
   printf("  0.1 setlinewidth\n");
   printf("  clippath setbackground fill\n");
   printf("  setforeground\n");
   printf("} def\n");
   printf("\n");
   

   printf("%% \n");
   printf("%% This routine draws a single number which represents the day of the month.\n");
   printf("%% \n");
   printf("/drawdate {\n");
   printf("  /daystr day 3 string cvs def\n");
   
   printf("  /%s margin halfwidth add radius sub %s def\n",
          rotate == PORTRAIT ? "w" : "h",
          rotate == PORTRAIT ? "" : "2 div");
   
   printf("  /y datefontsize 0.375 mul neg def\n");
   printf("  titlefont findfont datefontsize scalefont setfont\n");
   printf("  gsave\n");
   
   printf("  neghalfwidth %s rmoveto\n",
          rotate == PORTRAIT ? "margin sub y" : "radius h add y add");
   
   printf("  daystr %s center show\n",
          rotate == PORTRAIT ? "w" : "width");
   
   printf("  grestore\n");
   printf("  gsave\n");
   
   printf("  %s 11 mul radius %s rmoveto\n",
          rotate == PORTRAIT ? "width" : "neghalfwidth negheight",
          rotate == PORTRAIT ? "add y" : "sub h sub y add");
   
   printf("  daystr %s center show\n",
          rotate == PORTRAIT ? "w" : "width");
   
   printf("  grestore\n");
   printf("} def\n");
   printf("\n");
   
   printf("%% \n");
   printf("%% This routine draws 12 abbreviated day-of-week names, inside the graphical\n");
   printf("%% moons, 1 for each month, for the day-of-month currently being processed.\n");
   printf("%% \n");
   printf("/draw_inmoon_weekdays {\n");
   printf("  dayfont findfont weekdayfontsize scalefont setfont\n");
   printf("  /n day 1 sub 12 mul def\n");
   printf("  gsave\n");
   printf("  neghalfwidth weekdayfontsize 0.375 mul neg rmoveto\n");
   printf("  0 1 11 {\n");
   printf("    /month exch def\n");
   printf("    /phase moon_phases n get def\n");
   printf("    phase 0 ge {\n");
   printf("      /wkd startday month get day 1 sub add 7 mod def\n");
   printf("      gsave\n");
   printf("      day_names wkd get width center\n");
   printf("      phase .35 ge phase .65 le and {\n");
   printf("        setforeground show\n");
   printf("      } {\n");
   printf("        phase .85 gt phase .15 lt or {\n");
   printf("          setbackground show\n");
   printf("        } {\n");
   printf("          true charpath gsave setbackground\n");
   printf("          fill grestore stroke\n");
   printf("        } ifelse\n");
   printf("      } ifelse\n");
   printf("      grestore\n");
   printf("    } if\n");
   printf("    /n n 1 add def\n");
   printf("    Xnext Ynext rmoveto\n");
   printf("  } for\n");
   printf("  grestore\n");
   printf("} def\n");
   printf("\n");

   printf("%% \n");
   printf("%% This routine draws 12 abbreviated day-of-week names, to the lower left of\n");
   printf("%% the graphical moons, 1 for each month, for the day-of-month\n");
   printf("%% currently being processed.\n");
   printf("%% \n");
   printf("/draw_outmoon_weekdays {\n");
   printf("  dayfont findfont sm_weekdayfontsize scalefont setfont\n");
   printf("  /n day 1 sub 12 mul def\n");
   printf("  gsave\n");
   printf("  negwidth 0.27 mul negheight 0.27 mul sm_weekdayfontsize 0.75 mul sub rmoveto\n");
   printf("  0 1 11 {\n");
   printf("    /month exch def\n");
   printf("    /phase moon_phases n get def\n");
   printf("    phase 0 ge {\n");
   printf("      /wkd startday month get day 1 sub add 7 mod def\n");
   printf("      gsave\n");
   printf("      day_names wkd get\n");
   printf("      dup stringwidth pop neg 0 rmoveto\n");
   printf("      show\n");
   printf("      grestore\n");
   printf("    } if\n");
   printf("    /n n 1 add def\n");
   printf("    Xnext Ynext rmoveto\n");
   printf("  } for\n");
   printf("  grestore\n");
   printf("} def\n");
   printf("\n");
   printf("/domoon {\n");
   printf("  /phase exch def\n");
   printf("  gsave\n");
   printf("  currentpoint translate\n");
   printf("  newpath\n");
   printf("  phase halfperiod .01 sub ge phase halfperiod .01 add le and {\n");
   printf("    0 0 radius\n");
   printf("    0 360 arc stroke\n");
   printf("  } {\n");
   printf("    0 0 radius\n");
   printf("    0 0 radius\n");
   printf("    phase halfperiod lt {\n");
   printf("      270 90 arc stroke\n");
   printf("      0 radius neg moveto\n");
   printf("      270 90 arcn\n");
   printf("    } {\n");
   printf("      90 270 arc stroke\n");
   printf("      0 radius neg moveto\n");
   printf("      270 90 arc\n");
   printf("      /phase phase halfperiod sub def\n");
   printf("    } ifelse\n");
   printf("    /x1 quartperiod phase sub rect mul def\n");
   printf("    /y1 x1 abs 2 sqrt div def\n");
   printf("    x1\n");
   printf("    y1\n");
   printf("    x1\n");
   printf("    y1 neg\n");
   printf("    0\n");
   printf("    radius neg\n");
   printf("    curveto\n");
   printf("    fill\n");
   printf("  } ifelse\n");
   printf("  grestore\n");
   printf("} def\n");
   printf("\n");

   printf("%% \n");
   printf("%% This routine draws 12 graphical moons, 1 for each month, for the\n");
   printf("%% day-of-month currently being processed.\n");
   printf("%% \n");
   printf("/drawmoons {\n");
   printf("  /n day 1 sub 12 mul def\n");
   printf("  gsave\n");
   printf("  0 1 11 {\n");
   printf("    /phase moon_phases n get def\n");
   printf("    phase 0 ge {\n");
   printf("      phase domoon\n");
   printf("    } if\n");
   printf("    /n n 1 add def\n");
   printf("    pop\n");

   printf("    %s rmoveto\n",
          rotate == PORTRAIT ? "width 0" : "Xnext Ynext");

   printf("  } for\n");
   printf("  grestore\n");
   printf("} def\n");
   printf("\n");


   printf("%% \n");
   printf("%% This routine does everything needed to process a single day of the month,\n");
   printf("%% for all months at once.\n");
   printf("%% \n");
   printf("/process_one_day {\n");
   printf("  /day exch def\n");

   printf("    %s ",
          rotate == PORTRAIT ? "halfwidth Y0 day 1 sub negheight" : "X0 day 1 sub width");
   printf("%s ",
          odd_days_singlepage ? "0.5 mul" : "");
   printf("%s moveto\n",
          rotate == PORTRAIT ? "mul add" : "mul add neghalfheight");


   printf("  drawdate\n");
   printf("  drawmoons\n");
   printf("  inmoon_labels {\n");
   printf("    draw_inmoon_weekdays\n");
   printf("  } {\n");
   printf("    draw_outmoon_weekdays\n");
   printf("  } ifelse\n");
   printf("} def\n");
   printf("\n");

   printf("/draw_page_1 {\n");
   printf("  /fudge fudge1 def\n");
   printf("  startpage\n");
   printf("  drawtitle\n");


   printf("  %s moveto\n",
          rotate == PORTRAIT ? "0 10" : "radius halfwidth sub neghalfwidth monthfontsize 0.375 mul sub");

   printf("  %sdrawmonths\n",
          rotate == PORTRAIT ? "" : "(R)");

   printf("  /%s def\n",
          rotate == PORTRAIT ? "Y0 neghalfheight" : "X0 halfwidth");

   /* If odd-days-only output to a single page ('-O') has been requested,
      process all 31 days on 1 page, but increment the days by 2 instead of by
      1.  If output to a single page ('-S' or '-O') has been requested,
      process all 31 days on 1 page... */
   printf("  1 %d %d {\n", 
          odd_days_singlepage ? 2 : 1,
          (odd_days_singlepage || compressed_singlepage) ? 31 : 15);

   printf("    process_one_day \n");
   printf("  } for\n");
   printf("} def\n");
   printf("\n");

   /* If this is not a single-page calendar, create the routine to draw the
      2nd page... */

   if (!(compressed_singlepage || odd_days_singlepage)) {
      printf("/draw_page_2 {\n");
      printf("  /fudge fudge2 def\n");
      printf("  startpage\n");
      
      if (rotate == PORTRAIT) {
         printf("      /Y0 neghalfheight pageheight add def\n");
      }
      else {
         printf("      /X0 halfwidth pagewidth sub def\n");
      }
      
      printf("  16 1 31 {\n");
      printf("    process_one_day \n");
      printf("  } for\n");
      
      if (rotate == PORTRAIT) {
         printf("  0 Y0 31 negheight mul add moveto\n");
      }
      else {
         printf("  X0 31 width mul add radius sub neghalfwidth monthfontsize 0.375 mul sub moveto\n");
      }
      
      printf("  %sdrawmonths\n",
             rotate == PORTRAIT ? "" : "(L)");
      
      printf("} def\n");
      printf("\n");
   }

   
   /*
    * Write out PostScript code to print lunar calendar
    */
   
   printf("/year %d def\n", year);
   printf("/startday [");

   for (month = JAN; month <= DEC; month++) {
      printf("%2d", FIRST_OF(month, year));
   }

   printf(" ] def\n");
   
   /* invert moon phases (actually shift by 1/2 cycle, i.e., print first
    * quarter as third quarter, etc.) if background color is darker than
    * foreground color - thus ensuring that the visible portion of the moon is
    * printed in the lighter color
    */
   invert_moons = is_bg_darker();
   
   printf("/moon_phases [\n");
   for (day = 1; day <= 31; day++) {
      for (month = JAN; month <= DEC; month++) {
         phase = calc_phase(month, day, year);
         if (invert_moons) phase = fmod(phase + 0.5, 1.0);
         if (day <= LENGTH_OF(month, year)) {
            /* make sure printf() doesn't round "phase" up to 1.0 when printing it */
            printf("%.3f ", ((phase) >= 0.9995 ? 0.0 : (phase)));
         }
         else printf(" -1   ");
      }
      printf("\n");
   }
   printf("] def\n");
   
   printf("\n");
   
   printf("%%%%Page: 1st 1\n");
   printf("draw_page_1\n");
   printf("showpage\n");
   
   /* If this is not a single-page calendar, draw the 2nd page... */
   if (!(compressed_singlepage || odd_days_singlepage)) {
      printf("\n");
      printf("%%%%Page: 2nd 2\n");
      printf("draw_page_2\n");
      printf("showpage\n");
   }
   
   return;
}

/* ---------------------------------------------------------------------------

   get_args

   Notes:

      This routine walks the argument list.

      This utility routine was borrowed from 'pcal'.

      char **argv;        argument list 
      int curr_pass;      current pass 
      char *where;        for error messages 

*/
int get_args (char **argv, int curr_pass, char *where)
{
   char *parg, *opt;
   flag_usage_str_typ *pflag;
   int flag;
   time_t curr_tyme;   /* for getting current month/year */
   struct tm *p_tm;
   int badopt = FALSE;   /* flag set if bad param   */
   int nargs = 0;   /* count of non-flag args  */
   int numargs[MAXARGS];   /* non-flag (numeric) args */
   FILE *fp = stdout;   /* for piping "help" message */

/*
 * If argument follows flag (immediately or as next parameter), return
 * pointer to it (and bump argv if necessary); else return NULL
 */
#define GETARG() (*(*argv + 2) ? *argv + 2 : \
                  (*(argv+1) && **(argv+1) != '-' ? *++argv : NULL))

   /* Walk argument list, ignoring first element (program name) */

   while ((opt = *++argv) != NULL) {

      /* Assume that any non-flag argument is a numeric argument */
      if (*opt != '-') {
         if (curr_pass == P_CMD1 && nargs < MAXARGS) {
            if (! IS_NUMERIC(opt)) goto bad_par;
            numargs[nargs++] = atoi(opt);
         }
         continue;
      }

      /* Check that flag is legal */

      if (! (pflag = get_flag(flag = opt[1])) ) goto bad_par;
      
      /* get optional argument */
      
      parg = pflag->has_arg ? GETARG() : NULL;
      
      switch (flag) {
         
      case F_COMPR_1PAGE:
         compressed_singlepage = TRUE;
         break;

      case F_ODD_DAYS_1PAGE:
         odd_days_singlepage = TRUE;
         /* Can't have both methods ('compressed' and 'odd-days-only') of
            single-page mode enabled, so disable the compressed variant if
            it's enabled. */
         if (compressed_singlepage) compressed_singlepage = FALSE;
         break;

      case F_DAY_FONT:   /* specify alternate day font */
         strcpy(dayfont, parg ? parg : DATEFONT);
         break;
         
      case F_TITLE_FONT:   /* specify alternate title font */
         strcpy(titlefont, parg ? parg : TITLEFONT);
         break;
         
      case F_OUT_FILE:   /* specify alternate output file */
         strcpy(outfile, parg ? parg : OUTFILE);
         break;

      case F_LANDSCAPE:   /* generate landscape calendar */
         rotate = LANDSCAPE;
         break;
 
      case F_PORTRAIT:   /* generate portrait calendar */
         rotate = PORTRAIT;
         break;

      case F_SHADING:   /* set background/foreground shading */
         define_shading(shading, parg, SHADING);
         break;
         
      case F_HELP:   /* request "help" message */
      case F_USAGE:   /* request "usage" message */
      case F_VERSION:   /* request version stamp */
         /* PAGER_ENV (cf. lcaldefs.h) defines the name of an
          * environment variable which, if set, points to the
          * appropriate pager (e.g., "more", "less", "pg")
          * for piping the "help" message (Un*x systems only)
          */
#ifdef PAGER_ENV
         if (flag == F_HELP) {
            FILE *pfp;
            char *pager, *p;
            
            pager = (p = getenv(PAGER_ENV)) ? p : PAGER_DEFAULT;
            /* null pointer or string: no paging */
            if (pager && *pager && (pfp = popen(pager, "w")) != (FILE *)NULL) fp = pfp;
         }
#endif

         fprintf(fp, "%s\n", VERSION_STRING + 4);
         if (flag != F_VERSION) usage(fp, flag == F_HELP);
         fflush(fp);
#ifdef PAGER_ENV
         if (fp != stdout) pclose(fp);
#endif
         exit(EXIT_SUCCESS);
         break;
         
      case F_WEEKDAYS:   /* draw weekday names inside moons */
         draw_day_of_week_inside_moon = !(WEEKDAYS);
         break;

      case F_INVERT:   /* print white-on-black */
         strcpy(shading, INVERT);
         break;
         
      case F_XOFFSET:   /* X offset fudge factors */
         strcpy(x_offset, parg ? parg : X_OFFSET);
         break;
         
      case F_YOFFSET:   /* Y offset fudge factors */
         strcpy(y_offset, parg ? parg : Y_OFFSET);
         break;
         
      case F_TIMEZONE:   /* alternate time zone */
         strcpy(time_zone, parg ? parg : TIMEZONE);
         break;
         
      case '-' :   /* accept - and -- as dummy flags */
      case '\0':
         break;

      default:   /* missing case label if reached!!! */

      bad_par:   /* unrecognized parameter */

         fprintf(stderr, E_ILL_OPT, progname, opt);
         if (where) {
            fprintf(stderr, E_ILL_OPT2, curr_pass == P_ENV ? ENV_VAR : "", where);
         }
         fprintf(stderr, "\n");
         badopt = TRUE;
         break;
      }
   }

   if (curr_pass != P_CMD1) return !badopt;   /* return TRUE if OK, FALSE if error */

   /* Validate non-flag (numeric) parameters */
   
   time(&curr_tyme);
   p_tm = localtime(&curr_tyme);
   
   if (nargs == 0) {
      /* assume tm_year represents years elapsed since 1900 */
      init_year = CENTURY + p_tm->tm_year;
   } 
   else {
      init_year = numargs[0];
   }

   if (init_year > 0 && init_year < 100) {   /* treat nn as CCnn */
      init_year += 100 * ((CENTURY + p_tm->tm_year) / 100);
   }
   
   if (init_year < MIN_YR || init_year > MAX_YR) {
      fprintf(stderr, E_ILL_YEAR, progname, init_year, MIN_YR, MAX_YR);
      badopt = TRUE;
   }
   
   return !badopt;   /* return TRUE if OK, FALSE if error */
}

/* ---------------------------------------------------------------------------

   main

   Notes:

      Main program - parse and validate command-line arguments, open files,
      generate PostScript boilerplate and code to generate calendars.
     
      Program structure:
     
         main() looks for the environment variable 'LCAL_OPTS' and, if
         present, calls 'get_args()' to parse it.  It then calls 'get_args()'
         again to parse the command line.

*/
int main (int argc GCC_UNUSED, char **argv)
{
   char *p, tmp[STRSIZ];
   
   /* extract root program name and program path */
   
   strcpy(progname, **argv ? *argv : "lcal");
   
   if ((p = strrchr(progname, END_PATH)) != NULL) strcpy(progname, ++p);
   if ((p = strchr(progname, '.')) != NULL) *p = '\0';

   /* get version from VERSION_STRING (for use in PostScript comment) */
   strcpy(tmp, VERSION_STRING + 4);
   p = strchr(tmp, ' ') + 1;   /* skip program name */
   *strchr(p, ' ') = '\0';   /* terminate after version */
   strcpy(version, p);
   
   /*
    * Get the arguments from a) the environment variable, b) the command line
    */
   
   /* parse environment variable LCAL_OPTS as a command line */

   if ((p = getenv(LCAL_OPTS)) != NULL) {
      strcpy(lbuf, "lcal ");   /* dummy program name */
      strcat(lbuf, p);
      (void) loadwords(words, lbuf);   /* split string into words */
      if (! get_args(words, P_ENV, LCAL_OPTS)) {
         usage(stderr, FALSE);
         exit(EXIT_FAILURE);
      }
   }

   /* parse command-line arguments once to find name of moon file, etc. */
   
   if (!get_args(argv, P_CMD1, NULL)) {
      usage(stderr, FALSE);
      exit(EXIT_FAILURE);
   }
   
   /* done with the arguments and flags - try to open the output file */
   
   if (*outfile && freopen(outfile, "w", stdout) == (FILE *) NULL) {
      fprintf(stderr, E_FOPEN_ERR, progname, outfile);
      exit(EXIT_FAILURE);
   }
   
   /* generate the PostScript code */
   write_psfile(init_year);
   
   exit(EXIT_SUCCESS);
}
