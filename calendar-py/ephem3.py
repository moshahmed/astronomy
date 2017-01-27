#!C:/Python25/python
# What: Print calendar with full moon, sunrise, sunset for 1st of each month.
# From http://www.mannyjuan.com/calend2.txt
# Ephem integration GPL(C) mosh moshahmed/at/gmail
# $Header: c:/cvs/repo/mosh/python/calendar/ephem3.py,v 1.32 2017-01-27 08:38:57 a Exp $

USAGE = """USAGE:
  ephem3.py [-h] [-c style] [-l location] [-s start_date] [-e end_date] [-f file]
                 [-W width] [-h height]
                 [-E eventfile]
                 [-D days_delta]
      -c stlye      .. format is lines, monthly, yearly, all
      -D 10         .. print 10 lines of dates.
      -l location   .. format is city_name/latitude/longitude/elevation
      -s start_date .. format is YYYY/MM/DDDD
      -e end_date   .. format is YYYY/MM/DDDD
      -f file       .. write calendar to file,
                       -f dir/ to generate filenames.
      -E eventfile  .. file containing a list of dated events to print on calendar
                       eventfile line format is: 'date;description',
                       'YYYY/MM/DD;..' or 'M/D;..' or '# comment'
      -S subtitle   .. append subtitle to the monthly calendars

  For monthly calendar, the box size can be adjusted with:
      -W width      .. width of box
      -H height     .. extra height of box
  Example
  $ ephem3.py -c all -l "Mumbai;19;72.82;10;Asia/Kolkata" -s 2011/5/1/ -e 2012/5/2

  Prints yearly and monthly calendars
    For Mumbai at latitude 19N, longitude 72W, Elevation 10m
    For each year, month in the date range.
  Timezone: To see list of timezones matching a word, set timezone to /word, e.g.
  e.g. Location = 'Mumbai;19;73;0;/Asia'
  """

import types, time, datetime, ephem, sys, re, getopt, pytz, string
import cities

# Exception raised for bad input (with string parameter for details)
error = 'calendar.error'

fout = sys.__stdout__

subtitle = '' # 'http//hmi-tech.net'

def print_fout_nospace(str):
  fout.write(str)

def warn(str):
  sys.stderr.write('%s\n' % str)

January = 1
February = 2
day_in_month = [0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]

day_name = [
  # Monday is first day of the week (European), unlike unix cal(1)
  'Monday', 'Tuesday', 'Wednesday', 'Thursday',
  'Friday', 'Saturday', 'Sunday']

day_abbr = [
  'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun' ]

month_name = [ '',
  # Full and abbreviated names of months (1-based arrays)
  'January',   'February', 'March',    'April',
  'May',       'June',     'July',     'August',
  'September', 'October',  'November', 'December']

month_abbr = [ '   ',
  # Full and abbreviated names of months (1-based arrays)
  'Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun',
  'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']

def isleap(year):
  # Return 1 for leap years, 0 for non-leap years
  return year % 4 == 0 and (year % 100 <> 0 or year % 400 == 0)

def leap_days(y1, y2):
  # Return number of leap years in range [y1, y2)
  # Assume y1 <= y2 and no non-leap century years.
  return (y2+3)/4 - (y1+3)/4

def weekday(year, month, day):
  # Return weekday (0-6 ~ Mon-Sun) for year (1970-...), month (1-12), day (1-31)
  secs = time.mktime((year, month, day, 0, 0, 0, 0, 0, 0))
  tuple = time.localtime(secs)
  return tuple[6]

def month_range(year, month, begweek):
  # Return the weekday (0-6 ~ Mon-Sun) of (year, month, 1)
  # and number of days (28-31) in the month of (year, month)
  if not 1 <= month <= 12:
    raise ValueError, 'bad month number'
  day1 = (weekday(year, month, 1) + 1 - begweek) % 7
  ndays = day_in_month[month] + (month == February and isleap(year))
  return day1, ndays

