#!/bin/sh

USAGE="
What: deskcal is a bash script to generate PostScript dodecahedral year desk calendar (one month per face)
Usage: bash $0 [YEAR]
example: bash $0 1990
Options set by script,
  YEAR      starting year (default: current year)
  MONTH     starting month (1-12; default=1)
  RANDRGB   random colored output (default b&w)
  SCALE     factor to shrink or enlarge the output (>0; default =1; use 1.4 for A3 paper)
  DATEFONT  is the PostScript font for printing the dates
  TITLEFONT is the PostScript font for printing the month/year and weekdays
  WEEKDAY   is the starting day of the week (0-6; 0=Sunday)
From from http://www2.mpia-hd.mpg.de/~mathar/progs/deskcal2
History:
  Written by Andrew Rogers (PostScript boilerplate adapted from Ole Arntzen polyeder.ps)
  Options added by Richard Mathar 2000-12-20
  Converted to bash by moshahmed@gmail 2018-09-08
"
 
YEAR=${1:-$(date "+%Y")}
MONTH=$(date +%m)

DATEFONT=Helvetica-Bold		
TITLEFONT=Helvetica-Bold
START=0			# default starting weekday (0=Sun)
SCALE=1			# default scaling (1 equiv A4 size)
RANDRGB=    # random coloring with RANDRGB=randrgb (else black and white)
LANG=en			# default language english

MONTHS="(January) (February) (March) (April) (May) (June) (July) (August) (September) (October) (November) (December)"
WKDAYS="(Su) (Mo) (Tu) (We) (Th) (Fr) (Sa)"
INSTRU="(Cut along solid line; fold along dashed lines)"

# Adjust month if out-of-range, year if no century given
[[ $MONTH < 1 || $MONTH > 12 ]] && MONTH=1
[[ $YEAR -lt 100 ]] && ((YEAR += 1900))

# Stuff for PostScript header
DATE=$(date)
NAME=$USERNAME

# Append boilerplate, substituting values of NAME, DATE, MONTH, YEAR,
# START, DATEFONT and TITLEFONT calculated above

cat << End-Of-File
%!PS-Adobe-2.0
%%Title: '$0' output
%%Creator: $NAME
%%CreationDate: $DATE
%%Pages: 1
%%Orientation: Portrait
%%EndComments

%--------------------------------------------------------------------------
%
% A self-contained PostScript program to create a template which can be cut
% and folded into a dodecahedral calendar (one month per face).
%
% Adapted from Ole Arntzen's (olea@ii.uib.no) universal polyhedron program,
% polyeder.ps, by Andrew Rogers.
%
% Version 1.3:	Use canonical calendar routines for starting day and length
%
% Version 1.2:	allow user to define starting month and first day of logical
%		week (cf. "/month" and "/startday" definitions below)
%
% Version 1.1:	flip calendars (right-side-up when viewed from top); revise
%		orientation of July..December; misc. minor tweaks
%
% This program is in the public domain.
%
%--------------------------------------------------------------------------

/month $MONTH def			% define starting month here (1 = Jan)
/year $YEAR def				% define starting year here
/startday $START def			% define first day of week (0 = Sun)
/datefont /$DATEFONT def		% font name (dates)
/titlefont /$TITLEFONT def		% font name (month/year/weekdays)

/months [() $MONTHS ] def
/wkdays [$WKDAYS] def

/mon [1 11 2 10 3 9 4 12 8 5 7 6] def	% position -> logical month
/pos 0 def				% starting position
/FEB 2 def				% significant months
/JUL 7 def

/LineLength 80 def  			% length of the edges
/fsize LineLength 10 idiv def		% font size

% set color to a random value, added by mathar
/randrgb { rand 257 mod 256 div
           rand 257 mod 256 div
           rand 257 mod 256 div setrgbcolor } def

/center {				% str width center >> --
  /width exch def
  /str exch def
  width str stringwidth pop sub 2 div 0 rmoveto str show
  $RANDRGB
} def

/strcat {				% str1 str2 >> str1str2
  2 copy
  length exch length
  dup 3 -1 roll add
  string
  dup 0 6 -1 roll putinterval
  dup 3 -1 roll 4 -1 roll putinterval
} def

/is_leap {			% year => boolean [true: leap; false: common]
    dup 4 mod 0 eq 1 index 100 mod 0 ne 2 index 400 mod 0 eq or and exch pop
} def

