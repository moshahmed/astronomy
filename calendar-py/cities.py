locations = {
  # Notes:
  #   1. Spaces are first trimmed.
  #   2. TZ is Asia/Kolkoata or Asia/Calcutta, depending on python.
  #   3. vim !align.pl '-/[,:\;]'
  #"Location" : 'City      , Country ; Latitude  ; Longitude   ; Elevation ; Time   / Zone  '   ,
  "Accra"     : 'Accra     , Ghana   ; 5.5701389 ; -0.17819444 ; 20        ; Africa / Accra'    ,
  "Sunnyvale" : 'Sunnyvale , USA     ; 37.3689   ; -122.0353   ; 10        ; US     / Pacific'  ,
  "Mumbai"    : 'Mumbai    , India   ; 18.977707 ; 72.822960   ; 21        ; Asia   / Calcutta' ,
  "Mangalore" : 'Mangalore , India   ; 12.887261 ; 75.412277   ; 20        ; Asia   / Calcutta' ,
  "Kigali"    : 'Kigali    , Rwanda  ; -1.94629  ; +30.10313   ; 1426      ; Africa / Kigali'   ,
  "NYC"       : 'NewYork   , USA     ;  40.7128  ; -74.0059    ; 10        ; America/New_York' ,

  # Output of city2loc.pl | grep /
  "Accra"     : 'Accra     , Ghana   ;    5.5700 ;   -0.1781   ; 20        ; Africa/Accra ',
  "SanJose"   : 'SanJose   , USA     ;   37.3378 ; -121.8900   ; 27        ; America/Costa_Rica ',
  "Athens"    : 'Athens    , Greece  ;   37.9667 ;   23.7333   ; 91        ; Europe/Athens ',
  "Beijing"   : 'Beijing   , China   ;   39.9000 ;  116.4667   ; 183       ; Asia/Shanghai ',
  "Berlin"    : 'Berlin    , Germany ;   52.5333 ;   13.4167   ; 34        ; Europe/Berlin ',
  "Jerusalem" : 'Jerusalem , Israel  ;   31.7833 ;   35.2167   ; 762       ; Asia/Jerusalem ',
  "London"    : 'London    , UK      ;   51.5000 ;    0.0000   ; 75        ; Europe/London ',
  "Moscow"    : 'Moscow    , U.S.S.R.;   55.7500 ;   37.7000   ; 120       ; Europe/Kaliningrad ',
  "NewDelhi"  : 'NewDelhi  , India   ;   28.6333 ;   77.2000   ; 235       ; Asia/Calcutta ',
  "Paris"     : 'Paris     , France  ;   48.8333 ;    2.3372   ; 91        ; Europe/Paris ',
  "Rome"      : 'Rome      , Italy   ;   41.8833 ;   12.5000   ; 29        ; Europe/Rome ',
  "Tokyo"     : 'Tokyo     , Japan   ;   35.5833 ;  139.7500   ; 9         ; Asia/Tokyo ',
  "Warsaw"    : 'Warsaw    , Poland  ;   52.2500 ;   21.0000   ; 110       ; Europe/Warsaw ',
}