planet_info_lines = 6

def _month_calendar(city, year, month, begweek):
  # Main function creating a month's calendar.
  # Return a matrix representing a month's calendar
  # Each row represents a week; days outside this month are zero
  # Each cell is a string
  # if city is provided, add extra rows for planet info.
  day1, ndays = month_range(year, month, begweek)
  rows = []
  r7 = range(7)
  day = 1 - day1
  while day <= ndays:
    row = ['']*7
    # planet info
    pi_moon = ['']*7
    pi_name = ['']*7
    pi_constel = ['']*7
    pi_rise = ['']*7
    pi_set = ['']*7
    pi_event = ['']*7
    for i in r7:
      if 1 <= day <= ndays:
        row[i] = `day`

        # Annotate (city, year, month, day) here.
        if city:
          date = datetime.datetime(year, month, day)

          # Each weekday we will process and print one of the planets.
          if   i==0: planet = ephem.Sun()
          elif i==1: planet = ephem.Mercury()
          elif i==2: planet = ephem.Mars()
          elif i==3: planet = ephem.Venus()
          elif i==4: planet = ephem.Jupiter()
          elif i==5: planet = ephem.Moon()
          elif i==6: planet = ephem.Saturn()
          planet.compute(date)
          pi_name[i] = planet.name + ' in'

          # Find the constel of the planet
          constel = ephem.constellation(planet)[1]
          if len(constel) > 10:
            constel = constel[:10]
          pi_constel[i] = constel

          rise = city.next_rising(planet, start=date).datetime()
          sett = city.next_setting(planet, start=date).datetime()

          # convert rise/set to localtime
          rise = pytz.utc.localize(rise).astimezone(city.pytz)
          sett = pytz.utc.localize(sett).astimezone(city.pytz)

          pi_rise[i] = "Rise %02d:%02d" % (rise.hour, rise.minute)
          pi_set[i]  = "Set  %02d:%02d" % (sett.hour, sett.minute)

          # Moon phases (FM, NM, LQ, FQ)
          moon_phase = moon_is_full(date)
          if moon_phase:
            pi_moon[i] = moon_phase
          # TODO look for solstice equinox

          evdate = "%04d/%02d/%02d" % (year, month, day)
          if events.has_key(evdate):
            pi_event[i] += events[evdate]
          evdate = "%02d/%02d" % (month, day)
          if events.has_key(evdate):
            pi_event[i] += events[evdate]

      day = day + 1
      # next day
    rows.append(row)
    if city:
      rows.append(pi_name)
      rows.append(pi_constel)
      rows.append(pi_rise)
      rows.append(pi_set)
      rows.append(pi_moon)
      rows.append(pi_event)
      # we added planet_info_lines = 6
    # next week
  # next month
  return rows

_mc_cache = {}
def month_calendar(city, year, month, begweek):
  # Caching interface to _month_calendar
  key = (city, year, month, begweek)
  if _mc_cache.has_key(key):
    return _mc_cache[key]
  else:
    _mc_cache[key] = ret = _month_calendar(city, year, month, begweek)
    return ret

def _center(str, width, padon='both'):
  pad = width - len(str)
  if pad < 0:
    warn("_center '%s' cannot fit in width=%d, pad=%d" % (
      str, width, pad))
    str = str[0:width]
  if pad <= 0:
    return str
  if padon == 'both': # center
    return ' '*((pad+1)/2) + str + ' '*((pad)/2)
  elif padon == 'left':
    return ' '*pad + str
  else:
    return str+' '*pad

def print_week(week, width, border=' ', padon='both'):
  # Print a single week (no newline)
  if border and border != ' ':
    print_fout_nospace(border)
  for day in week:
    print_fout_nospace(_center(day, width, padon)+border)

