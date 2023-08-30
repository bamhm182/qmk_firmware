#pragma once

/* Audio & Songs */
#define UPGRADE_SOUND \
      S__NOTE(_C5 ), S__NOTE(_G4 ), S__NOTE(_C5 ), S__NOTE(_E5 ), S__NOTE(_G5 ), S__NOTE(_C6 ), S__NOTE(_G5 ), S__NOTE(_GS4), \
        S__NOTE(_C5 ), S__NOTE(_DS5), S__NOTE(_GS5), S__NOTE(_DS5), S__NOTE(_GS5), S__NOTE(_C6 ), S__NOTE(_DS6), S__NOTE(_GS6), \
          S__NOTE(_DS6), S__NOTE(_AS4), S__NOTE(_D5 ), S__NOTE(_F5 ), S__NOTE(_AS5), S__NOTE(_D6 ), S__NOTE(_F6 ), S__NOTE(_AS6), \
            S__NOTE(_F6 )
#define CHANGE_SOUND S__NOTE(_C3)

#ifdef AUDIO_ENABLE
  #define STARTUP_SONG SONG(NO_SOUND)
  #define GOODBYE_SONG SONG(NO_SOUND)
#endif

/* Tapping Toggle */
#define TAPPING_TERM 200
#undef  TAPPING_TOGGLE
#define TAPPING_TOGGLE 2

/* Auto Shift */
#define AUTO_SHIFT_TIMEOUT 175
//#define AUTO_SHIFT_NO_SETUP

/* Layer Lights */
#define ORYX_CONFIGURATOR

/* Mouse Keys */
#undef  MOUSEKEY_INTERVAL
#define MOUSEKEY_INTERVAL 16