/month_len {			% month[1-12] year => int[28-31]
    1 index FEB eq { is_leap { pop 29 } { pop 28 } ifelse }
                   { pop dup 8 idiv add 2 mod 30 add } ifelse
} def

/day_of_week {			% month[1-12] day year => weekday[0-6]
	3 2 roll dup FEB le { 12 add exch 1 sub exch } if
	dup 2 mul exch 1 add 3 mul 5 idiv add exch	% Zeller's Congruence
	dup dup dup 4 idiv exch 100 idiv sub exch 400 idiv add add
	add add 1 add 7 mod
} def

/printcal {
  gsave
  /m mon pos get def				% get logical month

  % rotate logical July..December for consistent orientation with Jan..June
  m JUL ge {
    /angle NumEdges 2 sub 180 mul NumEdges div def
    LineLength 2 div LineLength angle sin angle 2 div cos add mul translate
    angle 2 mul rotate
  } if

  % translate logical month to physical month and year
  /m m month add 2 sub 12 mod 1 add def
  /y m month lt { year 1 add } { year } ifelse def

  % flip calendars for legibility from top/side (thanks to Scott Schwartz)
  180 rotate LineLength neg LineLength -1.35 mul translate

  /Y LineLength 1.05 mul def				% month/year heading
  0 Y moveto
  titlefont findfont fsize scalefont setfont
  months m get (  ) strcat y 4 string cvs strcat LineLength center

  /l m y month_len def					% length of month
  /s m 1 y day_of_week def				% starting day
  /s s startday sub 7 add 7 mod def			% starting position

  datefont findfont fsize 0.875 mul scalefont setfont	% centering info
  /w3 (222) stringwidth pop def
  /w2 (22) stringwidth pop def
  /X LineLength w3 6 mul w2 add sub 2 div def

  titlefont findfont fsize 0.75 mul scalefont setfont	% weekdays
  /Y Y fsize 1.5 mul sub def
  0 1 6 {
    /w exch def
    X w w3 mul add Y moveto
    wkdays w startday add 7 mod get w2 center
  } for

  datefont findfont fsize 0.875 mul scalefont setfont	% dates
  /Y Y fsize sub def
  1 1 l {
    /d exch 3 string cvs def
    X s 7 mod w3 mul add w2 add d stringwidth pop sub
      Y s 7 idiv fsize mul sub moveto
    d show
    $RANDRGB
    /s s 1 add def
  } for
  grestore
  /pos pos 1 add def
} def