def week_header(width, begweek, border=' '):
  str = ''
  if width >= 9:
      names = day_name
  else:
      names = day_abbr
  for i in range(7):
    if str:
      str = str + border
    str = str + _center(names[(i+begweek+6)%7][:width], width)
  if border and border != ' ':
    str = border + str + border
  return str

def print_month(city, year, month, begweek=0, width = 0, extra_height=2):
  width = max(2, width)
  title = month_name[month] + ' ' + `year` + ' - ' + city.name
  if subtitle:
    title = title + ', ' + subtitle
  print >> fout, _center(title, 7*(width+1) - 1),
  print >> fout
  print_week([('-'*(width))]*7, width, '+'); print >> fout
  print >> fout, week_header(width, begweek, '|'), ; print >> fout
  rows = month_calendar(city, year, month, begweek)
  while len(rows)>0:
    week, rows = rows[0], rows[1:]

    print_week([('-'*(width))]*7, width, '+'); print >> fout
    print_week(week, width, '|', 'right'); print >> fout
    if city:
      for i in range(planet_info_lines):
        pi_info, rows = rows[0], rows[1:]
        print_week(pi_info, width, '|', 'right'); print >> fout

    for i in range(extra_height):
      print_week(['']*7, width, '|'); print >> fout
  print_week([('-'*(width))]*7, width, '+'); print >> fout

# Spacing of month columns
_day_width = 2
_col_width = 7*(_day_width+1) - 1      # Amount printed by print_week()
_col_pad = '|'                         # Pad between columns
_right_end = '|'

def format_3_column(a, b, c):
  # 3-column formatting for year calendars
  print >> fout, _col_pad + _center(a, _col_width),
  print >> fout, _col_pad + _center(b, _col_width),
  print >> fout, _col_pad + _center(c, _col_width),
  print >> fout, _right_end

def monthly_ephem(city, year, month):
  # Return a short string containing full_moon day
  # and sun rise, sun set for (city, date=(year, month, 1)).
  # moon_phase is 0..29, 0 new moon, 15 is new moon.
  # moon_fm is 'FM', 'NM' etc for the given date.
  date = datetime.datetime(year, month, 1)
  next_full_moon = ephem.next_full_moon(date).datetime()
  (sunrise, sunset, moon_phase, moon_fm) = ephem_one_day(city, date)
  # moon_phase is not used, only next_full_moon date is printed.
  return "M:%2d Sun:%02d:%02d-%02d:%02d" % (
    next_full_moon.day,
    sunrise.hour, sunrise.minute,
    sunset.hour, sunset.minute)

