#!/bin/sh

# $Id: pscal.sh,v 1.1 2014-01-05 03:11:54 a Exp $

# Copyright (C) 1987 by Pipeline Associates, Inc.
# Permission is granted to modify and distribute this free of charge.

# initializations
V=1.15

LANGUAGE="$LANG"

TITLE_FONT=Times-Bold
DAY_FONT=Helvetica-Bold
EVENT_FONT=Times-Roman

LANDSCAPE_ROTATE=90
LANDSCAPE_SCALE="1.0 1.0"
LANDSCAPE_TRANSLATE="50 -120"
LANDSCAPE_BOUNDINGBOX="21 45 570 755"

PORTRAIT_ROTATE=0
PORTRAIT_SCALE="0.75 0.75"
PORTRAIT_TRANSLATE="50 900"
PORTRAIT_BOUNDINGBOX="30 335 570 755"

ROTATE="$LANDSCAPE_ROTATE"
SCALE="$LANDSCAPE_SCALE"
TRANSLATE="$LANDSCAPE_TRANSLATE"
BOUNDINGBOX="$LANDSCAPE_BOUNDINGBOX"

MOON=0
DIAMETER=13

DAYNUMS="false"

EUROPEAN_STYLE="false"

# set messages, based on the value of $LANGUAGE
setmessage() {
#
# CATALAN
#
    if test \
	    "$LANGUAGE" = "ca" -o \
	    "$LANGUAGE" = "catalan" -o \
	    "$LANGUAGE" = "Catalan";
    then
	VERSION="Pscal-$V: un formatador de calendaris en PostScript"

	USAGE="\
Pscal és un programa que genera un PostScript per imprimir calendaris

Us: pscal [-hmnRrv] [-d diàmetre] [-eFf font] [-L idioma] [mes [any]]
Opcions:
  -d  diàmetrede la lluna (predeterminat = 13)
  -e  font dels events (predeterminat = Times-Roman)
  -F  font del títol (predeterminat = Times-Bold)
  -f  font del dia (predeterminat = Helvetica-Bold)
  -h  missatge ajut
  -L  idioma (predeterminat = English)
  -M  mostra les fases de la lluna (hemisferi sud)
  -m  mostra les fases de la lluna (hemisferi nord)
  -n  mostra el número del dia
  -R  no gira (vertical)
  -r  gira 90 graus (apaisat)
  -S  European style (Dilluns first)
  -s  American style (Diumenge first)
  -v  número de versió
"
	MONTH_NAMES="(Gener) (Febrer) (Març) (Abril) (Maig) (Juny) (Juliol) \
(Agost) (Setembre) (Octubre) (Novembre) (Decembre)"

	SUNDAY="(Diumenge)"
	DAY_NAMES="(Dilluns) (Dimarts) (Dimecres) (Dijous) (Divendres) \
(Dissabte)"

#
# DANISH
#
    elif test \
	    "$LANGUAGE" = "da" -o \
	    "$LANGUAGE" = "da_DK" -o \
	    "$LANGUAGE" = "danish" -o \
	    "$LANGUAGE" = "Danish" -o \
	    "$LANGUAGE" = "dansk" -o \
	    "$LANGUAGE" = "Dansk";
    then
	VERSION="Pscal-$V: formater en PostScript kalender"

	USAGE="\
Pscal er et program til at printe kalendere i PostScript.

Brug: pscal [-hmnRrv] [-d diameter] [-eFf font] [-L sprog] [måned [år]]
Valgmuligheder:
  -d  månediameter (normalværdi = 13)
  -e  skrifttype til begivenheder (normalværdi = Times-Roman)
  -F  skrifttype til titel (normalværdi = Times-Bold)
  -f  skrifttype til navne på dage (normalværdi = Helvetica-Bold)
  -h  hjælp
  -L  sprog (normalværdi = English)
  -M  medtag månens faser (sydlige halvkugle)
  -m  medtag månens faser (nordlige halvkugle)
  -n  medtag dagens nummer
  -R  roter ikke kalender (stående)
  -r  roter med 90 grader (liggende)
  -S  European style (Mandag first)
  -s  American style (Søndag first)
  -v  versionsnummer
"
	MONTH_NAMES="(Januar) (Februar) (Marts) (April) (Maj) (Juni) (Juli) \
(August) (September) (Oktober) (November) (December)"

	SUNDAY="(Søndag)"
	DAY_NAMES="(Mandag) (Tirsdag) (Onsdag) (Torsdag) (Fredag) (Lørdag)"

#
# ENGLISH
#
    elif test \
	    "$LANGUAGE" = "" -o \
	    "$LANGUAGE" = "C" -o \
	    "$LANGUAGE" = "en" -o \
	    "$LANGUAGE" = "en_US" -o \
	    "$LANGUAGE" = "english" -o \
	    "$LANGUAGE" = "English";
    then
	VERSION="Pscal-$V: a PostScript calendar formatter"

	USAGE="\
Pscal is a PostScript program to print calendars.

Usage: pscal [-hmnRrv] [-d diameter] [-eFf font] [-L language] [month [year]]
Options:
  -d  moon diameter (default = 13)
  -e  event font (default = Times-Roman)
  -F  title font (default = Times-Bold)
  -f  day font (default = Helvetica-Bold)
  -h  help message
  -L  language (default = English)
  -M  show moon phases (southern hemisphere)
  -m  show moon phases (northern hemisphere)
  -n  show day numbering
  -R  don't rotate (portrait)
  -r  rotate by 90 degrees (landscape)
  -S  European style (Monday first)
  -s  American style (Sunday first)
  -v  version number

Example:
  1> sh pscal.sh -d 4 -m 1 2003  > cal-01-03.ps
  2> gs cal-01-03.ps
  3> pscal  \`date +\"%m %Y\"\`  > cal-\`date +\"%m-%Y\"\`
  4> for y in 2003 ;do
       for m in 01 02 03 04 05 06 07 08 09 10 11 12 ;do
         pscal -d 4 -m \$m \$y > cal-\$m-\$y.ps
       done
     done
  5> ls -l cal-*

"
	MONTH_NAMES="(January) (February) (March) (April) (May) (June) (July) \
(August) (September) (October) (November) (December)"

	SUNDAY="(Sunday)"
	DAY_NAMES="(Monday) (Tuesday) (Wednesday) (Thursday) (Friday) \
(Saturday)"

#
# ESPERANTO
#
    elif test \
	    "$LANGUAGE" = "eo" -o \
	    "$LANGUAGE" = "esperanto" -o \
	    "$LANGUAGE" = "Esperanto";
    then
	VERSION="Pscal-$V: Postskripta kalendar-preparilo"

	USAGE="\
Pscal estas programo en Postskripto [PostScript] por presi kalendarojn.

Uzo: pscal [-hmnRrv] [-d diametro] [-eFf tiparo] [-L lingvo] [monato [jaro]]
Elektiloj (kaj iliaj defaultoj):
  -d  diametro de la lun-simbolo (13)
  -e  tiparo por eventoj (Times-Roman)
  -F  tiparo por titolo (Times-Bold)
  -f  tiparo por tagoj (Helvetica-Bold)
  -h  help-mesagho
  -L  lingvo (Angla)
  -M  montru lun-fazon por suda hemisfero
  -m  montru lun-fazon por norda hemisfero
  -n  montru tag-numerojn
  -R  ne turnu: t.n. portret-formato
  -r  turnu je kvarono: t.n. pejzagh-formato
  -S  Europa aspekto (semajno komencas lunde)
  -s  Usona aspekto (semajno komancas dimanche)
  -v  eligu version

Bv-u noti ke la programo ne povas presi la ghustajn Esperantajn
supersignojn (mankas subteno por Unikodo au ISO-Latin-3); anstataue
ghi uzas la kutiman \`Zamenhofan' au \`fundamentan' transliteradon.
"

	MONTH_NAMES="(Januaro) (Februaro) (Marto) (Aprilo) (Majo) (Junio) \
(Julio) (Augusto) (Septembro) (Oktobro) (Novembro) (Decembro)"

	SUNDAY="(Dimancho)"
	DAY_NAMES="(Lundo) (Mardo) (Merkredo) (Jhaudo) (Vendredo) (Sabato)"

#
# FINNISH
#
    elif test \
	    "$LANGUAGE" = "fi" -o \
	    "$LANGUAGE" = "en_FI" -o \
	    "$LANGUAGE" = "finnish" -o \
	    "$LANGUAGE" = "Finnish";
    then
	VERSION="Pscal-$V: PostScript kalenterin muokkaaja"

	USAGE="\
Pscal on PostScript ohjelma kalentereiden tulostukseen.

Usage: pscal [-hmnRrv] [-d diameter] [-eFf font] [-L language] [month [year]]
Options:
  -d  kuun halkaisija (oletus = 13)
  -e  tapahtuma fontti (oletus = Times-Roman)
  -F  otsikko fontri (oletus = Times-Bold)
  -f  päivä fontti (oletus = Helvetica-Bold)
  -h  käyttöohje
  -L  kieli (oletus = English)
  -M  näytä kuun vaiheet (eteläinen pallonpuolisko)
  -m  näytä kuun vaiheet (pohjoinen pallonpuolisko)
  -n  näytä päivä numerointi
  -R  älä käännä (portrait)
  -r  käännä 90 astetta (landscape)
  -S  European style (Maanantai first)
  -s  American style (Sunnuntai first)
  -v  versio numero
"
	MONTH_NAMES="(Tammikuu) (Helmikuu) (Maaliskuu) (Huhtikuu) (Toukokuu) \
(Kesäkuu) (Heinäkuu) (Elokuu) (Syyskuu) (Lokakuu) (Marraskuu) (Joulukuu)"

	SUNDAY="(Sunnuntai)"
	DAY_NAMES="(Maanantai) (Tiistai) (Keskiviikko) (Torstai) (Perjantai) \
(Lauantai)"

#
# FRENCH
#
    elif test \
	    "$LANGUAGE" = "fr" -o \
	    "$LANGUAGE" = "french" -o \
	    "$LANGUAGE" = "French";
    then
	VERSION="Pscal-$V: formateur PostScript de calendrier"

	USAGE="\
Pscal est un programme PostScript destiné à imprimer les calendriers.

Usage: pscal [-hmnRrv] [-d diamètre] [-eFf font] [-L langage] [mois [année]]
Options:
  -d  diamètre de la Lune (défaut = 13)
  -e  event font (défaut = Times-Roman)
  -F  title font (défaut = Times-Bold)
  -f  day font (défaut = Helvetica-Bold)
  -h  message d'aide
  -L  langage (défaut = English)
  -M  montrer les phases de la Lune (méridional hémisphère)
  -m  montrer les phases de la Lune (nordique hémisphère)
  -n  montrer la numérotation des jours
  -R  ne pas tourner (portrait)
  -r  tourner de 90 degrés (paysage)
  -S  style Européen  (Lundi au début)
  -s  style Américain (Dimanche au début)
  -v  numéro de version
"
	MONTH_NAMES="(Janvier) (Février) (Mars) (Avril) (Mai) (Juin) \
(Juillet) (Août) (Septembre) (Octobre) (Novembre) (Décembre)"

	SUNDAY="(Dimanche)"
	DAY_NAMES="(Lundi) (Mardi) (Mercredi) (Jeudi) (Vendredi) (Samedi)"

#
# GERMAN
#
    elif test \
	    "$LANGUAGE" = "de" -o \
	    "$LANGUAGE" = "de_DE" -o \
	    "$LANGUAGE" = "german" -o \
	    "$LANGUAGE" = "German";
    then
	VERSION="Pscal-$V: a PostScript calendar formatter"

	USAGE="\
Pscal is a PostScript program to print calendars.

Usage: pscal [-hmnRrv] [-d diameter] [-eFf font] [-L language] [month [year]]
Options:
  -d  moon diameter (default = 13)
  -e  event font (default = Times-Roman)
  -F  title font (default = Times-Bold)
  -f  day font (default = Helvetica-Bold)
  -h  help message
  -L  language (default = English)
  -M  show moon phases (southern hemisphere)
  -m  show moon phases (northern hemisphere)
  -n  show day numbering
  -R  don't rotate (portrait)
  -r  rotate by 90 degrees (landscape)
  -S  European style (Montag first)
  -s  American style (Sonntag first)
  -v  version number
"
	MONTH_NAMES="(Januar) (Februar) (März) (April) (Mai) (Juni) (Juli) \
(August) (September) (Oktober) (November) (Dezember)"

	SUNDAY="(Sonntag)"
	DAY_NAMES="(Montag) (Dienstag) (Mittwoch) (Donnerstag) (Freitag) \
(Samstag)"

#
# ITALIAN
#
    elif test \
	    "$LANGUAGE" = "it" -o \
	    "$LANGUAGE" = "it_IT" -o \
	    "$LANGUAGE" = "it_IT.ISO-8859-1" -o \
	    "$LANGUAGE" = "italian" -o \
	    "$LANGUAGE" = "Italian";
    then
	VERSION="Pscal-$V: un creatore di calendari in formato Postscript"

	USAGE="\
Pscal è un programma PostScript per stampare calendari.

Uso: pscal [-hmnRrv] [-d diametro] [-eFf font] [-L lingua] [mese [anno]]
Opzioni:
  -d  diametro della luna (default = 13)
  -e  font della ricorrenza (default = Times-Roman)
  -F  font del titolo (default = Times-Bold)
  -f  font del giorno (default = Helvetica-Bold)
  -h  messaggio di help
  -L  lingua (default = English)
  -M  mostra le fasi lunari (emisfero meridionale)
  -m  mostra le fasi lunari (emisfero settentrionale)
  -n  mostra la numerazione dei giorni
  -R  non ruotare (verticale)
  -r  ruota di 90 gradi (orizzontale)
  -S  European style (Lunedì first)
  -s  American style (Domenica first)
  -v  informazioni sulla versione
"
	MONTH_NAMES="(Gennaio) (Febbraio) (Marzo) (Aprile) (Maggio) (Giugno) \
(Luglio) (Agosto) (Settembre) (Ottobre) (Novembre) (Dicembre)"

	SUNDAY="(Domenica)"
	DAY_NAMES="(Lunedì) (Martedì) (Mercoledì) (Giovedì) \
(Venerdì) (Sabato)"

#
# POLISH
#
    elif test \
	    "$LANGUAGE" = "pl" -o \
	    "$LANGUAGE" = "pl_PL" -o \
	    "$LANGUAGE" = "pl_PL.ISO_8859-2" -o \
	    "$LANGUAGE" = "polish" -o \
	    "$LANGUAGE" = "Polish";
    then
	VERSION="Pscal-$V: program generuj±cy kalendarz w PostScripcie"

	USAGE="\
Pscal jest programem drukuj±cym kalendarz w PostScripcie.

Sk³adnia: pscal [-hmnRrv] [-d wielko¶æ] [-eFf czcionka] [-L jêzyk] [miesi±c [rok]]
Opcje:
  -d  wielko¶æ obrazka przedstawiaj±cego fazy ksiê¿yca (domy¶lnie = 13)
  -e  czcionka zdarzeñ (domy¶lnie = Times-Roman)
  -F  czcionka tytu³u (domy¶lnie = Times-Bold)
  -f  czcionka dnia (domy¶lnie = Helvetica-Bold)
  -h  pomoc
  -L  jêzyk (domy¶lnie = English)
  -M  dodaje fazy ksiê¿yca (pó³kula po³udniowa)
  -m  dodaje fazy ksiê¿yca (pó³kula pó³nocna)
  -n  dodaje numeracjê dni (od pocz±tku i do koñca roku)
  -R  bez obrotu (pionowo)
  -r  obrót o 90 stopni (poziomo)
  -S  uk³ad europejski (pierwszy dzieñ tygodnia = poniedzia³ek)
  -s  uk³ad amerykañski (pierwszy dzieñ tygodnia = niedziela)
  -v  wypisuje numer wersji
W celu uzyskania poprawnych czcionek na wydruku nale¿y u¿yæ 'ogonkify -AT -N'.
"
	MONTH_NAMES="(Styczeñ) (Luty) (Marzec) (Kwiecieñ) (Maj) \
(Czerwiec) (Lipiec) (Sierpieñ) (Wrzesieñ) (Pa¼dziernik) (Listopad) \
(Grudzieñ)"

	SUNDAY="(Niedziela)"
	DAY_NAMES="(Poniedzia³ek) (Wtorek) (¦roda) (Czwartek) \
(Pi±tek) (Sobota)"

#
# PORTUGUESE
#
    elif test \
	    "$LANGUAGE" = "pt_BR" -o \
	    "$LANGUAGE" = "portuguese" -o \
	    "$LANGUAGE" = "Portuguese";
    then
	VERSION="Pscal-$V: um formatador de calendário PostScript"

	USAGE="\
Pscal é um programa PostScript que imprime calendários

Uso: pscal [-hmnRrv] [-d diametro] [-eFf fonte] [-L linguagem] [mes [ano]]
Opções:
  -d  diâmetro da lua (padrão = 13)
  -e  fonte para eventos (padrão = Times-Roman)
  -F  fonte para o título (padrão = Times-Bold)
  -f  fonte para o dia (padrão = Helvetica-Bold)
  -h  ajuda
  -L  linguagem (padrão = English)
  -M  mostra fase da lua (hemisfério sul)
  -m  mostra fase da lua (hemisfério norte)
  -n  mostra numeração dos dias
  -R  não rotacionar o calendário
  -r  rotacionar o calendário em 90 graus
  -S  European style (Segunda first)
  -s  American style (Domingo first)
  -v  mostra versão do programa
"
	MONTH_NAMES="(Janeiro) (Fevereiro) (Março) (Abril) (Maio) (Junho) \
(Julho) (Agosto) (Setembro) (Outubro) (Novembro) (Dezembro)"

	SUNDAY="(Domingo)"
	DAY_NAMES="(Segunda) (Terça) (Quarta) (Quinta) (Sexta) (Sábado)"

#
# SERBIAN
#
    elif test \
	    "$LANGUAGE" = "yu" -o \
	    "$LANGUAGE" = "sr" -o \
	    "$LANGUAGE" = "srpski" -o \
	    "$LANGUAGE" = "serbian" -o \
	    "$LANGUAGE" = "Srpski" -o \
	    "$LANGUAGE" = "Serbian";
    then
	VERSION="Pscal-$V: PostScript kalendar"

	USAGE="\
Pscal je PostScript program za stampanje kalendara.

Usage: pscal [-hmnRrv] [-d diameter] [-eFf font] [-L jezik] [mesec[godina]]
Options:

  -d  diameter meseca (default = 13)
  -e  stil slova za Dogadjaj (default = Times-Roman)
  -F  stil slova za Naslovna (default = Times-Bold)
  -f  stil slova za Dan (default = Helvetica-Bold)
  -h  pomoc
  -L  jezik (default = English)
  -M  pokazi meseceve mene (juzna hemisfera)
  -m  pokazi meseceve mene (severna hemisfera)
  -n  show day numbering
  -R  ne rotiraj (portret)
  -r  rotiraj za 90 stepeni (landscape)
  -S  Europski stil  (Ponedeljak prvi)
  -s  Americki stil  (Nedelja prva)
  -v  verzija
"
	MONTH_NAMES="(Januar) (Februar) (Mart) (April) (Maj) (Jun) (Jul) \
(August) (Septembar) (Oktobar) (Novembar) (Decembar)"

	SUNDAY="(Nedelja)"
	DAY_NAMES="(Ponedeljak) (Utorak) (Sreda) (Cetvrtak) (Petak) (Subota)"

#
# SPANISH
#
    elif test \
	    "$LANGUAGE" = "es" -o \
	    "$LANGUAGE" = "spanish" -o \
	    "$LANGUAGE" = "Spanish";
    then
	VERSION="Pscal-$V: un formato de calendarios en PostScript"

	USAGE="\
Pscal es un programa que genera un PostScript para imprimir calendarios

Uso: pscal [-hmnRrv] [-d diámetro] [-eFf font] [-L idioma] [mes [año]]
Opcions:
  -d  diámetro de la luna (predeterminado = 13)
  -e  fuente de los eventos (predeterminado = Times-Roman)
  -F  fuente del título (predeterminado = Times-Bold)
  -f  fuente del día (predeterminado = Helvetica-Bold)
  -h  mensaje de ayuda
  -L  idioma (predeterminado = English)
  -M  muestra las fases de la luna (hemisferio sur)
  -m  muestra las fases de la luna (hemisferio norte)
  -n  muestra el número del día
  -R  no gira (vertical)
  -r  gira 90 grados (horizontal)
  -S  European style (Lunes first)
  -s  American style (Domingo first)
  -v  número de versión
"
	MONTH_NAMES="(Enero) (Febrero) (Marzo) (Abril) (Mayo) (Junio) (Julio) \
(Agosto) (Septiembre) (Octubre) (Noviembre) (Diciembre)"

	SUNDAY="(Domingo)"
	DAY_NAMES="(Lunes) (Martes) (Miércoles) (Jueves) (Viernes) \
(Sábado)"

#
# TURKISH
#
    elif test \
	    "$LANGUAGE" = "tr" -o \
	    "$LANGUAGE" = "tr_TR" -o \
	    "$LANGUAGE" = "turkish" -o \
	    "$LANGUAGE" = "Turkish";
    then
	VERSION="Pscal-$V: PostScript takvim Ã¼reticisi"

	USAGE="\
Pscal takvim Ã§Ä±ktÄ±sÄ± Ã¼reten PostScript bir yazÄ±lÄ±mdir.

KullanÄ±m: pscal [-hmnRrv] [-d Ã§ap] [-eFf font] [-L dil] [ay [yÄ±l]]
SeÃ§enekler:
  -d  ay Ã§apÄ± (ilkdeÄŸer = 13)
  -e  tatil fontu (ilk deÄŸer = Times-Roman)
  -F  baÅŸlÄ±k fontu (ilk deÄŸer = Times-Bold)
  -f  gÃ¼n fontu (ilk deÄŸer = Helvetica-Bold)
  -h  yardÄ±m sayfasÄ±
  -L  dil (ilk deÄŸer = English)
  -M  ay'Ä±n evrelerini gÃ¶ster (gÃ¼ney yarÄ±kÃ¼re iÃ§in)
  -m  ay'Ä±n evrelerini gÃ¶ster (kuzey yarÄ±kÃ¼re iÃ§in)
  -n  gÃ¼n sayÄ±larÄ±nÄ± gÃ¶ster
  -R  takvimi Ã§evirme (dikey)
  -r  takvimi 90 derece Ã§evir (yatay)
  -S  Avrupa biÃ§imli (Pazartesi Ã¶nce)
  -s  Amerikan biÃ§imli (Pazar Ã¶nce)
  -v  sÃ¼rÃ¼m sayÄ±sÄ±
"
	MONTH_NAMES="(Ocak) (Åžubat) (Mart) (Nisan) (MayÄ±s) \
(Haziran) (Temmuz) (AÄŸustos) (EylÃ¼l) (Ekim) (KasÄ±m) \
(AralÄ±k)"

	SUNDAY="(Pazar)"
	DAY_NAMES="(Pazartesi) (SalÄ±) (Ã‡arÅŸamba) \
(PerÅŸembe) (Cuma) (Cumartesi)"

#
# UNKNOWN
#
    else
	echo "Error: Unknown language: $LANGUAGE" 1>&2
	echo "       Known languages: Catalan, Danish, English, Esperanto, \
Finnish, French," 1>&2
	echo "                        German, Italian, Polish, Portuguese, \
Serbian, Spanish," 1>&2
	echo "                        and Turkish" 1>&2
	exit 1
    fi
} # setmessage


# default messages
setmessage

# process arguments
while test $# != 0; do
    case $1 in
	-d)	test $# -lt 2 && { echo "$USAGE" 1>&2; exit 1; }
		DIAMETER="$2";
		shift 2;;
	-d*)	DIAMETER=`echo $1 | sed -n 1s/-.//p`;
		shift;;
	-e)	test $# -lt 2 && { echo "$USAGE" 1>&2; exit 1; }
		EVENT_FONT="$2";
		shift 2;;
	-e*)	EVENT_FONT=`echo $1 | sed -n 1s/-.//p`;
		shift;;
	-F)	test $# -lt 2 && { echo "$USAGE" 1>&2; exit 1; }
		TITLE_FONT="$2";
		shift 2;;
	-F*)	TITLE_FONT=`echo $1 | sed -n 1s/-.//p`;
		shift;;
	-f)	test $# -lt 2 && { echo "$USAGE" 1>&2; exit 1; }
		DAY_FONT="$2";
		shift 2;;
	-f*)	DAY_FONT=`echo $1 | sed -n 1s/-.//p`;
		shift;;
	-h*|-\?)	echo "$USAGE" 1>&2;
		exit 1;;
	-L)	test $# -lt 2 && { echo "$USAGE" 1>&2; exit 1; }
		LANGUAGE="$2";
		setmessage;
		shift 2;;
	-L*)	LANGUAGE=`echo $1 | sed -n 1s/-.//p`;
		setmessage;
		shift;;
	-M)	MOON="2";
		shift;;
	-m)	MOON="1";
		shift;;
	-n)	DAYNUMS="true";
		shift;;
	-R)	ROTATE="$PORTRAIT_ROTATE";
		SCALE="$PORTRAIT_SCALE";
		TRANSLATE="$PORTRAIT_TRANSLATE";
		BOUNDINGBOX="$PORTRAIT_BOUNDINGBOX";
		shift;;
	-r)	ROTATE="$LANDSCAPE_ROTATE";
		SCALE="$LANDSCAPE_SCALE";
		TRANSLATE="$LANDSCAPE_TRANSLATE";
		BOUNDINGBOX="$LANDSCAPE_BOUNDINGBOX";
		shift;;
	-S)     EUROPEAN_STYLE="true"
		shift;;
	-s)     EUROPEAN_STYLE="false"
		shift;;
	-v)	echo "$VERSION" 1>&2;
		exit 1;;
	--|-)	break;;
	*)	break
    esac
