/* ---------------------------------------------------------------------------

   lcaldefs.h
   
   Notes:

      This file contains various definitions and declarations of items used
      throughout the application.
     
   Revision history:

	2.0.0
		B.Marr		2006-07-20
		
		Reformatted comments and code to match my standards.
		
		B.Marr		2006-06-24
		
		Major overhaul.  Eliminated lots of unnecessary items.
		Cleaned up lots of other items.
		
		Merged 'lcallang.h' and 'protos.h' into this file.
		
		Added a defined constant (string) to represent the
		'lcal'/'pcal' website in the PostScript output.
		
*/


/* ---------------------------------------------------------------------------
   
   Notes:

      This file contains various definitions and function prototypes.

*/

/* ---------------------------------------------------------------------------

   Header Files

*/

/* ---------------------------------------------------------------------------

   Type, Struct, & Enum Declarations

*/

/* 
 * Global typedef declaration for 'flag usage' (cf. lcal.c, get_args())
 */
typedef struct {
   char flag;   /* name of flag */
   char has_arg;   /* TRUE if flag takes (optional) arg */
} flag_usage_str_typ;

/*
 * Global typedef declarations for 'usage message' (cf. lcal.c, usage())
 */
typedef struct {
   char flag;   /* name of flag */
   char *meta;   /* metaname for argument (if any) */
   char *text;   /* associated text */
   char *def;   /* default value (if any) */
} flag_msg_str_typ;

typedef struct {
   char *desc;   /* description of param */
   char *text;   /* associated text */
} param_msg_str_typ;

/* ---------------------------------------------------------------------------

   Constant Declarations

*/

/* Define strings for comments in PostScript output file... */
#define PS_RELEASE      "PS-Adobe-1.0"
#define LCAL_WEBSITE    "http://pcal.sourceforge.net"

/*
 * System dependencies:
 */
#ifdef BUILD_ENV_MSDOS
/* oddities for MS-DOS */
#define END_PATH   '\\'
#else
/* not MS-DOS -- assume Unix or DOS+DJGPP */
#define END_PATH   '/'

/* 
   PAGER_ENV -- points to help message pager
   PAGER_DEFAULT -- default pager (NULL = none)
*/
#define PAGER_ENV "PAGER"
#define PAGER_DEFAULT "more"
#endif

/* define OUTFILE to "" (stdout) */
#define OUTFILE   ""

/*
 * Environment variables:
 */
#define LCAL_OPTS   "LCAL_OPTS"   /* command-line flags */

/*
 * Miscellaneous other constants:
 */

#define P_ENV		1	/* environment variable / command line pass */
#define P_CMD1		2

#ifndef FALSE
#define FALSE		0	/* pseudo-Booleans */
#define TRUE		1
#endif

#define MIN_YR		1753	/* significant years (calendar limits) */
#define MAX_YR		9999

#define CENTURY		1900	/* base year for struct tm year field */

#define JAN		 1	/* significant months */
#define FEB		 2
#define DEC		12

#define SUN		 0
#define SAT		 6

#define SCREENWIDTH	72	/* command-line message in usage() */

#define STRSIZ		200	/* size of misc. strings */
#define MAXWORD		100
#define LINSIZ		512	/* size of source line buffer */

#define MAXARGS		1	/* numeric command-line args */

#define DIGITS		"0123456789"
#define WHITESPACE	" \t"

/*
 * Defaults for calendar layout:
 */

/* default font names */
#ifndef DATEFONT
#define DATEFONT   "Times-Bold"
#endif
#ifndef TITLEFONT
#define TITLEFONT   "Times-Bold"
#endif

/* 

   Define the various font sizes... 

*/

/* The title font (for the year of the calendar) must be made much smaller
   (and relocated) to fit everything on one page when the odd-days-only ('-O')
   method of single-page output has been enabled... */
#define	TITLEFONTSIZE_NORMAL	72
#define	TITLEFONTSIZE_ODD_DAYS	18

#define	DATEFONTSIZE	16		/* dates			*/
#define	MONTHFONTSIZE	16		/* month names			*/
#define	WKDFONTSIZE	10		/* weekday names (inside moons) */
#define	SMWKDFONTSIZE	 7		/* weekday names		*/

#define	DATEFONTSIZE_S	18		/* enlarge font sizes slightly	*/
#define	MONTHFONTSIZE_S	18		/* for legibility when output	*/
#define SMWKDFONTSIZE_S	 9		/* is compressed (-S)		*/

/*
 * Very few PostScript printers will actually print to the edges of each page.
 * These site-specific magic numbers (also definable in the Makefile)
 * compensate for this.
 */
#ifndef X_OFFSET
#define X_OFFSET	"0/0"		/* X origin offset (p.1, p.2) */
#endif
#ifndef Y_OFFSET
#define Y_OFFSET	"0/0"		/* Y origin offset (p.1, p.2) */
#endif

#define LANDSCAPE	90		/* degrees to rotate */
#define PORTRAIT	 0
#define ROTATE		LANDSCAPE	/* default */

#define WEEKDAYS	FALSE		/* weekdays inside moons? */