def print_year_calendar(city, year, begweek=0):
  # +----------+----------+----------+----------+----------+----------+----------+
  # |  Sunday  |  Monday  |  Tuesday | Wednesday| Thursday |  Friday  | Saturday |
  head = '+' + ((('-'*(_col_width+1))+'+')*3)
  header = week_header(_day_width, begweek)

  print >> fout, head

  # |          -          | 2011 Mumbai, India  |          -          |
  format_3_column('-', `year` + ' ' + city.name, '-')

  for q in range(January, January+12, 3):

    ### Print month header, (23, 23, 23) wide columns.
    print >> fout, head

    # |       January       |      February       |        March        |
    # |Su Mo Tu We Th Fr Sa |Su Mo Tu We Th Fr Sa |Su Mo Tu We Th Fr Sa |
    format_3_column(month_name[q], month_name[q+1], month_name[q+2])
    format_3_column(header, header, header)
    data = []
    height = 0

    # print 3 months at a time into 'data'
    for month in range(q, q+3):
      cal = month_calendar('', year, month, begweek)
      if len(cal) > height:
        height = len(cal)
      data.append(cal)

    # print data
    # e.g. data for 2011 =
    #    [[[0, 0, 0, 0, 0, 0, 1], [2, 3, 4, 5, 6, 7, 8],
    #      [9, 10, 11, 12, 13, 14, 15], [16, 17, 18, 19, 20, 21, 22],
    #      [23, 24, 25, 26, 27, 28, 29], [30, 31, 0, 0, 0, 0, 0]],
    #     [[0, 0, 1, 2, 3, 4, 5], [6, 7, 8, 9, 10, 11, 12],
    #      [13, 14, 15, 16, 17, 18, 19], [20, 21, 22, 23, 24, 25, 26],
    #      [27, 28, 0, 0, 0, 0, 0]],
    #     [[0, 0, 1, 2, 3, 4, 5],
    #      [6, 7, 8, 9, 10, 11, 12], [13, 14, 15, 16, 17, 18, 19],
    #      [20, 21, 22, 23, 24, 25, 26], [27, 28, 29, 30, 31, 0, 0]]]

    # print each line 0..height-1
    for i in range(height):
      print >> fout, _col_pad,
      for cal in data:
        if i >= len(cal):
          # blank fill the empty week in a month
          print >> fout, ' '*(_col_width-1),
        else:
          print_week(cal[i], _day_width)
        print >> fout, _col_pad,
      print >> fout

    # Moon sunrise sunset info for city for 3 months, eg.
    # |M:19 Sun:07:11-18:12 |M:18 Sun:07:12-18:31 |M:19 Sun:06:58-18:44 |
    # where M:19 means, next_full_moon is on 19th of the month,
    #  and  Sun:sunrise-sunset in localtime.
    format_3_column(
      monthly_ephem(city, year, q),
      monthly_ephem(city, year, q+1),
      monthly_ephem(city, year, q+2))

  print >> fout, head

def edate_rounded(edate):
  year, month, day = edate.triple()
  return ephem.Date("%04d/%02d/01" % (year, month))

# events is a hash map: date -> description
events = { }

def read_events(event_filename):
  # event_filename consist of sequence of lines:
  #   # comment
  #   date ; description
  # date format: YYYY/MM/DD or MM/DD
  fin = open(event_filename)
  for line in fin:
    if re.match('^#',line):
      continue
    date, event = line.split(';', 2)
    date, event = date.strip(), event.strip()
    # YYYY/MM/DD or M/D
    if re.match('(\d{4}/)?\d{2}/\d{2}', date):
      # TODO: validate the date
      if events.has_key(date):
        events[date] += ',' + event
      else:
        events[date] = event
    else:
      print "Cannot parse date ", date
      sys.exit(1)
    # print '"%s" -- "%s"' % (date, events[date])
  fin.close()

# ephem2.py
def cmp_ymd(d1, d2):
  " Compare two ephem dates for approx equality."
  return abs(float(d1)-float(d2)) < 1

def moon_is(adate):
  " Find the next moon shape, returns FM .. NM."
  # TODO(mohsin) This is expensive, should cache it for whole month.
  # ephem.previous_new_moon(date)
  # ephem.previous_first_quarter_moon(date)
  # ephem.previous_full_moon(date)
  # ephem.previous_last_quarter_moon(date)
  date = ephem.Date(adate)
  if cmp_ymd(ephem.next_new_moon(date)          , date): return 'NM'
  if cmp_ymd(ephem.next_first_quarter_moon(date), date): return 'FQ'
  if cmp_ymd(ephem.next_full_moon(date)         , date): return 'FM'
  if cmp_ymd(ephem.next_last_quarter_moon(date) , date):  return 'LQ'
  return ''

def moon_is_full(date):
  "Return phase of the moon"
  m = moon_is(date)
  if m == 'NM': return 'Moon New'
  if m == 'FQ': return 'Moon FQ'
  if m == 'FM': return 'Moon Full'
  if m == 'LQ': return 'Moon LQ'
  return ''