done

# try to figure out what year this is
getyear() {
    YEAR=`date "+%Y" 2>&1`
    if echo "$YEAR" | grep '^[0-9][0-9][0-9][0-9]$' > /dev/null 2>&1; then
	return;
    fi

    # some versions of date don't support %Y, and the year might not be last
    # on the line depending on what locale you're in, so grab something that's
    # four digits and hope it's the year
    YEAR=`date | sed -n '1s/^.* \([0-9][0-9][0-9][0-9]\).*$/\1/p'`
    if echo "$YEAR" | grep '^[0-9][0-9][0-9][0-9]$' > /dev/null 2>&1; then
	return;
    fi

    # Do the best we can with the last two digits
    YEAR=`date "+%y" 2>&1`
    if echo "$YEAR" | grep '^[0-9][0-9]$' > /dev/null 2>&1; then
	YEAR="20$YEAR"
	return;
    fi

    echo "Error: Can't determine the year!"
    exit 1
} # getyear


# do something with the remaining arguments
case $# in
    0)	MONTH=`date "+%m" | sed 's/^0//'`;
	getyear;;
    1)	MONTH=$1;
	getyear;;
    2)	MONTH=$1 YEAR=$2;;
    *)  echo "$USAGE" 1>&2;
	exit 1;;
esac

