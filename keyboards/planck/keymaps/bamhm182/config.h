/* Copyright 2015-2021 Jack Humbert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#define UPGRADE_SOUND \
  S__NOTE(_C5 ), S__NOTE(_G4 ), S__NOTE(_C5 ), S__NOTE(_E5 ), S__NOTE(_G5 ), S__NOTE(_C6 ), S__NOTE(_G5 ), S__NOTE(_GS4), \
  S__NOTE(_C5 ), S__NOTE(_DS5), S__NOTE(_GS5), S__NOTE(_DS5), S__NOTE(_GS5), S__NOTE(_C6 ), S__NOTE(_DS6), S__NOTE(_GS6), \
  S__NOTE(_DS6), S__NOTE(_AS4), S__NOTE(_D5 ), S__NOTE(_F5 ), S__NOTE(_AS5), S__NOTE(_D6 ), S__NOTE(_F6 ), S__NOTE(_AS6), \
  S__NOTE(_F6 )

#ifdef AUDIO_ENABLE
  #define STARTUP_SONG SONG(NO_SOUND)
  #define GOODBYE_SONG SONG(UPGRADE_SOUND)
#endif

#define MIDI_BASIC

#define ENCODER_RESOLUTION 4

#define TAPPING_TERM 200

#define ORYX_CONFIGURATOR
#define PLANCK_EZ_USER_LEDS

#define PLANCK_EZ_LED_LOWER  1
#define PLANCK_EZ_LED_RAISE  2
#define PLANCK_EZ_LED_ADJUST 3

#undef  MOUSEKEY_INTERVAL
#define MOUSEKEY_INTERVAL 16
