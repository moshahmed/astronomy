// what: Generate Sun/Moon Rise/Set calendar for a given location, see usage.
// author: moshahmed/at/gmail 

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "AstroOps.h"
#include "DateOps.h"
#include "RiseSet.h"
#include "Lunar.h"
#include "ConfigFile.h"
#include "DarkCal.h"

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

//  "January", "February", "March",     "April",   "May",      "June",
//  "July",    "August",   "September", "October", "November", "December"
static const char* monthNames[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

static const char* weekDay[] = {
  "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
};

int verbose=0;

void set_cd_today(CalData &cd) {
  time_t ltime;
  struct tm *today;
  time( &ltime );
  today = localtime( &ltime );
  cd.day = today->tm_mday;
  cd.month = 1 + today->tm_mon;
  cd.year = 1900 + today->tm_year;
  if (verbose>0) {
    fprintf(stderr, "Using today's date %04d/%02d/%02d\n",
      cd.year, cd.month, cd.day);
  }
}

void set_cd(CalData &cd, ConfigFile &cf){

  if (cf.status() == cf.NO_ENTRIES )
    if ( ConfigFile::OK != cf.filename( "DarkCal.cfg" ) )
      exit(1);
  if ( cf.value( "longitude" ) )
    cd.loc.setLongitude( cf.dblValue( "longitude" ) );

  if ( cf.value( "latitude" ) )
    cd.loc.setLatitude( cf.dblValue( "latitude" ) );

  if ( cf.value( "timeZone" ) )
    cd.loc.setTimeZone( cf.dblValue( "timeZone" ) );

}

char* time_str5(double t, char* p) {
  if( t < 0.)
    sprintf( p, "--:--" );
  else {
    long minutes = long(t * 24. * 60. + .5); // round up to nearest minute
    sprintf( p, "%02d:%02d", int(minutes / 60), int(minutes % 60) );
  }
  return p;
}

void title_year(CalData &cd, ConfigFile &cf, int year){
  printf("\n");
  printf("Year %04d Calendar for %s\n"
         "Lat=%g %c, Lon=%g %c, TZ=%g\n",
    year, // cd.year,
    cf.value("city"),
    cd.loc.degLatitude(),   cd.loc.degLatitude()>0?  'N':'S',
    cd.loc.degLongitude(),  cd.loc.degLongitude()>0? 'W':'E',
    cd.loc.timeZone()
    );
}

void separator(void) {
  printf("+%3s+%4s-%2s-%2s+%5s+%5s+%5s+%5s+%5s+\n",
    "---", "----", "--", "--", "-----", "-----", "-----", "-----", "-----"); 
}

void title_month(int month, int year){
  printf("\n");
  printf("%s %4d/%d Monthly Sun Moon Calendar\n",
    monthNames[month-1], (int) year, month);
  separator();
  printf("|%3s|%4s %2s %2s|%5s|%5s|%5s|%5s|%5s|\n",
    "Day", "Date", "  ", "  ", "Sun  ", "Sun  ", "Moon ", "Moon ", "Moon ");
  printf("|%3s|%4s/%2s/%2s|%5s|%5s|%5s|%5s|%5s|\n",
    "day", "yyyy", "mm", "dd", "Rise ", "Set  ", "Rise ", "Set  ", "Phase"); 
  separator();
}

int main(int argc, char** argv) {  /* IF riser.exe */
  CalData cd;
  ConfigFile cf;
  set_cd_today(cd);
   
  // Default dates for calendar:
  int num_days = (int)(365.25*2.);
  double jd1 = DateOps::dmyToDay( 1, cd.month, cd.year ) ;
  double jd2 = jd1 + num_days;

  // Parse command line, eg. -from 2014/01/10 -to 2015/12/31
  //   '-days N'  is '-from 01/mm/yyyy -to $today+N'
  for(int i=1;i<argc;i++){ 
    if (!strcmp(argv[i],"--"))
      break;
    int year, month, day;
    if (!strcmp(argv[i],"-from") && (++i)<argc) {
      sscanf(argv[i], "%d/%d/%d", &year, &month, &day);
      jd1 = DateOps::dmyToDay( day, month, year ) ;
      continue;
    } 
    if (!strcmp(argv[i],"-to") && (++i)<argc) {
      sscanf(argv[i], "%d/%d/%d", &year, &month, &day);
      jd2 = DateOps::dmyToDay( day, month, year ) ;
      continue;
    }
    if (!strcmp(argv[i],"-today")) {
      jd1 = DateOps::dmyToDay( cd.day, cd.month, cd.year ) ;
      num_days=1;
      jd2 = jd1 + num_days;
      continue;
    }
    if (!strcmp(argv[i],"-days") && (++i)<argc) {
      sscanf(argv[i], "%d", &num_days);
      jd2 = jd1 + num_days;
      // fprintf(stderr,"num_days=%d\n",num_days);
      continue;
    }
    if (!strcmp(argv[i],"-lat") && (++i)<argc) {
      double d; sscanf(argv[i], "%lf", &d);
      cd.loc.setLatitude( d );
      continue;
    }
    if (!strcmp(argv[i],"-lon") && (++i)<argc) {
      double d; sscanf(argv[i], "%lf", &d);
      cd.loc.setLongitude( d );
      continue;
    }
    if (!strcmp(argv[i],"-tz") && (++i)<argc) {
      double d; sscanf(argv[i], "%lf", &d);
      cd.loc.setTimeZone( d );
      continue;
    }
    if (!strcmp(argv[i],"-city") && (++i)<argc) {
      cf.update("city", argv[i]);
      continue;
    }
    if (!strcmp(argv[i],"-cfg") && (++i)<argc) {
      cf.filename(argv[i]);
      set_cd(cd, cf);
      continue;
    }
    if ( !strcmp(argv[i],"-h") || !strcmp(argv[i],"-?")){
      fprintf(stderr, 
        "Usage: cmd -from 2014/01/01 -to 2014/12/31\n"
        "  -today  .. for today's sunrise sunset moonrise moonset\n"
        "  -days 1 .. for number of days\n"
        "  -cfg file.cfg .. read config file\n"
        "  -lat 12.887261 -lon 74.831657 -tz +5.5 .. latitude longitude timezone\n"
        "  -city Mangalore,India       .. location string\n"
        "  -h -? .. help \n"
        "  -v    .. verbose info\n"
        "  -debug\n"
        );
      exit(0);
    }
    if (!strcmp(argv[i],"-v")){
      verbose++;
      fprintf(stderr,"verbose=%d\n",verbose);
      continue;
    }
    if (!strcmp(argv[i],"-debug")){
      ConfigFile::debug=1;
      continue;
    }
    fprintf(stderr, "Cannot parse option argv[%d] %s\n", i, argv[i]);
    exit(1);
  }

  static const double hourFraction = 1./24.;
  double tzAdj = (double)cd.loc.timeZone() * hourFraction;

  int count=0;
  for (double jd=jd1; jd < jd2; jd+=1.) {
    long int year;
    int month, day;
    DateOps::dayToDmy( (long int) jd, &day, &month, &year);

    if ((month==1 && day==1) || (count==0))
      title_year(cd, cf, year);
    if (day==1 || (count==0))
      title_month(month, year);
    count++;
    TimePair sunRS, moonRS;

    static char srt[4][20];
    #define TP_RISE a
    #define TP_SET b

    double jdtz = jd -tzAdj -.5;
    RiseSet::getTimes( sunRS,  RiseSet::SUN,  jdtz, cd.loc );
    RiseSet::getTimes( moonRS, RiseSet::MOON, jdtz, cd.loc );

    printf("|%3s|%04d/%02d/%02d|%5s|%5s|%5s|%5s|%3d%% |\n",
      weekDay[((int)jd) % 7],
      (int)year, month, day,
      time_str5(sunRS.TP_RISE,srt[0]) , time_str5(sunRS.TP_SET,srt[1]),
      time_str5(moonRS.TP_RISE,srt[2]), time_str5(moonRS.TP_SET,srt[3]),
      (int)(Lunar::ageOfMoonInDays(jdtz)*100./29.53));
  }
  if (count>0)
    separator();
  return 0;
}