# determine which holiday file to use
if [ -n "$EFILE" -a -r "$EFILE" ]; then
	Files=$EFILE
elif [ -r Events ]; then
	Files=Events
elif [ -r $HOME/.holiday ]; then
	Files=$HOME/.holiday
else
	Files=/dev/null
fi

# convert events into a form acceptable to PostScript
holidays=`grep "^0*$MONTH:" $Files | awk -F: '{ printf("%s ( %s", $2, $3); \
		for (i = 4; i <= NF; i++) printf(":%s", $i); printf(")\n"); }'`

# european style: rotate weekdays
if [ $EUROPEAN_STYLE = "true" ]; then
    DAY_NAMES="$DAY_NAMES $SUNDAY"
else
    DAY_NAMES="$SUNDAY $DAY_NAMES"
fi

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

cat <<END-OF-CALENDAR
%!PS-Adobe-3.0 EPSF-1.2
%%Title: Calendar
%%Creator: pscal $V
%%CreationDate: `date`
%%BoundingBox: $BOUNDINGBOX
%%DocumentFonts: $TITLE_FONT $DAY_FONT $EVENT_FONT
%%Pages: 1
%%EndComments
%%BeginProlog
% PostScript program to draw calendar
% Copyright (C) 1987 by Pipeline Associates, Inc.
% Permission is granted to modify and distribute this free of charge.
%
% The number after /month should be set to a number from 1 to 12.
% The number after /year should be set to the year you want.
% You can change the title and date fonts, if you want.
% We figure out the rest.
% This program won't produce valid calendars before 1800 due to the switch
% from Julian to Gregorian calendars in September of 1752 wherever English
% was spoken.

