#include <stdio.h>
#include "screen.h"

/* define BANG if and only if your system supports the system() function.
 */
#define	BANG

/* table of the fields, with flags indicating which menu(s) they are on and
 * whether pickable for changing or plotting.
 * N.B. type must be long enough to hold 16 bits.
 */
typedef unsigned short F_t;
static F_t fields[] = {
    rcfpack (R_ALTM,	C_ALTM,		F_MMNU|F_CHG),
    rcfpack (R_CALLISTO,C_JMX,		F_MNUJ|F_PLT),
    rcfpack (R_CALLISTO,C_JMY,		F_MNUJ|F_PLT),
    rcfpack (R_CALLISTO,C_JMZ,		F_MNUJ|F_PLT),
    rcfpack (R_DAWN,	C_DAWN,		F_MMNU|F_CHG),
    rcfpack (R_DAWN,	C_DAWNV,	F_MMNU|F_PLT),
    rcfpack (R_DUSK,	C_DUSK,		F_MMNU|F_CHG),
    rcfpack (R_DUSK,	C_DUSKV,	F_MMNU|F_PLT),
    rcfpack (R_EPOCH,	C_EPOCHV,	F_MMNU|F_CHG),
    rcfpack (R_EUROPA,	C_JMX,		F_MNUJ|F_PLT),
    rcfpack (R_EUROPA,	C_JMY,		F_MNUJ|F_PLT),
    rcfpack (R_EUROPA,	C_JMZ,		F_MNUJ|F_PLT),
    rcfpack (R_GANYMEDE,C_JMX,		F_MNUJ|F_PLT),
    rcfpack (R_GANYMEDE,C_JMY,		F_MNUJ|F_PLT),
    rcfpack (R_GANYMEDE,C_JMZ,		F_MNUJ|F_PLT),
    rcfpack (R_HEIGHT,	C_HEIGHTV,	F_MMNU|F_CHG|F_PLT),
    rcfpack (R_IO,	C_JMX,		F_MNUJ|F_PLT),
    rcfpack (R_IO,	C_JMY,		F_MNUJ|F_PLT),
    rcfpack (R_IO,	C_JMZ,		F_MNUJ|F_PLT),
    rcfpack (R_JCML,	C_JCMLSI,	F_MNUJ|F_PLT),
    rcfpack (R_JCML,	C_JCMLSII,	F_MNUJ|F_PLT),
    rcfpack (R_JD,	C_JDV,		F_MMNU|F_CHG|F_PLT),
    rcfpack (R_JUPITER,	C_ALT,		F_MNU1|F_PLT),
    rcfpack (R_JUPITER,	C_AZ,		F_MNU1|F_PLT),
    rcfpack (R_JUPITER,	C_CONSTEL,	F_MNU1|F_CHG),
    rcfpack (R_JUPITER,	C_CONSTEL,	F_MNU2|F_CHG),
    rcfpack (R_JUPITER,	C_CONSTEL,	F_MNU3|F_CHG),
    rcfpack (R_JUPITER,	C_DEC,		F_MNU1|F_PLT),
    rcfpack (R_JUPITER,	C_EDIST,	F_MNU1|F_PLT),
    rcfpack (R_JUPITER,	C_ELONG,	F_MNU1|F_PLT),
    rcfpack (R_JUPITER,	C_HLAT,		F_MNU1|F_PLT),
    rcfpack (R_JUPITER,	C_HLONG,	F_MNU1|F_PLT),
    rcfpack (R_JUPITER,	C_MAG,		F_MNU1|F_PLT),
    rcfpack (R_JUPITER,	C_MARS,		F_MNU3|F_PLT),
    rcfpack (R_JUPITER,	C_MERCURY,	F_MNU3|F_PLT),
    rcfpack (R_JUPITER,	C_MOON,		F_MNU3|F_PLT),
    rcfpack (R_JUPITER,	C_NEPTUNE,	F_MNU3|F_PLT),
    rcfpack (R_JUPITER,	C_OBJ,		F_MNU1|F_CHG),
    rcfpack (R_JUPITER,	C_OBJ,		F_MNU2|F_CHG),
    rcfpack (R_JUPITER,	C_OBJ,		F_MNU3|F_CHG),
    rcfpack (R_JUPITER,	C_OBJX,		F_MNU3|F_PLT),
    rcfpack (R_JUPITER,	C_OBJY,		F_MNU3|F_PLT),
    rcfpack (R_JUPITER,	C_PHASE,	F_MNU1|F_PLT),
    rcfpack (R_JUPITER,	C_PLUTO,	F_MNU3|F_PLT),
    rcfpack (R_JUPITER,	C_RA,		F_MNU1|F_PLT),
    rcfpack (R_JUPITER,	C_RISEAZ,	F_MNU2|F_PLT),
    rcfpack (R_JUPITER,	C_RISETM,	F_MNU2|F_PLT),
    rcfpack (R_JUPITER,	C_SATURN,	F_MNU3|F_PLT),
    rcfpack (R_JUPITER,	C_SDIST,	F_MNU1|F_PLT),
    rcfpack (R_JUPITER,	C_SETAZ,	F_MNU2|F_PLT),
    rcfpack (R_JUPITER,	C_SETTM,	F_MNU2|F_PLT),
    rcfpack (R_JUPITER,	C_SIZE,		F_MNU1|F_PLT),
    rcfpack (R_JUPITER,	C_SUN,		F_MNU3|F_PLT),
    rcfpack (R_JUPITER,	C_TRANSALT,	F_MNU2|F_PLT),
    rcfpack (R_JUPITER,	C_TRANSTM,	F_MNU2|F_PLT),
    rcfpack (R_JUPITER,	C_TUP,		F_MNU2|F_PLT),
    rcfpack (R_JUPITER,	C_URANUS,	F_MNU3|F_PLT),
    rcfpack (R_JUPITER,	C_VENUS,	F_MNU3|F_PLT),
    rcfpack (R_JUPITER,	C_XTRA,		F_MNU1|F_CHG),
    rcfpack (R_JUPITER,	C_XTRA,		F_MNU2|F_CHG),
    rcfpack (R_JUPITER,	C_XTRA,		F_MNU3|F_CHG),
    rcfpack (R_LAT,	C_LATV,		F_MMNU|F_CHG|F_PLT),
    rcfpack (R_LD,	C_LD,		F_MMNU|F_PLT|F_CHG),
    rcfpack (R_LISTING,	C_LISTING,	F_MMNU|F_CHG),
    rcfpack (R_LON,	C_LON,		F_MMNU|F_CHG),
    rcfpack (R_LON,	C_LONV,		F_MMNU|F_PLT),
    rcfpack (R_LONG,	C_LONGV,	F_MMNU|F_CHG|F_PLT),
    rcfpack (R_LST,	C_LSTV,		F_MMNU|F_CHG|F_PLT),
    rcfpack (R_LT,	C_LT,		F_MMNU|F_CHG|F_PLT),
    rcfpack (R_MARS,	C_ALT,		F_MNU1|F_PLT),
    rcfpack (R_MARS,	C_AZ,		F_MNU1|F_PLT),
    rcfpack (R_MARS,	C_CONSTEL,	F_MNU1|F_CHG),
    rcfpack (R_MARS,	C_CONSTEL,	F_MNU2|F_CHG),
    rcfpack (R_MARS,	C_CONSTEL,	F_MNU3|F_CHG),
    rcfpack (R_MARS,	C_DEC,		F_MNU1|F_PLT),
    rcfpack (R_MARS,	C_EDIST,	F_MNU1|F_PLT),
    rcfpack (R_MARS,	C_ELONG,	F_MNU1|F_PLT),
    rcfpack (R_MARS,	C_HLAT,		F_MNU1|F_PLT),
    rcfpack (R_MARS,	C_HLONG,	F_MNU1|F_PLT),
    rcfpack (R_MARS,	C_JUPITER,	F_MNU3|F_PLT),
    rcfpack (R_MARS,	C_MAG,		F_MNU1|F_PLT),
    rcfpack (R_MARS,	C_MERCURY,	F_MNU3|F_PLT),
    rcfpack (R_MARS,	C_MOON,		F_MNU3|F_PLT),
    rcfpack (R_MARS,	C_NEPTUNE,	F_MNU3|F_PLT),
    rcfpack (R_MARS,	C_OBJ,		F_MNU1|F_CHG),
    rcfpack (R_MARS,	C_OBJ,		F_MNU2|F_CHG),
    rcfpack (R_MARS,	C_OBJ,		F_MNU3|F_CHG),
    rcfpack (R_MARS,	C_OBJX,		F_MNU3|F_PLT),
    rcfpack (R_MARS,	C_OBJY,		F_MNU3|F_PLT),
    rcfpack (R_MARS,	C_PHASE,	F_MNU1|F_PLT),
    rcfpack (R_MARS,	C_PLUTO,	F_MNU3|F_PLT),
    rcfpack (R_MARS,	C_RA,		F_MNU1|F_PLT),
    rcfpack (R_MARS,	C_RISEAZ,	F_MNU2|F_PLT),
    rcfpack (R_MARS,	C_RISETM,	F_MNU2|F_PLT),
    rcfpack (R_MARS,	C_SATURN,	F_MNU3|F_PLT),
    rcfpack (R_MARS,	C_SDIST,	F_MNU1|F_PLT),
    rcfpack (R_MARS,	C_SETAZ,	F_MNU2|F_PLT),
    rcfpack (R_MARS,	C_SETTM,	F_MNU2|F_PLT),
    rcfpack (R_MARS,	C_SIZE,		F_MNU1|F_PLT),
    rcfpack (R_MARS,	C_SUN,		F_MNU3|F_PLT),
    rcfpack (R_MARS,	C_TRANSALT,	F_MNU2|F_PLT),
    rcfpack (R_MARS,	C_TRANSTM,	F_MNU2|F_PLT),
    rcfpack (R_MARS,	C_TUP,		F_MNU2|F_PLT),
    rcfpack (R_MARS,	C_URANUS,	F_MNU3|F_PLT),
    rcfpack (R_MARS,	C_VENUS,	F_MNU3|F_PLT),
    rcfpack (R_MERCURY,	C_ALT,		F_MNU1|F_PLT),
    rcfpack (R_MERCURY,	C_AZ,		F_MNU1|F_PLT),
    rcfpack (R_MERCURY,	C_CONSTEL,	F_MNU1|F_CHG),
    rcfpack (R_MERCURY,	C_CONSTEL,	F_MNU2|F_CHG),
    rcfpack (R_MERCURY,	C_CONSTEL,	F_MNU3|F_CHG),
    rcfpack (R_MERCURY,	C_DEC,		F_MNU1|F_PLT),
    rcfpack (R_MERCURY,	C_EDIST,	F_MNU1|F_PLT),
    rcfpack (R_MERCURY,	C_ELONG,	F_MNU1|F_PLT),
    rcfpack (R_MERCURY,	C_HLAT,		F_MNU1|F_PLT),
    rcfpack (R_MERCURY,	C_HLONG,	F_MNU1|F_PLT),
    rcfpack (R_MERCURY,	C_JUPITER,	F_MNU3|F_PLT),
    rcfpack (R_MERCURY,	C_MAG,		F_MNU1|F_PLT),
    rcfpack (R_MERCURY,	C_MARS,		F_MNU3|F_PLT),
    rcfpack (R_MERCURY,	C_MOON,		F_MNU3|F_PLT),
    rcfpack (R_MERCURY,	C_NEPTUNE,	F_MNU3|F_PLT),
    rcfpack (R_MERCURY,	C_OBJ,		F_MNU1|F_CHG),
    rcfpack (R_MERCURY,	C_OBJ,		F_MNU2|F_CHG),
    rcfpack (R_MERCURY,	C_OBJ,		F_MNU3|F_CHG),
    rcfpack (R_MERCURY,	C_OBJX,		F_MNU3|F_PLT),
    rcfpack (R_MERCURY,	C_OBJY,		F_MNU3|F_PLT),
    rcfpack (R_MERCURY,	C_PHASE,	F_MNU1|F_PLT),
    rcfpack (R_MERCURY,	C_PLUTO,	F_MNU3|F_PLT),
    rcfpack (R_MERCURY,	C_RA,		F_MNU1|F_PLT),
    rcfpack (R_MERCURY,	C_RISEAZ,	F_MNU2|F_PLT),
    rcfpack (R_MERCURY,	C_RISETM,	F_MNU2|F_PLT),
    rcfpack (R_MERCURY,	C_SATURN,	F_MNU3|F_PLT),
    rcfpack (R_MERCURY,	C_SDIST,	F_MNU1|F_PLT),
    rcfpack (R_MERCURY,	C_SETAZ,	F_MNU2|F_PLT),
    rcfpack (R_MERCURY,	C_SETTM,	F_MNU2|F_PLT),
    rcfpack (R_MERCURY,	C_SIZE,		F_MNU1|F_PLT),
    rcfpack (R_MERCURY,	C_SUN,		F_MNU3|F_PLT),
    rcfpack (R_MERCURY,	C_TRANSALT,	F_MNU2|F_PLT),
    rcfpack (R_MERCURY,	C_TRANSTM,	F_MNU2|F_PLT),
    rcfpack (R_MERCURY,	C_TUP,		F_MNU2|F_PLT),
    rcfpack (R_MERCURY,	C_URANUS,	F_MNU3|F_PLT),
    rcfpack (R_MERCURY,	C_VENUS,	F_MNU3|F_PLT),
    rcfpack (R_MOON,	C_ALT,		F_MNU1|F_PLT),
    rcfpack (R_MOON,	C_AZ,		F_MNU1|F_PLT),
    rcfpack (R_MOON,	C_CONSTEL,	F_MNU1|F_CHG),
    rcfpack (R_MOON,	C_CONSTEL,	F_MNU2|F_CHG),
    rcfpack (R_MOON,	C_CONSTEL,	F_MNU3|F_CHG),
    rcfpack (R_MOON,	C_DEC,		F_MNU1|F_PLT),
    rcfpack (R_MOON,	C_EDIST,	F_MNU1|F_PLT),
    rcfpack (R_MOON,	C_ELONG,	F_MNU1|F_PLT),
    rcfpack (R_MOON,	C_HLAT,		F_MNU1|F_PLT),
    rcfpack (R_MOON,	C_HLONG,	F_MNU1|F_PLT),
    rcfpack (R_MOON,	C_JUPITER,	F_MNU3|F_PLT),
    rcfpack (R_MOON,	C_MAG,		F_MNU1|F_PLT),
    rcfpack (R_MOON,	C_MARS,		F_MNU3|F_PLT),
    rcfpack (R_MOON,	C_MERCURY,	F_MNU3|F_PLT),
    rcfpack (R_MOON,	C_NEPTUNE,	F_MNU3|F_PLT),
    rcfpack (R_MOON,	C_OBJ,		F_MNU1|F_CHG),
    rcfpack (R_MOON,	C_OBJ,		F_MNU2|F_CHG),
    rcfpack (R_MOON,	C_OBJ,		F_MNU3|F_CHG),
    rcfpack (R_MOON,	C_OBJX,		F_MNU3|F_PLT),
    rcfpack (R_MOON,	C_OBJY,		F_MNU3|F_PLT),
    rcfpack (R_MOON,	C_PHASE,	F_MNU1|F_PLT),
    rcfpack (R_MOON,	C_PLUTO,	F_MNU3|F_PLT),
    rcfpack (R_MOON,	C_RA,		F_MNU1|F_PLT),
    rcfpack (R_MOON,	C_RISEAZ,	F_MNU2|F_PLT),
    rcfpack (R_MOON,	C_RISETM,	F_MNU2|F_PLT),
    rcfpack (R_MOON,	C_SATURN,	F_MNU3|F_PLT),
    rcfpack (R_MOON,	C_SDIST,	F_MNU1|F_PLT),
    rcfpack (R_MOON,	C_SETAZ,	F_MNU2|F_PLT),
    rcfpack (R_MOON,	C_SETTM,	F_MNU2|F_PLT),
    rcfpack (R_MOON,	C_SIZE,		F_MNU1|F_PLT),
    rcfpack (R_MOON,	C_SUN,		F_MNU3|F_PLT),
    rcfpack (R_MOON,	C_TRANSALT,	F_MNU2|F_PLT),
    rcfpack (R_MOON,	C_TRANSTM,	F_MNU2|F_PLT),
    rcfpack (R_MOON,	C_TUP,		F_MNU2|F_PLT),
    rcfpack (R_MOON,	C_URANUS,	F_MNU3|F_PLT),
    rcfpack (R_MOON,	C_VENUS,	F_MNU3|F_PLT),
    rcfpack (R_NEPTUNE,	C_ALT,		F_MNU1|F_PLT),
    rcfpack (R_NEPTUNE,	C_AZ,		F_MNU1|F_PLT),
    rcfpack (R_NEPTUNE,	C_CONSTEL,	F_MNU1|F_CHG),
    rcfpack (R_NEPTUNE,	C_CONSTEL,	F_MNU2|F_CHG),
    rcfpack (R_NEPTUNE,	C_CONSTEL,	F_MNU3|F_CHG),
    rcfpack (R_NEPTUNE,	C_DEC,		F_MNU1|F_PLT),
    rcfpack (R_NEPTUNE,	C_EDIST,	F_MNU1|F_PLT),
    rcfpack (R_NEPTUNE,	C_ELONG,	F_MNU1|F_PLT),
    rcfpack (R_NEPTUNE,	C_HLAT,		F_MNU1|F_PLT),
    rcfpack (R_NEPTUNE,	C_HLONG,	F_MNU1|F_PLT),
    rcfpack (R_NEPTUNE,	C_JUPITER,	F_MNU3|F_PLT),
    rcfpack (R_NEPTUNE,	C_MAG,		F_MNU1|F_PLT),
    rcfpack (R_NEPTUNE,	C_MARS,		F_MNU3|F_PLT),
    rcfpack (R_NEPTUNE,	C_MERCURY,	F_MNU3|F_PLT),
    rcfpack (R_NEPTUNE,	C_MOON,		F_MNU3|F_PLT),
    rcfpack (R_NEPTUNE,	C_OBJ,		F_MNU1|F_CHG),
    rcfpack (R_NEPTUNE,	C_OBJ,		F_MNU2|F_CHG),
    rcfpack (R_NEPTUNE,	C_OBJ,		F_MNU3|F_CHG),
    rcfpack (R_NEPTUNE,	C_OBJX,		F_MNU3|F_PLT),
    rcfpack (R_NEPTUNE,	C_OBJY,		F_MNU3|F_PLT),
    rcfpack (R_NEPTUNE,	C_PHASE,	F_MNU1|F_PLT),
    rcfpack (R_NEPTUNE,	C_PLUTO,	F_MNU3|F_PLT),
    rcfpack (R_NEPTUNE,	C_RA,		F_MNU1|F_PLT),
    rcfpack (R_NEPTUNE,	C_RISEAZ,	F_MNU2|F_PLT),
    rcfpack (R_NEPTUNE,	C_RISETM,	F_MNU2|F_PLT),
    rcfpack (R_NEPTUNE,	C_SATURN,	F_MNU3|F_PLT),
    rcfpack (R_NEPTUNE,	C_SDIST,	F_MNU1|F_PLT),
    rcfpack (R_NEPTUNE,	C_SETAZ,	F_MNU2|F_PLT),
    rcfpack (R_NEPTUNE,	C_SETTM,	F_MNU2|F_PLT),
    rcfpack (R_NEPTUNE,	C_SIZE,		F_MNU1|F_PLT),
    rcfpack (R_NEPTUNE,	C_SUN,		F_MNU3|F_PLT),
    rcfpack (R_NEPTUNE,	C_TRANSALT,	F_MNU2|F_PLT),
    rcfpack (R_NEPTUNE,	C_TRANSTM,	F_MNU2|F_PLT),
    rcfpack (R_NEPTUNE,	C_TUP,		F_MNU2|F_PLT),
    rcfpack (R_NEPTUNE,	C_URANUS,	F_MNU3|F_PLT),
    rcfpack (R_NEPTUNE,	C_VENUS,	F_MNU3|F_PLT),
    rcfpack (R_NSTEP,	C_NSTEPV,	F_MMNU|F_CHG),
    rcfpack (R_OBJX,	C_ALT,		F_MNU1|F_PLT),
    rcfpack (R_OBJX,	C_AZ,		F_MNU1|F_PLT),
    rcfpack (R_OBJX,	C_CONSTEL,	F_MNU1|F_CHG),
    rcfpack (R_OBJX,	C_CONSTEL,	F_MNU2|F_CHG),
    rcfpack (R_OBJX,	C_CONSTEL,	F_MNU3|F_CHG),
    rcfpack (R_OBJX,	C_DEC,		F_MNU1|F_PLT),
    rcfpack (R_OBJX,	C_EDIST,	F_MNU1|F_PLT),
    rcfpack (R_OBJX,	C_ELONG,	F_MNU1|F_PLT),
    rcfpack (R_OBJX,	C_HLAT,		F_MNU1|F_PLT),
    rcfpack (R_OBJX,	C_HLONG,	F_MNU1|F_PLT),
    rcfpack (R_OBJX,	C_JUPITER,	F_MNU3|F_PLT),
    rcfpack (R_OBJX,	C_MAG,		F_MNU1|F_PLT),
    rcfpack (R_OBJX,	C_MARS,		F_MNU3|F_PLT),
    rcfpack (R_OBJX,	C_MERCURY,	F_MNU3|F_PLT),
    rcfpack (R_OBJX,	C_MOON,		F_MNU3|F_PLT),
    rcfpack (R_OBJX,	C_NEPTUNE,	F_MNU3|F_PLT),
    rcfpack (R_OBJX,	C_OBJ,		F_MNU1|F_CHG),
    rcfpack (R_OBJX,	C_OBJ,		F_MNU2|F_CHG),
    rcfpack (R_OBJX,	C_OBJ,		F_MNU3|F_CHG),
    rcfpack (R_OBJX,	C_OBJY,		F_MNU3|F_PLT),
    rcfpack (R_OBJX,	C_PHASE,	F_MNU1|F_PLT),
    rcfpack (R_OBJX,	C_PLUTO,	F_MNU3|F_PLT),
    rcfpack (R_OBJX,	C_RA,		F_MNU1|F_PLT),
    rcfpack (R_OBJX,	C_RISEAZ,	F_MNU2|F_PLT),
    rcfpack (R_OBJX,	C_RISETM,	F_MNU2|F_PLT),
    rcfpack (R_OBJX,	C_SATURN,	F_MNU3|F_PLT),
    rcfpack (R_OBJX,	C_SDIST,	F_MNU1|F_PLT),
    rcfpack (R_OBJX,	C_SETAZ,	F_MNU2|F_PLT),
    rcfpack (R_OBJX,	C_SETTM,	F_MNU2|F_PLT),
    rcfpack (R_OBJX,	C_SIZE,		F_MNU1|F_PLT),
    rcfpack (R_OBJX,	C_SUN,		F_MNU3|F_PLT),
    rcfpack (R_OBJX,	C_TRANSALT,	F_MNU2|F_PLT),
    rcfpack (R_OBJX,	C_TRANSTM,	F_MNU2|F_PLT),
    rcfpack (R_OBJX,	C_TUP,		F_MNU2|F_PLT),
    rcfpack (R_OBJX,	C_URANUS,	F_MNU3|F_PLT),
    rcfpack (R_OBJX,	C_VENUS,	F_MNU3|F_PLT),
    rcfpack (R_OBJY,	C_ALT,		F_MNU1|F_PLT),
    rcfpack (R_OBJY,	C_AZ,		F_MNU1|F_PLT),
    rcfpack (R_OBJY,	C_CONSTEL,	F_MNU1|F_CHG),
    rcfpack (R_OBJY,	C_CONSTEL,	F_MNU2|F_CHG),
    rcfpack (R_OBJY,	C_CONSTEL,	F_MNU3|F_CHG),
    rcfpack (R_OBJY,	C_DEC,		F_MNU1|F_PLT),
    rcfpack (R_OBJY,	C_EDIST,	F_MNU1|F_PLT),
    rcfpack (R_OBJY,	C_ELONG,	F_MNU1|F_PLT),
    rcfpack (R_OBJY,	C_HLAT,		F_MNU1|F_PLT),
    rcfpack (R_OBJY,	C_HLONG,	F_MNU1|F_PLT),
    rcfpack (R_OBJY,	C_JUPITER,	F_MNU3|F_PLT),
    rcfpack (R_OBJY,	C_MAG,		F_MNU1|F_PLT),
    rcfpack (R_OBJY,	C_MARS,		F_MNU3|F_PLT),
    rcfpack (R_OBJY,	C_MERCURY,	F_MNU3|F_PLT),
    rcfpack (R_OBJY,	C_MOON,		F_MNU3|F_PLT),
    rcfpack (R_OBJY,	C_NEPTUNE,	F_MNU3|F_PLT),
    rcfpack (R_OBJY,	C_OBJ,		F_MNU1|F_CHG),
    rcfpack (R_OBJY,	C_OBJ,		F_MNU2|F_CHG),
    rcfpack (R_OBJY,	C_OBJ,		F_MNU3|F_CHG),
    rcfpack (R_OBJY,	C_OBJX,		F_MNU3|F_PLT),
    rcfpack (R_OBJY,	C_PHASE,	F_MNU1|F_PLT),
    rcfpack (R_OBJY,	C_PLUTO,	F_MNU3|F_PLT),
    rcfpack (R_OBJY,	C_RA,		F_MNU1|F_PLT),
    rcfpack (R_OBJY,	C_RISEAZ,	F_MNU2|F_PLT),
    rcfpack (R_OBJY,	C_RISETM,	F_MNU2|F_PLT),
    rcfpack (R_OBJY,	C_SATURN,	F_MNU3|F_PLT),
    rcfpack (R_OBJY,	C_SDIST,	F_MNU1|F_PLT),
    rcfpack (R_OBJY,	C_SETAZ,	F_MNU2|F_PLT),
    rcfpack (R_OBJY,	C_SETTM,	F_MNU2|F_PLT),
    rcfpack (R_OBJY,	C_SIZE,		F_MNU1|F_PLT),
    rcfpack (R_OBJY,	C_SUN,		F_MNU3|F_PLT),
    rcfpack (R_OBJY,	C_TRANSALT,	F_MNU2|F_PLT),
    rcfpack (R_OBJY,	C_TRANSTM,	F_MNU2|F_PLT),
    rcfpack (R_OBJY,	C_TUP,		F_MNU2|F_PLT),
    rcfpack (R_OBJY,	C_URANUS,	F_MNU3|F_PLT),
    rcfpack (R_OBJY,	C_VENUS,	F_MNU3|F_PLT),
    rcfpack (R_PAUSE,	C_PAUSEV,	F_MMNU|F_CHG),
    rcfpack (R_PLOT,	C_PLOT,		F_MMNU|F_CHG),
    rcfpack (R_PLUTO,	C_ALT,		F_MNU1|F_PLT),
    rcfpack (R_PLUTO,	C_AZ,		F_MNU1|F_PLT),
    rcfpack (R_PLUTO,	C_CONSTEL,	F_MNU1|F_CHG),
    rcfpack (R_PLUTO,	C_CONSTEL,	F_MNU2|F_CHG),
    rcfpack (R_PLUTO,	C_CONSTEL,	F_MNU3|F_CHG),
    rcfpack (R_PLUTO,	C_DEC,		F_MNU1|F_PLT),
    rcfpack (R_PLUTO,	C_EDIST,	F_MNU1|F_PLT),
    rcfpack (R_PLUTO,	C_ELONG,	F_MNU1|F_PLT),
    rcfpack (R_PLUTO,	C_HLAT,		F_MNU1|F_PLT),
    rcfpack (R_PLUTO,	C_HLONG,	F_MNU1|F_PLT),
    rcfpack (R_PLUTO,	C_JUPITER,	F_MNU3|F_PLT),
    rcfpack (R_PLUTO,	C_MAG,		F_MNU1|F_PLT),
    rcfpack (R_PLUTO,	C_MARS,		F_MNU3|F_PLT),
    rcfpack (R_PLUTO,	C_MERCURY,	F_MNU3|F_PLT),
    rcfpack (R_PLUTO,	C_MOON,		F_MNU3|F_PLT),
    rcfpack (R_PLUTO,	C_NEPTUNE,	F_MNU3|F_PLT),
    rcfpack (R_PLUTO,	C_OBJ,		F_MNU1|F_CHG),
    rcfpack (R_PLUTO,	C_OBJ,		F_MNU2|F_CHG),
    rcfpack (R_PLUTO,	C_OBJ,		F_MNU3|F_CHG),
    rcfpack (R_PLUTO,	C_OBJX,		F_MNU3|F_PLT),
    rcfpack (R_PLUTO,	C_OBJY,		F_MNU3|F_PLT),
    rcfpack (R_PLUTO,	C_PHASE,	F_MNU1|F_PLT),
    rcfpack (R_PLUTO,	C_RA,		F_MNU1|F_PLT),
    rcfpack (R_PLUTO,	C_RISEAZ,	F_MNU2|F_PLT),
    rcfpack (R_PLUTO,	C_RISETM,	F_MNU2|F_PLT),
    rcfpack (R_PLUTO,	C_SATURN,	F_MNU3|F_PLT),
    rcfpack (R_PLUTO,	C_SDIST,	F_MNU1|F_PLT),
    rcfpack (R_PLUTO,	C_SETAZ,	F_MNU2|F_PLT),
    rcfpack (R_PLUTO,	C_SETTM,	F_MNU2|F_PLT),
    rcfpack (R_PLUTO,	C_SIZE,		F_MNU1|F_PLT),
    rcfpack (R_PLUTO,	C_SUN,		F_MNU3|F_PLT),
    rcfpack (R_PLUTO,	C_TRANSALT,	F_MNU2|F_PLT),
    rcfpack (R_PLUTO,	C_TRANSTM,	F_MNU2|F_PLT),
    rcfpack (R_PLUTO,	C_TUP,		F_MNU2|F_PLT),
    rcfpack (R_PLUTO,	C_URANUS,	F_MNU3|F_PLT),
    rcfpack (R_PLUTO,	C_VENUS,	F_MNU3|F_PLT),
    rcfpack (R_PRES,	C_PRESV,	F_MMNU|F_CHG|F_PLT),
    rcfpack (R_SATURN,	C_ALT,		F_MNU1|F_PLT),
    rcfpack (R_SATURN,	C_AZ,		F_MNU1|F_PLT),
    rcfpack (R_SATURN,	C_CONSTEL,	F_MNU1|F_CHG),
    rcfpack (R_SATURN,	C_CONSTEL,	F_MNU2|F_CHG),
    rcfpack (R_SATURN,	C_CONSTEL,	F_MNU3|F_CHG),
    rcfpack (R_SATURN,	C_DEC,		F_MNU1|F_PLT),
    rcfpack (R_SATURN,	C_EDIST,	F_MNU1|F_PLT),
    rcfpack (R_SATURN,	C_ELONG,	F_MNU1|F_PLT),
    rcfpack (R_SATURN,	C_HLAT,		F_MNU1|F_PLT),
    rcfpack (R_SATURN,	C_HLONG,	F_MNU1|F_PLT),
    rcfpack (R_SATURN,	C_JUPITER,	F_MNU3|F_PLT),
    rcfpack (R_SATURN,	C_MAG,		F_MNU1|F_PLT),
    rcfpack (R_SATURN,	C_MARS,		F_MNU3|F_PLT),
    rcfpack (R_SATURN,	C_MERCURY,	F_MNU3|F_PLT),
    rcfpack (R_SATURN,	C_MOON,		F_MNU3|F_PLT),
    rcfpack (R_SATURN,	C_NEPTUNE,	F_MNU3|F_PLT),
    rcfpack (R_SATURN,	C_OBJ,		F_MNU1|F_CHG),
    rcfpack (R_SATURN,	C_OBJ,		F_MNU2|F_CHG),
    rcfpack (R_SATURN,	C_OBJ,		F_MNU3|F_CHG),
    rcfpack (R_SATURN,	C_OBJX,		F_MNU3|F_PLT),
    rcfpack (R_SATURN,	C_OBJY,		F_MNU3|F_PLT),
    rcfpack (R_SATURN,	C_PHASE,	F_MNU1|F_PLT),
    rcfpack (R_SATURN,	C_PLUTO,	F_MNU3|F_PLT),
    rcfpack (R_SATURN,	C_RA,		F_MNU1|F_PLT),
    rcfpack (R_SATURN,	C_RISEAZ,	F_MNU2|F_PLT),
    rcfpack (R_SATURN,	C_RISETM,	F_MNU2|F_PLT),
    rcfpack (R_SATURN,	C_SDIST,	F_MNU1|F_PLT),
    rcfpack (R_SATURN,	C_SETAZ,	F_MNU2|F_PLT),
    rcfpack (R_SATURN,	C_SETTM,	F_MNU2|F_PLT),
    rcfpack (R_SATURN,	C_SIZE,		F_MNU1|F_PLT),
    rcfpack (R_SATURN,	C_SUN,		F_MNU3|F_PLT),
    rcfpack (R_SATURN,	C_TRANSALT,	F_MNU2|F_PLT),
    rcfpack (R_SATURN,	C_TRANSTM,	F_MNU2|F_PLT),
    rcfpack (R_SATURN,	C_TUP,		F_MNU2|F_PLT),
    rcfpack (R_SATURN,	C_URANUS,	F_MNU3|F_PLT),
    rcfpack (R_SATURN,	C_VENUS,	F_MNU3|F_PLT),
    rcfpack (R_SRCH,	C_SRCH,		F_MMNU|F_CHG|F_PLT),
    rcfpack (R_STPSZ,	C_STPSZV,	F_MMNU|F_CHG),
    rcfpack (R_SUN,	C_ALT,		F_MNU1|F_PLT),
    rcfpack (R_SUN,	C_AZ,		F_MNU1|F_PLT),
    rcfpack (R_SUN,	C_CONSTEL,	F_MNU1|F_CHG),
    rcfpack (R_SUN,	C_CONSTEL,	F_MNU2|F_CHG),
    rcfpack (R_SUN,	C_CONSTEL,	F_MNU3|F_CHG),
    rcfpack (R_SUN,	C_DEC,		F_MNU1|F_PLT),
    rcfpack (R_SUN,	C_EDIST,	F_MNU1|F_PLT),
    rcfpack (R_SUN,	C_HLONG,	F_MNU1|F_PLT),
    rcfpack (R_SUN,	C_JUPITER,	F_MNU3|F_PLT),
    rcfpack (R_SUN,	C_MAG,		F_MNU1|F_PLT),
    rcfpack (R_SUN,	C_MARS,		F_MNU3|F_PLT),
    rcfpack (R_SUN,	C_MERCURY,	F_MNU3|F_PLT),
    rcfpack (R_SUN,	C_MOON,		F_MNU3|F_PLT),
    rcfpack (R_SUN,	C_NEPTUNE,	F_MNU3|F_PLT),
    rcfpack (R_SUN,	C_OBJ,		F_MNU1|F_CHG),
    rcfpack (R_SUN,	C_OBJ,		F_MNU2|F_CHG),
    rcfpack (R_SUN,	C_OBJ,		F_MNU3|F_CHG),
    rcfpack (R_SUN,	C_OBJX,		F_MNU3|F_PLT),
    rcfpack (R_SUN,	C_OBJY,		F_MNU3|F_PLT),
    rcfpack (R_SUN,	C_PLUTO,	F_MNU3|F_PLT),
    rcfpack (R_SUN,	C_RA,		F_MNU1|F_PLT),
    rcfpack (R_SUN,	C_RISEAZ,	F_MNU2|F_PLT),
    rcfpack (R_SUN,	C_RISETM,	F_MNU2|F_PLT),
    rcfpack (R_SUN,	C_SATURN,	F_MNU3|F_PLT),
    rcfpack (R_SUN,	C_SETAZ,	F_MNU2|F_PLT),
    rcfpack (R_SUN,	C_SETTM,	F_MNU2|F_PLT),
    rcfpack (R_SUN,	C_SIZE,		F_MNU1|F_PLT),
    rcfpack (R_SUN,	C_TRANSALT,	F_MNU2|F_PLT),
    rcfpack (R_SUN,	C_TRANSTM,	F_MNU2|F_PLT),
    rcfpack (R_SUN,	C_TUP,		F_MNU2|F_PLT),
    rcfpack (R_SUN,	C_URANUS,	F_MNU3|F_PLT),
    rcfpack (R_SUN,	C_VENUS,	F_MNU3|F_PLT),
    rcfpack (R_TEMP,	C_TEMPV,	F_MMNU|F_CHG|F_PLT),
    rcfpack (R_TZN,	C_TZN,		F_MMNU|F_CHG),
    rcfpack (R_TZONE,	C_TZONEV,	F_MMNU|F_CHG),
    rcfpack (R_UD,	C_UD,		F_MMNU|F_PLT|F_CHG),
    rcfpack (R_URANUS,	C_ALT,		F_MNU1|F_PLT),
    rcfpack (R_URANUS,	C_AZ,		F_MNU1|F_PLT),
    rcfpack (R_URANUS,	C_CONSTEL,	F_MNU1|F_CHG),
    rcfpack (R_URANUS,	C_CONSTEL,	F_MNU2|F_CHG),
    rcfpack (R_URANUS,	C_CONSTEL,	F_MNU3|F_CHG),
    rcfpack (R_URANUS,	C_DEC,		F_MNU1|F_PLT),
    rcfpack (R_URANUS,	C_EDIST,	F_MNU1|F_PLT),
    rcfpack (R_URANUS,	C_ELONG,	F_MNU1|F_PLT),
    rcfpack (R_URANUS,	C_HLAT,		F_MNU1|F_PLT),
    rcfpack (R_URANUS,	C_HLONG,	F_MNU1|F_PLT),
    rcfpack (R_URANUS,	C_JUPITER,	F_MNU3|F_PLT),
    rcfpack (R_URANUS,	C_MAG,		F_MNU1|F_PLT),
    rcfpack (R_URANUS,	C_MARS,		F_MNU3|F_PLT),
    rcfpack (R_URANUS,	C_MERCURY,	F_MNU3|F_PLT),
    rcfpack (R_URANUS,	C_MOON,		F_MNU3|F_PLT),
    rcfpack (R_URANUS,	C_NEPTUNE,	F_MNU3|F_PLT),
    rcfpack (R_URANUS,	C_OBJ,		F_MNU1|F_CHG),
    rcfpack (R_URANUS,	C_OBJ,		F_MNU2|F_CHG),
    rcfpack (R_URANUS,	C_OBJ,		F_MNU3|F_CHG),
    rcfpack (R_URANUS,	C_OBJX,		F_MNU3|F_PLT),
    rcfpack (R_URANUS,	C_OBJY,		F_MNU3|F_PLT),
    rcfpack (R_URANUS,	C_PHASE,	F_MNU1|F_PLT),
    rcfpack (R_URANUS,	C_PLUTO,	F_MNU3|F_PLT),
    rcfpack (R_URANUS,	C_RA,		F_MNU1|F_PLT),
    rcfpack (R_URANUS,	C_RISEAZ,	F_MNU2|F_PLT),
    rcfpack (R_URANUS,	C_RISETM,	F_MNU2|F_PLT),
    rcfpack (R_URANUS,	C_SATURN,	F_MNU3|F_PLT),
    rcfpack (R_URANUS,	C_SDIST,	F_MNU1|F_PLT),
    rcfpack (R_URANUS,	C_SETAZ,	F_MNU2|F_PLT),
    rcfpack (R_URANUS,	C_SETTM,	F_MNU2|F_PLT),
    rcfpack (R_URANUS,	C_SIZE,		F_MNU1|F_PLT),
    rcfpack (R_URANUS,	C_SUN,		F_MNU3|F_PLT),
    rcfpack (R_URANUS,	C_TRANSALT,	F_MNU2|F_PLT),
    rcfpack (R_URANUS,	C_TRANSTM,	F_MNU2|F_PLT),
    rcfpack (R_URANUS,	C_TUP,		F_MNU2|F_PLT),
    rcfpack (R_URANUS,	C_VENUS,	F_MNU3|F_PLT),
    rcfpack (R_UT,	C_UTV,		F_MMNU|F_PLT|F_CHG),
    rcfpack (R_VENUS,	C_ALT,		F_MNU1|F_PLT),
    rcfpack (R_VENUS,	C_AZ,		F_MNU1|F_PLT),
    rcfpack (R_VENUS,	C_CONSTEL,	F_MNU1|F_CHG),
    rcfpack (R_VENUS,	C_CONSTEL,	F_MNU2|F_CHG),
    rcfpack (R_VENUS,	C_CONSTEL,	F_MNU3|F_CHG),
    rcfpack (R_VENUS,	C_DEC,		F_MNU1|F_PLT),
    rcfpack (R_VENUS,	C_EDIST,	F_MNU1|F_PLT),
    rcfpack (R_VENUS,	C_ELONG,	F_MNU1|F_PLT),
    rcfpack (R_VENUS,	C_HLAT,		F_MNU1|F_PLT),
    rcfpack (R_VENUS,	C_HLONG,	F_MNU1|F_PLT),
    rcfpack (R_VENUS,	C_JUPITER,	F_MNU3|F_PLT),
    rcfpack (R_VENUS,	C_MAG,		F_MNU1|F_PLT),
    rcfpack (R_VENUS,	C_MARS,		F_MNU3|F_PLT),
    rcfpack (R_VENUS,	C_MERCURY,	F_MNU3|F_PLT),
    rcfpack (R_VENUS,	C_MOON,		F_MNU3|F_PLT),
    rcfpack (R_VENUS,	C_NEPTUNE,	F_MNU3|F_PLT),
    rcfpack (R_VENUS,	C_OBJ,		F_MNU1|F_CHG),
    rcfpack (R_VENUS,	C_OBJ,		F_MNU2|F_CHG),
    rcfpack (R_VENUS,	C_OBJ,		F_MNU3|F_CHG),
    rcfpack (R_VENUS,	C_OBJX,		F_MNU3|F_PLT),
    rcfpack (R_VENUS,	C_OBJY,		F_MNU3|F_PLT),
    rcfpack (R_VENUS,	C_PHASE,	F_MNU1|F_PLT),
    rcfpack (R_VENUS,	C_PLUTO,	F_MNU3|F_PLT),
    rcfpack (R_VENUS,	C_RA,		F_MNU1|F_PLT),
    rcfpack (R_VENUS,	C_RISEAZ,	F_MNU2|F_PLT),
    rcfpack (R_VENUS,	C_RISETM,	F_MNU2|F_PLT),
    rcfpack (R_VENUS,	C_SATURN,	F_MNU3|F_PLT),
    rcfpack (R_VENUS,	C_SDIST,	F_MNU1|F_PLT),
    rcfpack (R_VENUS,	C_SETAZ,	F_MNU2|F_PLT),
    rcfpack (R_VENUS,	C_SETTM,	F_MNU2|F_PLT),
    rcfpack (R_VENUS,	C_SIZE,		F_MNU1|F_PLT),
    rcfpack (R_VENUS,	C_SUN,		F_MNU3|F_PLT),
    rcfpack (R_VENUS,	C_TRANSALT,	F_MNU2|F_PLT),
    rcfpack (R_VENUS,	C_TRANSTM,	F_MNU2|F_PLT),
    rcfpack (R_VENUS,	C_TUP,		F_MNU2|F_PLT),
    rcfpack (R_VENUS,	C_URANUS,	F_MNU3|F_PLT),
    rcfpack (R_WATCH,	C_WATCH,	F_MMNU|F_CHG),
};
#define	NFIELDS (sizeof(fields)/sizeof(fields[0]))

