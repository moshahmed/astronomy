-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
Overview:

   'Lcal' is a multi-platform program which generates PostScript lunar
   calendars in a yearly format, showing a graphical representation of the
   phase of the moon for every day in the specified year.

   Currently, 'lcal' is supported on the following platforms:

      - Unix (including GNU/Linux)
      - DOS (including DOS running under Windows)
      - DOS + DJGPP
      - Windows + Cygwin

   Historically, 'lcal' was supported on other platforms as well, but
   beginning with version 2.0.0, support for these platforms has been dropped:

      - Vax/VMS
      - Amiga

   'Lcal' is run from the command line.

   By default, it generates a 2-page calendar, with the first half of all 12
   months on page 1 and the last half of all 12 months on page 2.  However, a
   1-page (full year on a single sheet) version can be generated as well,
   either in a compressed format or in an uncompressed, odd-days-only format.

   Both portrait and landscape page orientations are supported.

   Currently, 'lcal' only supports US-letter-sized (8.5 x 11.0 inches) paper.

   By default, the calendar is printed in black and white, but colors are
   supported.

   For much more detail on the available options and use of 'lcal', use any of
   the following sources:

      - View the manual page for 'lcal' by running 'man lcal' after
        installation.

      - View any one of the supplied help files:

           'lcal-help.ps' -- PostScript (nicest to view)
           'lcal-help.html' -- HTML
           'lcal-help.txt' -- ASCII text

        These files contain the exact same content as the manual page ('man
        lcal') but in different formats.

      - Run the command 'lcal -h' to get a detailed list of help and
        command-line options.  

        This output is not nearly as thorough as the help sources mentioned
        above, but it can be useful as a brief reminder of the options and
        their usage.

   The 'lcal' (and 'pcal' [conventional calendar]) homepage is:

      http://pcal.sourceforge.net

   The 'pcal' and 'lcal' applications are available at:
   
      http://sourceforge.net/projects/pcal/

   A section entitled "Guidelines for Application Maintainers" can be found
   here:

      http://pcal.sourceforge.net/maintainers.html

-----------------------------------------------------------------------------
-----------------------------------------------------------------------------

Building 'lcal':

   Unix/Linux (or DOS/Windows + Cygwin):

      make
      make install  (as 'root' user)

   DOS (Borland, etc):

      make -f Makefile.DOS

         Successfully compiles with Borland C++ v5.0 (and probably several
         other earlier/later versions of Borland C++ or Turbo C/C++).

      make -f Makefile.DOS install

   DOS (using DJGPP):

      make OS=DJGPP
      make OS=DJGPP install

-----------------------------------------------------------------------------
-----------------------------------------------------------------------------

"Lcal" Version 2.0.0

   This release of 'lcal' includes additional functionality, some bug fixes,
   and an update of the associated documentation.

   Major changes:

      1) New functionality:

         - A new 1-page, uncompressed format was added.  

           Previously, if you wanted single-page output of a whole year, you
           had to use the '-S' option, which compressed the output, resulting
           in very small moon icons.  That option is still available.

           Beginning with this release, you can use the '-O' option to support
           a 1-page uncompressed format which only displays the odd days of
           the month, allowing a whole year to fit on one page without having
           to compress the content at all.  This leaves the moon icons at
           their normal (large) size and uses the page space more efficiently,
           all without significantly reducing the calendar's functionality.

         - Added support for some new environments, essentially making 'lcal'
           support all the same platforms as her "sister" program 'pcal'.
           Newly-supported platforms include:

              - DOS (including DOS under Windows)
              - DOS + DJGPP
              - DOS/Windows + Cygwin              

      2) Removed functionality:

         - Support for the Amiga and Vax/VMS platforms has been removed.

           These platforms are essentially obsolete and the files and code
           needed to accommodate these platforms was cluttering things up and
           interfering with further progress by needlessly complicating the
           code.

           Anyone needing support for these platforms is advised to continue
           using the 1.2 release of 'lcal'.

      3) Bug fixes:
   
         - Fixed a couple of long-standing, recently-uncovered bugs in the use
           of the timezone ('-z') option, which is used in the 'moon phase'
           calculations:

              (1) Negative timezone arguments, used for areas east of
                  (i.e. later than) UTC/Greenwich (e.g. '-z-5'), were not
                  being processed correctly. (They were erroneously adjusted
                  by adding a full 24 hours to make them positive.) 

                  Thanks to Lalit Chhabra for reporting this bug.
   
              (2) The user-specified timezone offset was being normalized to
                  +/- 12 hours. 

                  Although this works for the vast majority of users, there
                  are places in the world which are outside that range. For
                  example, Christmas Island (aka 'Kiritimati', in the Pacific
                  Ocean) is in the UTC+14 timezone. The local time there is 14
                  hours ahead of UTC, so the 'pcal' option would be
                  '-z-14'. Obviously, with such places in the world, we can no
                  longer normalize the user-specified timezone offset to +/-
                  12 hours.

         - Fixed a long-standing bug whereby the help text for several of the
           options was never being displayed when 'lcal' was invoked with the
           '-h' option.

      4) Other changes:

         - Provided better page separation in the PostScript output, thanks to
           the redesign work mentioned below.

           This provides the ability, using a PostScript previewer application
           (e.g. 'gv') to page backwards from page 2 to page 1 of a 2-page
           calendar.  In the past, this never worked because of the way the
           PostScript was generated.

           This also causes PostScript previewer applications to properly show
           the number of pages in the file.

         - Changed the format for the timestamp in the PostScript comment
           output ('%%CreationDate:') to one which is more thorough and less
           ambiguous.

         - Provide the URL to the 'pcal'/'lcal' website in the header
           (comment) section ('%%Creator:') of the PostScript output.

      5) Of interest to 'lcal' hackers and installers:

         - Made a major change to 'lcal' design.

           Previously, the PostScript output was generated in part using an
           external program ('pcalinit[.c]') which read the PostScript
           template file ('lcal_{p,l}.ps') and automatically generated a C
           header file ('lcal_{p,l}.h').

           This old method had a few disadvantages:

              (1) The PostScript output that it generated was ugly, with no
                  whitespace (vertically or horizontally) and devoid of the
                  (useful) comments that were part of the PostScript template
                  file.

                  This made reading the PostScript much harder.

              (2) It was harder to take advantage of patterns in the
                  PostScript output, causing redundancy.

              (3) It was harder to compile (especially as the size of the
                  PostScript template file grew) under limited environments,
                  like DOS.

           The new method generates all the PostScript output using C code
           only.  

           Minor optimizations in the code can be made this way, without
           needlessly replicating similar chunks of PostScript code.  This
           puts some of the logic behind the PostScript output back into C
           code, where it's easier to see what's happening.

           In truth, this change was first made (recently) to the 'lcal'
           application, where the inefficiencies are greater.  Logically,
           however, 'pcal' benefited from the same change, so it was made
           there as well.

         - Provided comments in the output file for many of the PostScript
           routines, documenting what they do.

         - Made a lot of minor tweaks to the code, improving variable names,
           removing useless macros, etc.

   Credits:
   
      The original calendar PostScript was Copyright (c) 1987 by Patrick Wood
      and Pipeline Associates, Inc. with permission to modify and
      redistribute.
   
      The following people contributed to Lcal v2.0.0:

         New 1-page, uncompressed format, 
            bug fixes, various other fixes:		Bill Marr

      For a list of all known contributors to date, see the 'Authors' section
      of the 'man' page.
   
   Bill Marr (marr99@users.sourceforge.net) 
   05 Aug 2006