/isolatin1encoding [
 /.notdef /.notdef /.notdef /.notdef /.notdef
 /.notdef /.notdef /.notdef /.notdef /.notdef
 /.notdef /.notdef /.notdef /.notdef /.notdef
 /.notdef /.notdef /.notdef /.notdef /.notdef
 /.notdef /.notdef /.notdef /.notdef /.notdef
 /.notdef /.notdef /.notdef /.notdef /.notdef
 /.notdef /.notdef /space /exclam /quotedbl
 /numbersign /dollar /percent /ampersand /quoteright
 /parenleft /parenright /asterisk /plus /comma
 /hyphen /period /slash /zero /one
 /two /three /four /five /six /seven /eight /nine /colon /semicolon
 /less /equal /greater /question /at /A /B /C /D /E
 /F /G /H /I /J /K /L /M /N /O
 /P /Q /R /S /T /U /V /W /X /Y
 /Z /bracketleft /backslash /bracketright /asciicircum
 /underscore /quoteleft /a /b /c
 /d /e /f /g /h /i /j /k /l /m
 /n /o /p /q /r /s /t /u /v /w
 /x /y /z /braceleft /bar /braceright /asciitilde /.notdef /.notdef /.notdef
 /.notdef /.notdef /.notdef /.notdef /.notdef
 /.notdef /.notdef /.notdef /.notdef /.notdef
 /.notdef /.notdef /.notdef /.notdef /.notdef
 /.notdef /.notdef /.notdef /.notdef /.notdef
 /.notdef /.notdef /.notdef /.notdef /.notdef
 /.notdef /.notdef /.notdef /.notdef /.notdef
 /space /exclamdown /cent /sterling /currency
 /yen /brokenbar /section /dieresis /copyright
 /ordfeminine /guillemotleft /logicalnot /hyphen /registered
 /macron /degree /plusminus /twosuperior /threesuperior
 /acute /mu /paragraph /periodcentered /cedilla
 /onesuperior /ordmasculine /guillemotright /onequarter /onehalf
 /threequarters /questiondown /Agrave /Aacute /Acircumflex
 /Atilde /Adieresis /Aring /AE /Ccedilla
 /Egrave /Eacute /Ecircumflex /Edieresis /Igrave
 /Iacute /Icircumflex /Idieresis /Eth /Ntilde
 /Ograve /Oacute /Ocircumflex /Otilde /Odieresis
 /multiply /Oslash /Ugrave /Uacute /Ucircumflex
 /Udieresis /Yacute /Thorn /germandbls /agrave
 /aacute /acircumflex /atilde /adieresis /aring
 /ae /ccedilla /egrave /eacute /ecircumflex
 /edieresis /igrave /iacute /icircumflex /idieresis
 /eth /ntilde /ograve /oacute /ocircumflex
 /otilde /odieresis /divide /oslash /ugrave
 /uacute /ucircumflex /udieresis /yacute /thorn /ydieresis ] def