def full_moon_from_to(start_date, end_date):
  "Print the full moons"
  # Not used,
  # TODO: compute lunar eclipse at full moon
  #       and solar eclipse on new moon.
  date = start_date
  next_day = datetime.timedelta(days=1)
  while date < end_date:
      fm = ephem.next_full_moon(date).datetime()
      print "full moon on %04d-%02d-%02d" % (
        fm.year, fm.month, fm.day)
      date = fm + next_day

def ephem_one_day(city, date):
  city.date = date
  moon = ephem.Moon()
  sun = ephem.Sun()
  moon.compute(date, epoch='2000')

  sunrise = city.next_rising(sun).datetime()
  sunset = city.next_setting(sun).datetime()

  # convert rise/set to localtime
  sunrise = pytz.utc.localize(sunrise).astimezone(city.pytz)
  sunset = pytz.utc.localize(sunset).astimezone(city.pytz)

  moon_phase = int(moon.moon_phase*30),
  moon_fm = moon_is(date)

  return (sunrise, sunset, moon_phase, moon_fm)

def calendar_from_to(city, start_date, end_date, braces):
  " Print the calendar for each day in city"
  date = start_date
  line = 0
  next_day = datetime.timedelta(days=1)
  while date < end_date:
    line = line+1
    yesterday = date
    # new month or new year?
    if braces and (date.day == 1 or line == 1):
      if line > 1:
        print "}}}", yesterday.strftime("%b")
      else:
        print "# Calendar for %s from %s-%s-%s to %s-%s-%s" % (
          city.name,
          start_date.year, start_date.month, start_date.day,
          end_date.year, end_date.month, end_date.day)
      # new year?
      if date.month == 1:
        if line > 1: print "}}}", yesterday.strftime("%Y")
        print date.strftime("%Y"), "{{{"
      # new month?
      print date.strftime("%b %Y"), "{{{"

    # Get and print the data for city
    (sunrise, sunset, moon_phase, moon_fm) = ephem_one_day(city, date)

    weekday = date.strftime("%a")

    if moon_fm:
      moonx = moon_fm
    else:
      moonx = "%2d" % moon_phase


    print '%04d-%02d-%02d %3s M:%2s SR:%02d:%02d SS:%02d:%02d' % (
      date.year, date.month, date.day, weekday,
      moonx,
      sunrise.hour, sunrise.minute,
      sunset.hour, sunset.minute
      )

    date += next_day
  if braces:
    print "}}}\n# vim:fdm=marker:fen: "

def get_timezone(tzname):
  try:
    return pytz.timezone(tzname)
  except pytz.UnknownTimeZoneError,zone:
    print "UnknownTimeZoneError",zone
    if re.match('^/', tzname):
      maybe_tz = filter(lambda x: re.search(tzname[1:],x, re.IGNORECASE),
        pytz.all_timezones)
      print 'Matching timezones are: ', maybe_tz
    else:
      print 'See pytz.all_timezones, supply /regexp, eg. /US'
    sys.exit(1)

class city_info(ephem.Observer): # Observer + { name, pytz }
  pass


def get_city(city):
  "Return city_info object"
  if  cities.locations.has_key(city):
    city = cities.locations[city]

  city = re.sub( r'\s+','', city) # trim all spaces
  m = city.split(';', 5)
  if len(m) == 5:
    obj = city_info() # ephem.Observer()
    (obj.name, obj.lat, obj.long, elev, tzname) = m
    obj.elev = int(elev)
    obj.pytz = get_timezone(tzname)
    obj.description = "Location: %s, lat=%s, lon=%s, elev=%d, tz=%s" % (
      obj.name, obj.lat, obj.long, obj.elev, obj.pytz.zone)
    return obj

  print ValueError, "Unknown city: '" + city + """'
    Location format is 'City Name;Latitude;Longitude;Elevation;TimeZone'
    Examples: 'Accra;5;0;0;Africa/Accra' or 'SF;37;-122;30;US/Pacific'
    """
  exit(1)