/* let op select a field by moving around and hitting RETURN or SPACE, or
 *   until see END. also allow moving directly to frequently used fields 
 *   with some hot-keys.
 * only allow fields with the given flag mask.
 * return the rcfpack()'d field, or 0 if typed END.
 * N.B. we might also exit() entirely by calling bye() if op types QUIT.
 */
sel_fld (f, flag, prmpt, help)
int f;		/* inital row, col */
int flag;
char *prmpt, *help;
{
	extern void bye();
	int r = unpackr(f), c = unpackc(f);
	char *lastp;
	int ch;

	lastp = 0;
	while (1) {
	    if (lastp != prmpt) {
		lastp = prmpt;
		f_prompt (lastp);
	    }
	    c_pos (r, c);
	    switch (ch = read_char()) {
	    case REDRAW:
		redraw_screen(2);	/* redraw all from scratch */
		lastp = 0;
		break;
	    case VERSION:
		version();
		lastp = 0;
		break;
	    case HELP:
		f_msg (help);
		lastp = 0;
		break;
	    case QUIT:
		f_prompt ("Exit ephem? (y) ");
		if (read_char() == 'y')
		    bye();	/* never returns */
		lastp = 0;
		break;
#ifdef BANG
	    case '!': {
		char buf[NC];
		f_prompt ("!");
		if (read_line (buf, sizeof(buf)-2) > 0) {
		    c_erase();
		    byetty();
		    (void) system (buf);
		    printf ("\nHit any key to resume ephem...");
		    (void) read_char();
		    redraw_screen(2);	/* redraw all from scratch */
		}
		lastp = 0;
		}
		break;
#endif
	    case END:
		return (0);
	    case '\r': case ' ':
		return (rcfpack (r, c, 0));
	    default:
		move_cur (ch, flag, &r, &c);
		break;
	    }
	}
}