/isofindfont {
	/new_isofont exch findfont
	dup length dict begin
	{1 index /FID ne {def} {pop pop} ifelse} forall
	/Encoding isolatin1encoding def
	currentdict
	end	
	definefont
} def	% isofindfont

/pscaldict 100 dict def
pscaldict begin

% calendar names - change these if you don't speak english
% "August", "April" and "February" could stand to be kerned even if you do
/month_names [ $MONTH_NAMES ] def
/day_names [ $DAY_NAMES ] def

% when printing a european calendar, some weekdays are changed
/wd6 $EUROPEAN_STYLE {5} {6} ifelse def
/wd1 $EUROPEAN_STYLE {6} {1} ifelse def

% layout parameters - you can change these, but things may not look nice

/daywidth 100 def
/dayheight 95 def

/titlefontsize 48 def
/weekdayfontsize 12 def
/datefontsize 30 def
/footfontsize 20 def
/eventfontsize 9 def

/topgridmarg 35 def
/leftmarg 35 def
/daytopmarg 10 def
/dayleftmarg 5 def

% layout constants - don't change these, things probably won't work

/rows 5 def
/subrows 6 def

% calendar constants - change these if you want a French revolutionary calendar

/days_week 7 def

/days_month [ 31 28 31 30  31  30  31  31  30  31  30  31 ] def