if __name__ == '__main__':
  opt_help = 0
  opt_style = 'lines'
  opt_file = ''
  edate1 = edate2 = location = ''

  opt_width = 10
  opt_height = 2
  days_delta = 1

  opts, args = getopt.getopt(sys.argv[1:], 'hc:l:s:e:f:W:H:E:S:D:')
  for opt in opts:
    if   opt[0] == '-h': opt_help = 1
    elif opt[0] == '-c': opt_style = opt[1]
    elif opt[0] == '-l': location = opt[1]
    elif opt[0] == '-s': edate1 = ephem.Date(opt[1])
    elif opt[0] == '-e': edate2 = ephem.Date(opt[1])
    elif opt[0] == '-f': opt_file = opt[1]
    elif opt[0] == '-W': opt_width = string.atoi(opt[1])
    elif opt[0] == '-H': opt_height = string.atoi(opt[1])
    elif opt[0] == '-E': read_events(opt[1])
    elif opt[0] == '-S': subtitle = opt[1]
    elif opt[0] == '-D': days_delta = string.atoi(opt[1])

  if opt_help or not opt_style :
    print USAGE
    sys.exit(0)

  # Default location and time
  if not location:
    location = 'Mangalore'
  if not edate1:
    today = datetime.datetime.date(datetime.datetime.now())
    edate1 = ephem.Date("%04d/%02d/%02d" % (today.year, today.month, 1))
  if not edate2:
    edate2 = edate1

  # Ready, round up the dates and location.
  city = get_city(location)
  edate1 = edate_rounded(edate1)
  edate2 = edate_rounded(edate2)

  # Swap dates if user entered in wrong order
  if edate2 < edate1:
    edate1, edate2 = edate2, edate1

  # Convert dates to datetime for iterating over years, months
  start_date = edate1.datetime()
  end_date = edate2.datetime()

  date_out = '%04d/%02d' % (start_date.year, start_date.month)
  if edate2 > edate1:
    date_out += ' to %04d/%02d' % (end_date.year, end_date.month)

  # out file name
  if opt_file:
    if opt_file[-1] == '/':
      generated_name = 'planetary-calendar-%s-%s.txt' % (city.name, date_out)
      generated_name = re.sub('[/\s]+', '-', generated_name)
      opt_file += generated_name
    opt_file = re.sub(r'[^\w_.//\\:]+', '-', opt_file)
    warn('Writing to %s' % opt_file)
    fout = open(opt_file, "w")


  # Print linear calendar
  if opt_style == 'lines' or opt_style == 'all':
    end_date = start_date + datetime.timedelta(days_delta)
    calendar_from_to(city, start_date, end_date, days_delta > 1)

  # Print yearly calendar
  if opt_style == 'yearly' or opt_style == 'all':
    print >> fout, 'Planetary Calendar for %s from %s' % (city.name, date_out)
    print >> fout, city.description
    print >> fout
    for year in range(start_date.year, end_date.year+1):
      warn('Generating yearly calendar for %04d, where M:next_full_moon.day' % (year))
      print_year_calendar(city, year)
      print >> fout, "\f"

  # Print monthly calendar
  if opt_style == 'monthly' or opt_style == 'all':
    print >> fout, 'Planetary Calendar for %s from %s' % (city.name, date_out)
    print >> fout, city.description
    print >> fout
    for year in range(start_date.year, end_date.year+1):
      for month in range(1, 12+1):
        edate3 = ephem.Date("%04d/%02d/%02d" % (year, month, 1))
        if edate1 <= edate3 <= edate2:
          warn('Generating monthly calendar for %04d-%02d' % (year, month))
          print_month(city, year, month, 0, opt_width, opt_height)
          print >> fout, "\f"

  print >> fout, "GPL(C) moshahmed/at/gmail, ephem3.py vim:nowrap:"
  if opt_file:
    fout.close()
    warn('Wrote %s' % opt_file)
  warn('Done')