/* move cursor to next field in given direction: hjkl, or directly to a
 * field, and set *rp and *cp.
 * limit eligible fields to those with given flag mask.
 */
static
move_cur (dirchar, flag, rp, cp)
char dirchar;
int flag;
int *rp, *cp;
{
	int curr = *rp, curc = *cp;
	F_t f, newf, *fp;
	int d, newd;

    wrapped:
	newf = 0;
	newd = 1000;

	switch (dirchar) {
	case 'h': /* left */
	    /* go to next field to the left, or wrap.  */
	    for (fp = fields+NFIELDS; --fp >= fields; ) {
		f = *fp;
		if (tstpackf(f,flag) && unpackr(f) == curr) {
		    d = curc - unpackc(f);
		    if (d > 0 && d < newd) {
			newf = f;
			newd = d;
		    }
		}
	    }
	    if (!newf) {
		curc = NC;
		goto wrapped;
	    }
	    break;

	case 'j': /* down */
	    /* go to closest field on next row down with anything on it,
	     * or wrap.
	     */
	    for (fp = fields+NFIELDS; --fp >= fields; ) {
		f = *fp;
		if (tstpackf(f,flag)) {
		    d = unpackr(f) - curr;
		    if (d > 0 && d < newd) {
			newf = f;
			newd = d;
		    }
		}
	    }
	    if (newf) {
		/* now find the field closest to current col on that row */
		newf = nearestfld (unpackr(newf), curc, flag);
	    } else {
		curr = 0;
		goto wrapped;
	    }
	    break;

	case 'k': /* up */
	    /* go to closest field on next row up with anything on it, 
	     * or wrap.
	     */
	    for (fp = fields+NFIELDS; --fp >= fields; ) {
		f = *fp;
		if (tstpackf(f,flag)) {
		    d = curr - unpackr(f);
		    if (d > 0 && d < newd) {
			newf = f;
			newd = d;
		    }
		}
	    }
	    if (newf) {
		/* now find the field closest to current col on that row */
		newf = nearestfld (unpackr(newf), curc, flag);
	    } else {
		curr = NR+1;
		goto wrapped;
	    }
	    break;

	case 'l': /* right */
	    /* go to next field to the right, or wrap.  */
	    for (fp = fields+NFIELDS; --fp >= fields; ) {
		f = *fp;
		if (tstpackf(f,flag) && unpackr(f) == curr) {
		    d = unpackc(f) - curc;
		    if (d > 0 && d < newd) {
			newf = f;
			newd = d;
		    }
		}
	    }
	    if (!newf) {
		curc = 0;
		goto wrapped;
	    }
	    break;

	/* handy shorthands directly to a given spot.
	 * calling nearestfld() automatically allows for which menu
	 *   is up now and what is pickable. you can use rcfpack()
	 *   directly for top half fields that are always up.
	 * N.B. using nearestfld() can be too aggressive. it will try
	 *   other fields entirely if one you intend is not eligible.
	 */
	case 'S': newf = nearestfld (R_SUN, C_OBJ, flag); break;
	case 'M': newf = nearestfld (R_MOON, C_OBJ, flag); break;
	case 'e': newf = nearestfld (R_MERCURY, C_OBJ, flag); break;
	case 'v': newf = nearestfld (R_VENUS, C_OBJ, flag); break;
	case 'm': newf = nearestfld (R_MARS, C_OBJ, flag); break;
	case 'J': newf = nearestfld (R_JUPITER, C_OBJ, flag); break;
	case 's': newf = nearestfld (R_SATURN, C_OBJ, flag); break;
	case 'u': newf = nearestfld (R_URANUS, C_OBJ, flag); break;
	case 'n': newf = nearestfld (R_NEPTUNE, C_OBJ, flag); break;
	case 'p': newf = nearestfld (R_PLUTO, C_OBJ, flag); break;
	case 'x': newf = nearestfld (R_OBJX, C_OBJ, flag); break;
	case 'y': newf = nearestfld (R_OBJY, C_OBJ, flag); break;
	case 'c': newf = nearestfld (R_ALTM, C_ALTM, flag); break;
	case 'd': newf = nearestfld (R_UD, C_UD, flag); break;
	case 'o': newf = nearestfld (R_EPOCH, C_EPOCHV, flag); break;
	case 'z': newf = nearestfld (R_STPSZ, C_STPSZV, flag); break;
	case 'w': newf = nearestfld (R_WATCH, C_WATCH, flag); break;
	case 'L': newf = nearestfld (R_LISTING, C_LISTING, flag); break;
	}

	if (newf) {
	    *rp = unpackr(newf);
	    *cp = unpackc(newf);
	}
}

/* return the nearest field with given flag mask, either way, on this row,
 * else 0 if none.
 */
static int
nearestfld (r, c, flag)
int r, c, flag;
{
	F_t nf, f, *fp;
	int d, d0;

	nf = 0;
	d0 = 1000;

	for (fp = fields+NFIELDS; --fp >= fields; ) {
	    f = *fp;
	    if (tstpackf(f,flag) && unpackr(f) == r) {
		d = abs(c - unpackc(f));
		if (d < d0) {
		    nf = f;
		    d0 = d;
		}
	    }
	}
	return (nf);
}