/days_sofar [  0 31 59 90 120 151 181 212 243 273 304 334 ] def

/isleap {				% is this a leap year?
        /theyear exch def
	theyear 4 mod 0 eq		% multiple of 4
	theyear 100 mod 0 ne		% not century
	theyear 400 mod 0 eq or and	% unless it's divisible by 400
} def	% isleap

/ndays {				% number of days in this month
	/themonth exch def
	/theyear exch def
	days_month themonth 1 sub get
	month 2 eq			% February
	theyear isleap and
	{
		1 add
	} if
} def	% ndays

/weekday {				% weekday (range 0-6) for integer date
	days_week mod
} def	% weekday

/startday {				% starting day-of-week for this month
	/off year 9600 sub def		% offset from start of "epoch"
	off
	off 4 idiv add			% number of leap years
	off 100 idiv sub		% number of centuries
	off 400 idiv add		% number of extra weird days
	wd6 add weekday days_week add	% offset from Saturday, Jan 1, 9600
	/off exch def
	1 1 month 1 sub {
		/idx exch def
		days_month idx 1 sub get
		idx 2 eq
		year isleap and
		{
			1 add
		} if
		/off exch off add def
	} for
	off weekday			% 0--Sunday, 1--monday, etc.
} def	% startday

/prtevent {		% event-string day prtevent
			%  print out an event
	/start startday def
	/day 2 1 roll def
	day start add 1 sub 7 mod daywidth mul
	day start add 1 sub 7 div truncate dayheight neg mul
	-5		% offset
	numevents day start add get -10 mul add
	numevents day start add
	numevents day start add get 1 add put
	add moveto	% move OVER and DOWN appropriate amount
	isdouble {	% go UP some if it's double and 30 or 31
	    { 0 dayheight numevents day 7 sub start add get 10 mul sub rmoveto
	      ( >>) show }
	    { ( <<) show } % give a hint as to what day the event is on
	  ifelse
	} if
     show
} def	% prtevent

/drawevents {		% read in a file full of events; print
			%  the events for this month
	/numevents
	[0 0 0 0 0 0 0
	 0 0 0 0 0 0 0
	 0 0 0 0 0 0 0
	 0 0 0 0 0 0 0
	 0 0 0 0 0 0 0
	 0 0 0 0 0 0 0] def
	 eventfont isofindfont eventfontsize scalefont setfont
	 0 2 holidays length 2 sub {
		dup
		1 add holidays 2 1 roll get       % loads string
		2 1 roll holidays 2 1 roll get    % loads day
		prtevent
	} for
} def	% drawevents

% ------------------------------------------------------------------------

/prtnum { 3 string cvs show } def

/center {				% center string in given width
	/width exch def
	/str exch def width str
	stringwidth pop sub 2 div 0 rmoveto str show
} def	% center

/centernum { exch 3 string cvs exch center } def

