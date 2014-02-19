// From DarkCal.cpp

#define TP_START a
#define TP_END b
#define TP_RISE a
#define TP_SET b

enum DST { DST_NONE, DST_START, DST_END };

// Struct to hold date & location
struct CalData {
  CalData() : month(1), year(2000) {}

  int month;
  int year;
  ObsInfo loc;
};
 