#define FULL_SIZE	1.0		/* scale factor (normal)      */
#define HALF_SIZE	0.5		/* scale factor (single page) */

#define PAGEBREAK	15		/* days printed on first page   */
#define PAGEBREAK_S	33		/* no page break if single page */

/*
 * default time zone is UTC; site-specific time zone may be defined here
 * or in the Makefile
 */
#ifndef TIMEZONE
#define TIMEZONE	"0"		/* hours west of UTC */
#endif

/*
 * background/foreground colors (one grayscale value or three r:g:b values)
 */
#define SHADING		"1/0"		/* default: black-on-white */
#define INVERT		"0/1"		/* -i flag: white-on-black */
#define RGB_CHAR	':'		/* r:g:b value separator */


/* size of abbreviations  */
#define MIN_DAY_LEN	2
#define MIN_MONTH_LEN	3


/*
 * Symbolic names for command-line flags.  These may be changed
 * as desired in order to be meaningful in languages other than
 * English.
 */
#define F_DAY_FONT	'd'		/* select alternate day font */
#define F_TITLE_FONT	't'		/* select alternate title font */

#define F_OUT_FILE	'o'		/* select alternate output file */

#define F_LANDSCAPE	'l'		/* landscape mode */
#define F_PORTRAIT	'p'		/* portrait mode */

#define F_SHADING	's'		/* set background/foreground shading */

#define F_INVERT	'i'		/* print white on black background */

#define F_HELP		'h'		/* generate help message */
#define F_USAGE		'u'		/* generate usage message */
#define F_VERSION	'v'		/* generate version stamp */

#define F_WEEKDAYS	'W'		/* weekday names inside moons */

#define F_XOFFSET	'X'		/* fudge factor for X offset (-l) */
#define F_YOFFSET	'Y'		/* fudge factor for Y offset (-p) */

#define F_TIMEZONE	'z'		/* specify alternate time zone */

#define F_COMPR_1PAGE	'S'		/* print compressed, single-page calendar */
#define F_ODD_DAYS_1PAGE	'O'	/* print odd-days-only, single-page calendar */

#define W_FONT		"<FONT>"		/* names of metavariables */
#define W_FILE		"<FILE>"
#define W_VALUE		"<VALUE>"
#define W_VAL2		"<n>{/<n>}"
#define W_SHADING	"{<b>}{/<f>}"

/* special flag_msg[] entries for end of option group, etc. */

#define END_GROUP	'\n', NULL, NULL, NULL		/* end of option group */
#define END_LIST	'\0', NULL, NULL, NULL		/* end of list */
#define GROUP_DEFAULT	' ', NULL, " "			/* group default */

/* Define the number of entries from the 'param_msg[]' array to print in
   command-line syntax message */
#define PARAM_MSGS	1

/* Error and information messages - translate as necessary */

/* program error messages */
#define	E_FOPEN_ERR	"%s: can't open file %s\n"
#define	E_ILL_OPT	"%s: unrecognized flag %s"
#define E_ILL_OPT2	" (%s\"%s\")"
#define	E_ILL_YEAR	"%s: year %d not in range %d .. %d\n"
#define E_FLAG_IGNORED	"%s: -%c flag ignored (%s\"%s\")\n"
#define ENV_VAR		"environment variable "

/* ---------------------------------------------------------------------------

   Macro Definitions
   
*/

/* 
 * Define a 'convenience' macro used to explicitly declare arguments as
 * 'unused' where intentionally-unused function parameters are declared.  This
 * prevents warnings by the 'gcc' GNU C compiler that would otherwise appear
 * because of using the '-W' compile option.
 *
 * The Unix and DOS+DJGPP build environments support this ability.  We assume
 * that all other build environments do not support it, so this macro is
 * defined as a null value in such cases.
 */ 
#if defined (BUILD_ENV_UNIX) || defined (BUILD_ENV_DJGPP)
#define GCC_UNUSED  __attribute__ ((unused))
#else
#define GCC_UNUSED
#endif

/*
 * Function-like macros:
 */

#define IS_LEAP(y)   ((y) % 4 == 0 && ((y) % 100 != 0 || (y) % 400 == 0))
#define LENGTH_OF(m, y) (month_len[(m)-1] + ((m) == FEB && IS_LEAP(y)))
#define YEAR_LEN(y)   (IS_LEAP(y) ? 366 : 365)
#define DAY_OF_YEAR(m, d, y) ((month_off[(m)-1] + ((m) > FEB && IS_LEAP(y))) + d)
#define OFFSET_OF(m, y) ((month_off[(m)-1] + ((m) > FEB && IS_LEAP(y))) % 7)
#define FIRST_OF(m, y)   calc_weekday(m, 1, y)

#define P_LASTCHAR(p)   ((p) && *(p) ? (p) + strlen(p) - 1 : NULL)
#define LASTCHAR(p)   (p)[strlen(p) - 1]

#define IS_NUMERIC(p)   ((p)[strspn((p), DIGITS)] == '\0')

/* ---------------------------------------------------------------------------

   Data Declarations (including externals)

*/

/* ---------------------------------------------------------------------------

   External Routine References & Function Prototypes

*/