/drawgrid {				% draw calendar boxes
	titlefont isofindfont weekdayfontsize scalefont setfont
	currentpoint /y0 exch def /x0 exch def
	0 1 days_week 1 sub {
		submonth not
		{
			x0 y0 moveto
			dup dup daywidth mul 40 rmoveto
			day_names exch get
			daywidth center
		} if
		x0 y0 moveto
		daywidth mul topgridmarg rmoveto
		1.0 setlinewidth
		submonth
		{
			/rowsused rows def
		}
		{
			/rowsused rows 1 sub def
		} ifelse
		0 1 rowsused {
			gsave
			daywidth 0 rlineto
			0 dayheight neg rlineto
			daywidth neg 0 rlineto
			closepath stroke
			grestore
			0 dayheight neg rmoveto
			pop
		} for
	} for
} def	% drawgrid

/drawnums {				% place day numbers on calendar
	dayfont isofindfont datefontsize
	submonth
	{
		2.5 mul
	} if scalefont setfont
	/start startday def
	/days year month ndays def
	start daywidth mul dayleftmarg add daytopmarg rmoveto
	submonth
	{
		dayleftmarg neg dayheight -2 div rmoveto
	} if
	1 1 days {
		/day exch def
		gsave
		day start add weekday 0 eq
		{
			submonth not
			{
				.7 setgray
			} if
		} if
		day start add weekday wd1 eq
		{
			submonth not
			{
				.7 setgray
			} if
		} if
		submonth
		{
			day daywidth centernum
		}
		{
			isdouble
			{
				day prtdouble
				daynumsflag day 30 lt and
				{
					day daynumsdouble
				} if
			}
			{
				day prtnum
				daynumsflag
				{
					day daynums
				} if
			} ifelse
		} ifelse
		grestore
		day start add weekday 0 eq
		{
			currentpoint exch pop dayheight sub 0 exch moveto
			submonth not
			{
				dayleftmarg 0 rmoveto
			} if
		}
		{
			daywidth 0 rmoveto
		} ifelse
	} for
} def	% drawnums

/daynums {
	gsave
	/day		exch def
	/sofar		year month day doy def

	0 setgray
	eventfont isofindfont eventfontsize scalefont setfont
	1 eventfontsize rmoveto
	sofar 3 string cvs show
	(/) show

	365
	year isleap {
		1 add
	} if
	sofar sub 3 string cvs show

	grestore
} def	% daynums

/daynumsdouble {
	gsave
	/day		exch def
	/sofar		year month day doy def
	/left		365
			year isleap {
				1 add
			} if
			sofar sub def
	/thefont	eventfont isofindfont eventfontsize
                        3 mul 4 div scalefont
			def

	0 setgray
	thefont setfont

	currentpoint
	sofar 3 string cvs show
	( / ) show
	left 3 string cvs show
	moveto

	/fsize	datefontsize 2 div 2 add def
	dayfont isofindfont fsize scalefont setfont
	(24/) stringwidth pop 8 add fsize rmoveto

	thefont setfont

	sofar 7 add 3 string cvs show
	( / ) show
	left 7 sub 3 string cvs show

	grestore
} def	% daynumsdouble

/isdouble {				% overlay today with next/last week?
	days start add rows days_week mul gt
	{
		day start add rows days_week mul gt
		{
			true true
		}
		{
			day start add rows 1 sub days_week mul gt
			day days_week add days le and
			{
				false true
			}
			{
				false
			} ifelse
		} ifelse
	}
	{
		false
	} ifelse
} def	% isdouble

/prtdouble {
	gsave
	  dayfont isofindfont datefontsize 2 mul 3 div scalefont setfont
	  exch
	  {
		(23/) stringwidth pop dayheight rmoveto
		prtnum
	  }
	  {
		0 datefontsize 5 div rmoveto
		prtnum
		0 datefontsize -5 div rmoveto
		gsave
		  dayfont isofindfont datefontsize scalefont setfont
		  (/) show
		grestore
	  } ifelse
	grestore
} def	% prtdouble

/drawfill {				% place fill squares on calendar
	/start startday def
	/days year month ndays def
	currentpoint /y0 exch def /x0 exch def
	submonth
	{
		/fillstart 0 def
	}
	{
		usefirst
		{
			/fillstart 2 def
		}
		{
			/fillstart 0 def
		}
		ifelse
	} ifelse
	fillstart daywidth mul topgridmarg rmoveto
	1.0 setlinewidth
	fillstart 1 start 1 sub {
		gsave
		.9 setgray
		daywidth 0 rlineto
		0 dayheight neg rlineto
		daywidth neg 0 rlineto
		closepath fill
		grestore
		daywidth 0 rmoveto
		pop
	} for
	x0 y0 moveto
	submonth
	{
		/lastday rows 1 add days_week mul def
		days_week 1 sub daywidth mul -440 rmoveto
	}
	{
		/lastday rows days_week mul 2 sub fillstart add def
		days_week 3 sub fillstart add daywidth mul
		-440 dayheight add rmoveto
	} ifelse
	lastday -1 year month ndays start 1 add add
	{
		/day exch def
		gsave
		.9 setgray
		daywidth 0 rlineto
		0 dayheight neg rlineto
		daywidth neg 0 rlineto
		closepath fill
		grestore
		day weekday 1 eq
		{
			x0 y0 moveto
			days_week 1 sub daywidth mul -440 dayheight add rmoveto
		}
		{
			daywidth neg 0 rmoveto
		} ifelse
	} for
} def	% drawfill

/usefirst {				% are last two boxes used by days?
	start year month ndays add rows days_week mul 3 sub gt
	start 2 ge and
} def	% usefirst

/calendar {
	titlefont isofindfont titlefontsize scalefont setfont
	0 60 moveto
	/month_name month_names month 1 sub get def
	month_name show
	/yearstring year 10 string cvs def
	daywidth days_week mul yearstring stringwidth pop sub 60 moveto
	yearstring show

	eventflag {
		                  % Show a centered Banner if any at the Top
		daywidth days_week mul 2 div
		Bannerstring stringwidth pop 2 div sub
		60 moveto
		Bannerstring show
		                           % Show footnotes left-center-right
		eventfont isofindfont footfontsize scalefont setfont
		/bottomrow { dayheight rows mul 5 sub neg } def
		0 bottomrow moveto
		Lfootstring show
		daywidth days_week mul Rfootstring stringwidth pop sub
		bottomrow moveto
		Rfootstring show
		daywidth days_week mul Cfootstring stringwidth pop sub 2 div
		bottomrow moveto
		Cfootstring show
	} if

	0 -5 moveto
	drawnums

	0 -5 moveto
	drawfill

	eventflag {
		0 0 moveto
		drawevents
	} if

	0 -5 moveto
	drawgrid

	moonflag 0 ne {
		drawmoons
	} if
} def	% calendar