-----------------------------------------------------------------------------
-----------------------------------------------------------------------------

"Lcal" Version 1.2

   This is a new release of "lcal", the PostScript lunar phase calendar
   program.  The major new features are:

      . a Y2K-related bug in the default year calculation has been fixed;

      . a man page has been added;

      . many program defaults may now be specified in the Makefile;

      . the Makefile has several new targets (read it for details);

      . the usage message is piped through a screen pager (Un*x only).

      . the -s flag (see below) allows the user to specify the background
        and foreground colors as grayscale values or red:green:blue values.

      . the -S flag (see below) prints a smaller, single-page calendar.

      . the distribution includes .html and .cgi files for setting up an
        Lcal server using Netscape (maybe others; I haven't tried them)
        under Un*x.

        Jun 2006 (Bill Marr):

           This HTML/CGI feature was not part of the 'lcal' 1.2 release after
           it was moved to SourceForge and released with the 'pcal' 4.7.1
           release.  It appears that those files were lost somewhere along the
           way and have not been restored.

   Jun 2006 (Bill Marr):

     Detailed notes here describing the available options and their usage have
     been deleted since they're superceded by the notes in the 'man lcal' page
     (and other sources, as described above in this file).

Credits:

The original calendar PostScript was Copyright (c) 1987 by Patrick Wood
and Pipeline Associates, Inc. with permission to modify and redistribute.

The following people contributed to Lcal (directly or indirectly):

	Moon phase code:				Jef Poskanzer and
							  Craig Leres
	VMS support:					Richard Dyson
	Other C and PostScript code:			Andrew Rogers

Lcal was inspired by "Moonlight 1996", a 16" x 36" full-color (silver moons
against a midnight blue background) lunar phase calendar marketed by
Celestial Products, Inc., P.O. Box 801, Middleburg VA  22117.  Send for
their catalog to see (and, hopefully, order) this as well as some even more
amazing stuff - particularly "21st Century Luna", a lunar phase calendar for
*every day* of the new century.

Andrew Rogers
25 January 2000

-----------------------------------------------------------------------------
-----------------------------------------------------------------------------

Original 'ReadMe' file from "Pcal"


"Pcal" is a program to print PostScript calendars for any month and year.
By default, it looks for a file in the home directory named "calendar"
for entries with leading dates matching dates on the calendar, and prints
any following text under the appropriate day.

The program may be a little System V flavored (getopt, time routines)
but should be easily portable to other vintages of UNIX.

Pcal is the combined effort of several people, most notably Patrick Wood
of Pipeline Associates, Inc. for the original PostScript code and Bill
Vogel of AT&T for the calendar file mechanism.  My [Ken Keirnan's] part was
simple translation to a "C" program, the addition of a couple options and
a more generalized date searching routine (oh yes, and a manual page :-).

The original calendar PostScript was Copyright (c) 1987 by Patrick Wood
and Pipeline Associates, Inc. with permission to modify and redistribute.
Please retain this README file with the package.


Ken Keirnan
Pacific Bell
San Ramon, CA.

-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
