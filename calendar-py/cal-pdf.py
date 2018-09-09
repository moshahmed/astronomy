# What: Awesome pdf calendar generator
# From http://www.mechanicalcat.net/richard/log/Python/Year_planner_PDF_generation_in_Python
# Dependency on reportlab pdf package
#    http://www.reportlab.com/software/opensource/rl-toolkit/download/
#    => reportlab-2.5.win32-py2.5.exe.gz

# rlcal.py v1.1 (C) 2005 Richard Jones
#   Generates 2 PDF pages for a "year planner".
#   License: http://creativecommons.org/licenses/by-nc-sa/2.0
#   Requires ReportLab PDF library http://www.reportlab.org/

# Uses ephem3 for sunrise/sunset/moon_phase

import calendar, sys, string
import datetime
import ephem, ephem3
from reportlab.lib import colors
from reportlab.graphics import shapes, renderPDF

# call calendar to calculate the days for each month
def get_months(year, startmon):
  mons = []
  reqd = 0
  for mon in range(6):
    weeks = calendar.monthcalendar(year, mon + startmon)
    weeks[-1] = [_f for _f in weeks[-1] if _f]
    mdays = []
    for week in weeks:
        mdays.extend(week)
    reqd = max(reqd, len(mdays))
    mons.append(mdays)
  return reqd, mons

def half_year(city, year, startmon):
    '''"startmon" gives the 1-indexed month to start the page on'''
    reqd, mons = get_months(year, startmon)

    LEFTMARGIN  = 5
    DAYCOLWIDTH = 50
    CELLWIDTH   = 80
    CELLHEIGHT  = 19
    TOPROWHEIGHT = 18
    WIDTH = LEFTMARGIN + DAYCOLWIDTH + CELLWIDTH*6
    HEIGHT = reqd * CELLHEIGHT + TOPROWHEIGHT

    d = shapes.Drawing(WIDTH, HEIGHT)

    # Month headings
    for i in range(6):
        x = LEFTMARGIN + i*CELLWIDTH + DAYCOLWIDTH + CELLWIDTH/2
        month_name = calendar.month_abbr[i + startmon]
        d.add(shapes.String(x, HEIGHT-14, month_name,
            fontSize=14, fontName='Times-Bold', textAnchor='middle'))

    # Day row headings
    for i in range(reqd):
        y = HEIGHT - i*CELLHEIGHT - TOPROWHEIGHT
        weekday_name = calendar.day_abbr[i%7]
        d.add(shapes.String(LEFTMARGIN + 10, y-14, weekday_name,
            fontSize=14))

    # Draw the day cells, for each month
    for j in range(6):
      x = LEFTMARGIN + j*CELLWIDTH + DAYCOLWIDTH
      # for each day
      for i in range(reqd):
        if i >= len(mons[j]) or not mons[j][i]:
          continue
        y = HEIGHT - i*CELLHEIGHT - TOPROWHEIGHT
        # cells for each day, light grey background if weekend
        weekend = i%7 > 4
        lightgrey = colors.HexColor(0xD0B090)
        color = weekend and lightgrey or colors.white

        # Now we have (x, y, color) for (year, month=j+startmon, day=mons[j][i])
        # Get the ephemerides for the date
        date = datetime.datetime(year, j+startmon, mons[j][i])
        (sunrise, sunset, moon_phase, moon_fm) = ephem3.ephem_one_day(city, date)

        # Insert the date cell at x, y
        d.add(shapes.Rect(x, y, CELLWIDTH, -CELLHEIGHT, fillColor=color))
        d.add(shapes.String(x+1, y-10, str(mons[j][i]), fontSize=10))
        green = colors.HexColor(0x207020)

        # Insert the moon phase
        if moon_fm:
          d.add(shapes.String(x+15, y-10, moon_fm, fontSize=8, fillColor=green))
      # for each day
    # for each month
    return d

if __name__ == '__main__':
    if len(sys.argv)>2:
      city_name = sys.argv[1]
      year = int(sys.argv[2])
    else:
      # today = datetime.datetime.date(datetime.datetime.now())
      # year = today.year
      # city_name = 'Accra'
      print("Usage: ", sys.argv[0], " city_name year")
      sys.exit(1)

    city = ephem3.get_city(city_name)
    outdir='out'
    outfile = '%s/calendar-%s-%d' % (outdir,city_name, year)
    print(("Writing %s.*.pdf" % (outfile)))
    renderPDF.drawToFile(half_year(city, year, 1), outfile + '-jan-jun.pdf')
    renderPDF.drawToFile(half_year(city, year, 7), outfile + '-jul-dec.pdf')