% ------------------------------------------------------------------------

/doy {	% year month day doy -> returns the number of the day of the year
	/theday		exch def
	/themonth	exch def
	/theyear	exch def
	days_sofar themonth 1 sub get
	theyear isleap
	themonth 2 gt and {
		1 add
	} if
	theday add
} def	% doy

/findphase {		% find the difference between any day and the reference
	/thisday	exch def	% day of the full moon
	/thismonth	exch def	% will probably be one off if the
					% reference is leap yr.
	/thisyear	exch def
	/daysdiff	thisyear thismonth thisday doy
			fullyear fullmonth fullday doy sub
			longer mul def	% try to be accurate about it
	/yearsdiff	thisyear fullyear sub def
	yearsdiff 0 ne {
		/daysdiff	daysdiff yearsdiff daysperyear mul
				yearsdiff 100 idiv yearsdiff 400 idiv
				sub sub add def
	} if
	daysdiff			% return difference in days
} def	% findphase

/shrink { 2 sqrt div } def

/bignum	1000 def

/transmogrify {
	bignum mul cvi		% take a real number and 'mod it down'
	period bignum mul cvi	% so it is in the range 0->period
	mod			% or -period->0
	bignum div		% the bignum preserves the accuracy
} def	% transmogrify.  Is there a better way to do this?

/domoon {			% draw the moon at the current phase
	/phase	exch def

	moonflag 2 eq {
		/phase period phase sub def
	} if

	0 0 radius		% might as well push these on now
	0 0 radius

	phase halfperiod lt
	{
		270 90 arc stroke                % line on right, fill on left
		0 radius neg moveto
		270 90 arcn
	}
	{
		90 270 arc stroke                % line on left, fill on right
		0 radius neg moveto
		270 90 arc
		/phase phase halfperiod sub def  % get rid of top halfperiod
	} ifelse

	/phase phase quartperiod sub	% scale it down to -r(root2) -> r(root2)
		rect mul def

	phase			% x1
	phase abs shrink	% y1 (need the abs!)
	phase			% x2
	phase abs shrink neg	% y2 (need the abs!)
	0			% x3
	radius neg		% y3
	curveto
	fill
} def	% domoon

/shiftdo {
	startphase day longer mul add
	transmogrify neg period add domoon
} def	% shiftdo

/drawmoons {
	/fullyear	1991 def	% These are the dates of a full moon,
	/fullmonth	11 def		% any date should work if it is that
	/fullday	21 def		% of a full moon, but I haven't tried
					% too many.

	% I wouldn't make this reference date fall in a leap year, wierdness
	% will probably happen in findphase. You will probably gain or lose a
	% day somewhere. MBH

	/period		29.5306 def
	/daysperyear	365.2422 def
	/longer		daysperyear 365 div def
	/halfperiod	period 2 div def
	/quartperiod	period 4 div def
	/radius		$DIAMETER def
	/rect		radius 2 sqrt mul quartperiod div def
	/startphase	year month 0 findphase transmogrify
			dup 0 lt { period add } if def
	/start		startday def
	/days		year month ndays def

	gsave
	0.1 setlinewidth
	newpath
	daywidth radius sub 3 sub 15 translate
	start daywidth mul 0 translate
	1 1 days {
		/day exch def
		isdouble
		{		% true if 30,31 -
				% false if 23,24 (left on stack)
			{
				gsave
				radius 2 div dayheight radius 2 div sub
					translate
				0.5 0.5 scale
				shiftdo
				grestore
			}
			{
				gsave
				radius 2 div neg radius 2 div translate
				0.5 0.5 scale
				shiftdo
				grestore
			} ifelse
		}
		{
			shiftdo
		} ifelse
		day start add 1 sub weekday 6 eq
		{
			daywidth 6 mul neg dayheight neg translate
		}
		{
			daywidth 0 translate
		} ifelse
	} for
	grestore
} def	% drawmoons

end

%%EndProlog
%%Page: 1 1

% ------------------------------------------------------------------------
%
% Main Program
%
% ------------------------------------------------------------------------
pscaldict begin

/titlefont /$TITLE_FONT def
/dayfont /$DAY_FONT def
/eventfont /$EVENT_FONT def

/holidays [ $holidays ] def

/Bannerstring ($BANNER) def

/Lfootstring ($LFOOT) def
/Rfootstring ($RFOOT) def
/Cfootstring ($CFOOT) def

% draw this stuff on the big calendar
/eventflag true def
/moonflag $MOON def
/daynumsflag $DAYNUMS def

gsave

/submonth false def

/month $MONTH def
/year $YEAR def
$SCALE scale
$ROTATE rotate
$TRANSLATE translate
calendar

% but don't draw the stuff on the small calendar
/eventflag false def
/moonflag 0 def
/daynumsflag false def

month 1 sub 0 eq
{
	/lmonth 12 def
	/lyear year 1 sub def
}
{
	/lmonth month 1 sub def
	/lyear year def
} ifelse

month 1 add 13 eq
{
	/nmonth 1 def
	/nyear year 1 add def
}
{
	/nmonth month 1 add def
	/nyear year def
} ifelse

usefirst
{
	0 30 translate
}
{
	days_week 2 sub daywidth mul -350 translate
}
ifelse

/submonth true def

% draw previous month small calendar
/year lyear def
/month lmonth def
gsave
.138 .138 scale
12 -120 translate
calendar
grestore

daywidth 0 translate

% draw next month small calendar
/year nyear def
/month nmonth def
gsave
.138 .138 scale
12 -120 translate
calendar
grestore

grestore

showpage
end
%%EOF
END-OF-CALENDAR