/ReadCharacter
{
% This routine looks for an interesting character, and return it on
% the stack.  Illegal character => Quit.
  /OneCharacter 1 string def
  {
    currentfile OneCharacter readstring % Read one character.
    not { (Unexpected end of FILE.  Quit) print quit } if
    OneCharacter (e) eq OneCharacter (f) eq or { exit } if
    OneCharacter (3) ge OneCharacter (9) le and { exit } if
    pop

    OneCharacter (%) eq
    {   % Found comment character, drop rest of line.
      {
        currentfile OneCharacter readstring     % Read one character.
        not { (Unexpected end of FILE.  Quit) print quit } if
        pop
        OneCharacter (\012) eq { exit } if
      } loop
    }
    {
      OneCharacter ( ) gt
      {
        % Illegal character => Quit.
        (Illegal character: ") print
        OneCharacter print
        ("\012.  Quit) print
        quit
      } if
    } ifelse
  } loop
} def

/DrawEdge
{
  0 0 moveto
  LineLength 0 lineto $RANDRGB stroke
} def

/DrawFlip
{
  [1 4] 4 setdash
  0 0 moveto
  LineLength 0 lineto stroke
  [] 0 setdash
  0 0 moveto
  LineLength 0.5 mul LineLength 0.3125 mul neg lineto
  LineLength 0 lineto stroke
} def

/InnerLoop
{
  /OneCharacter ReadCharacter def   % Read one character.
  OneCharacter (e) eq { DrawEdge }
    { OneCharacter (f) eq { DrawFlip } { DrawPolygon } ifelse } ifelse

  LineLength 0 translate
  CurrentAngle rotate
} def

/DrawPolygon
{
  [1 4] 4 setdash
  0 0 moveto
  LineLength 0 lineto stroke
  [] 0 setdash
  CurrentAngle  % Put previous CurrentAngle on stack for later use.

  /NumEdges OneCharacter cvi def
  /CurrentAngle 360 NumEdges div def
  180 CurrentAngle add rotate

  NumEdges 1 sub {
    InnerLoop
  } repeat

  printcal

  LineLength 0 translate    % Transformer back to start.
  180 rotate

  /CurrentAngle exch def    % Fetch CurrentAngle from the stack.
} def

/DrawPolyhedron
{

  /OneCharacter ReadCharacter def   % Read one character.
  /NumEdges OneCharacter cvi def
  /CurrentAngle 360 NumEdges div def

  printcal

  NumEdges {
    InnerLoop
  } repeat

} def

% added by mathar
 /reencsmalldict 24 dict def /ReEncodeSmall
{reencsmalldict begin /newcodesandnames exch def /newfontname exch def
/basefontname exch def /basefontdict basefontname findfont def
 /newfont basefontdict maxlength dict def basefontdict
{exch dup /FID ne {dup /Encoding eq {exch dup length array copy
newfont 3 1 roll put} {exch newfont 3 1 roll put} ifelse}
{pop pop} ifelse } forall
newfont /FontName newfontname put newcodesandnames aload pop
newcodesandnames length 2 idiv {newfont /Encoding get 3 1 roll put} repeat
newfontname newfont definefont pop end } def /accvec [
8#220 /dotlessi 8#221 /grave 8#222 /acute 8#223 /circumflex
8#224 /tilde 8#226 /breve 8#227 /dotaccent 8#232 /ring
8#235 /hungarumlaut 8#236 /ogonek 8#237 /caron 8#241 /exclamdown
8#242 /cent 8#243 /sterling 8#244 /currency 8#245 /yen
8#246 /brokenbar 8#247 /section 8#250 /dieresis 8#251 /copyright
8#252 /ordfeminine 8#253 /guillemotleft 8#254 /logicalnot 8#255 /hyphen
8#256 /registered 8#257 /macron 8#260 /degree 8#261 /plusminus
8#262 /twosuperior 8#263 /threesuperior 8#264 /acute 8#265 /mu
8#266 /paragraph 8#267 /periodcentered 8#270 /cedilla 8#271 /onesuperior
8#272 /ordmasculine 8#274 /onequarter 8#275 /onehalf 8#276 /threequarters
8#277 /questiondown 8#300 /Agrave 8#301 /Aacute 8#302 /Acircumflex
8#303 /Atilde 8#304 /Adieresis 8#305 /Aring 8#306 /AE
8#307 /Ccedilla 8#310 /Egrave 8#311 /Eacute 8#312 /Ecircumflex
8#313 /Edieresis 8#314 /Igrave 8#315 /Iacute 8#316 /Icircumflex
8#317 /Idieresis 8#320 /Eth 8#321 /Ntilde 8#322 /Ograve
8#323 /Oacute 8#324 /Ocircumflex 8#325 /Otilde 8#326 /Odieresis
8#327 /multiply 8#330 /Oslash 8#331 /Ugrave 8#332 /Uacute
8#333 /Ucircumflex 8#334 /Udieresis 8#335 /Yacute 8#336 /Thorn
8#337 /germandbls 8#340 /agrave 8#341 /aacute 8#342 /acircumflex
8#343 /atilde 8#344 /adieresis 8#345 /aring 8#346 /ae
8#347 /ccedilla 8#350 /egrave 8#351 /eacute 8#352 /ecircumflex
8#353 /edieresis 8#354 /igrave 8#355 /iacute 8#356 /icircumflex
8#357 /idieresis 8#360 /eth 8#361 /ntilde 8#362 /ograve
8#363 /oacute 8#364 /ocircumflex 8#365 /otilde 8#366 /odieresis
8#367 /divide 8#370 /oslash 8#371 /ugrave 8#372 /uacute
8#373 /ucircumflex 8#374 /udieresis 8#375 /yacute 8#376 /thorn
8#377 /ydieresis ] def
/$TITLEFONT /$TITLEFONT accvec ReEncodeSmall

% Draw pentagon dodecahedron.

0.1 setlinewidth      			% line thickness

$SCALE $SCALE scale		% added by mathar
gsave
270 350 translate			% ensure output fits on page
DrawPolyhedron
5					% directions for making dodecahedron
 5 f 5fff    e ee
 5 f 5fff    e ee
 5 f 5fff    e ee
 5 f 5ff5eeeee ee
 5 f 5fff    e ee
grestore

% Print some instructions.

/Helvetica findfont 12 scalefont setfont
40 40 moveto
$INSTRU show

showpage
%%EOF
End-Of-File

