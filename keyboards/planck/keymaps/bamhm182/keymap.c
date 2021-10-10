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

/********** Setup **********/

/* Base */

#include QMK_KEYBOARD_H
#include "muse.h"
#include "eeprom.h"
#include "print.h"

/* Colors */

#define CLR____ {  0,  0,  0}
#define CLR_RED {243,222,234}
#define CLR_ORG { 10,225,255}
#define CLR_YLW { 32,176,255}
#define CLR_GRN { 85,203,158}
#define CLR_BLU {134,255,213}
#define CLR_PUR {169,120,255}

extern rgb_config_t rgb_matrix_config;

/* Layers */

#define LOWER   TT(_LOWER)
#define RAISE   TT(_RAISE)
#define GAMING  TO(_GAMING)
#define COLEMAK TO(_COLEMAK)

enum planck_layers {
  _COLEMAK,
  _LOWER,
  _RAISE,
  _ADJUST,
  _GAMING,
};

/* Music */

bool     muse_mode      = false;
uint8_t  last_muse_note = 0;
uint16_t muse_counter   = 0;
uint8_t  muse_offset    = 70;
uint16_t muse_tempo     = 50;

/* Tap Dance */

#define TD_SPC  TD(TD_SPC_TAB)
#define TD_LSFT TD(TD_LSFT_CAPS)
#define TD_LCTL TD(TD_LCTL_RCTL)

enum tap_dance {
  TD_SPC_TAB,
  TD_LSFT_CAPS,
  TD_CTL,
  TD_ALT,
  TD_GUI,
};

qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_SPC_TAB]   = ACTION_TAP_DANCE_DOUBLE(KC_SPC , KC_TAB ),
  [TD_LSFT_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_LSFT, KC_CAPS),
  [TD_CTL]       = ACTION_TAP_DANCE_DOUBLE(KC_LCTL, KC_RCTL),
  [TD_ALT]       = ACTION_TAP_DANCE_DOUBLE(KC_LALT, KC_RALT),
  [TD_GUI]       = ACTION_TAP_DANCE_DOUBLE(KC_LGUI, KC_RGUI),
};

/********** Keymaps **********/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Template 
       1       2        3        4        5        6        7        8        9        10       11       12
   ,----------------------------------------------------------------------------------------------------------.
 1 |       |        |        |        |        |        |        |        |        |        |        |        |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 2 |       |        |        |        |        |        |        |        |        |        |        |        |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 3 |       |        |        |        |        |        |        |        |        |        |        |        |
   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 4 |       |        |        |        |        |                 |        |        |        |        |        |
   `----------------------------------------------------------------------------------------------------------'
 */
/*
  [_TEMPLATE] = LAYOUT_planck_grid(
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
  ),
*/

/* Colemak
       1       2        3        4        5        6        7        8        9        10       11       12
   ,----------------------------------------------------------------------------------------------------------.
 1 | Esc   |   Q    |   W    |   F    |   P    |   G    |   J    |   L    |   U    |   Y    |  ; :   |        |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 2 | Bksp  |   A    |   R    |   S    |   T    |   D    |   H    |   N    |   E    |   I    |   O    |  ' "   |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 3 | Shift |   Z    |   X    |   C    |   V    |   B    |   K    |   M    |  , <   |  . >   |  / ?   | Enter  |
   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 4 | GAMING| Ctrl   | Alt    | GUI    | Lower  |      Space      | Raise  | Left   | Down   |  Up    | Right  |
   `----------------------------------------------------------------------------------------------------------'
 */
  [_COLEMAK] = LAYOUT_planck_grid(
    KC_ESC , KC_Q   , KC_W   , KC_F   , KC_P   , KC_G   , KC_J   , KC_L   , KC_U   , KC_Y   , KC_SCLN, _______,
    KC_BSPC, KC_A   , KC_R   , KC_S   , KC_T   , KC_D   , KC_H   , KC_N   , KC_E   , KC_I   , KC_O   , KC_QUOT,
    TD_LSFT, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , KC_K   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, KC_ENT ,
    GAMING , TD_CTL , TD_ALT , TD_GUI , LOWER  , TD_SPC , TD_SPC , RAISE  , KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT
  ),

/* Lower
       1       2        3        4        5        6        7        8        9        10       11       12
   ,----------------------------------------------------------------------------------------------------------.
 1 |       |        | MS_LC  | MS_U   | MS_RC  |        |        |  [ {   |  ] }   |  - _   |  = +   |        |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 2 | Del   |        | MS_L   | MS_D   | MS_R   |        |        | MS_LC  | MS_RC  |  ` ~   | PageUp | Home   |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 3 |       |        |        |        |        |        |        |   (    |   )    |  \ |   | PageDn | End    |
   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 4 |       |        |        |        |        |                 |        |        |        |        |        |
   `----------------------------------------------------------------------------------------------------------'
 */
  [_LOWER] = LAYOUT_planck_grid(
    _______, _______, KC_BTN1, KC_MS_U, KC_BTN2, _______, _______, KC_LBRC, KC_RBRC, KC_MINS, KC_EQL , _______,
    KC_DEL , _______, KC_MS_L, KC_MS_D, KC_MS_R, _______, _______, KC_BTN1, KC_BTN2, KC_GRV , KC_PGUP, KC_HOME,
    _______, _______, _______, _______, _______, _______, _______, KC_LPRN, KC_RPRN, KC_BSLS, KC_PGDN, KC_END ,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
  ),

/* Raise
       1       2        3        4        5        6        7        8        9        10       11       12
   ,----------------------------------------------------------------------------------------------------------.
 1 |       |        |        |        |        |        |        |        |  7 &   |  8 *   |  9 (   |  0 )   |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 2 |       |        |        |        |        |        |        |   +    |  4 $   |  5 %   |  6 ^   |   *    |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 3 |       |        |        |        |        |        |        |  - _   |  1 !   |  2 @   |  3 #   |  / ?   |
   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 4 |       |        |        |        |        |                 |        |  . >   |  0 )   |  , >   |        |
   `----------------------------------------------------------------------------------------------------------'
 */
  [_RAISE] = LAYOUT_planck_grid(
    _______, _______, _______, _______, _______, _______, _______, _______, KC_7   , KC_8   , KC_9   , KC_0   ,
    _______, _______, _______, _______, _______, _______, _______, KC_PLUS, KC_4   , KC_5   , KC_6   , KC_ASTR,
    _______, _______, _______, _______, _______, _______, _______, KC_MINS, KC_1   , KC_2   , KC_3   , KC_SLSH,
    _______, _______, _______, _______, _______, _______, _______, _______, KC_DOT , KC_0   , KC_COMM, _______
  ),

/* Adjust (Lower + Raise)
       1       2        3        4        5        6        7        8        9        10       11       12
   ,----------------------------------------------------------------------------------------------------------.
 1 | AUtog | MIDtog | MUStog | MUSmod | RGBtog | RGBmod |        |        |        |        |        | Debug  |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 2 | F1    | F2     | F3     | F4     | F5     | F6     |        | Vol-   | Vol+   | Skip   | Play   | Reset  |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 3 | F7    | F8     | F9     | F10    | F11    | F12    |        |        |        |        |        |        |
   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 4 |       |        |        |        |        |                 |        |        |        |        |        |
   `----------------------------------------------------------------------------------------------------------'
 */
  [_ADJUST] = LAYOUT_planck_grid(
    AU_TOG , MI_TOG , MU_TOG , MU_MOD , RGB_TOG, RGB_MOD, _______, _______, _______, _______, _______, DEBUG  ,
    KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  , KC_F6  , _______, KC_VOLD, KC_VOLU, KC_MNXT, KC_MPLY, RESET  ,
    KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F11 , KC_F12 , _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
  ),

/* Gaming 
       1       2        3        4        5        6        7        8        9        10       11       12
   ,----------------------------------------------------------------------------------------------------------.
 1 |       |        | MS_LC  | MS_U   | MS_RC  |        |        |   Q    |   W    |   E    |        |        |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 2 |       |        | MS_L   | MS_D   | MS_R   |        |        |   A    |   S    |   D    |        |        |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 3 |       |        |        |        |        |        |        |        |        |        |        |        |
   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 4 |COLEMAK|        |        |        |        |                 |        |        |        |        |        |
   `----------------------------------------------------------------------------------------------------------'
 */
  [_GAMING] = LAYOUT_planck_grid(
    _______, _______, KC_BTN1, KC_MS_U, KC_BTN2, _______, _______, KC_Q   , KC_W   , KC_E   , _______, _______,
    _______, _______, KC_MS_L, KC_MS_D, KC_MS_R, _______, _______, KC_A   , KC_S   , KC_D   , _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    COLEMAK, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
  ),
};

/* LED Colormaps */

const uint8_t PROGMEM ledmap[][DRIVER_LED_TOTAL][3] = {
  /*
  [X] = {
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____         , CLR____, CLR____, CLR____, CLR____, CLR____,
  },
  */
  [_LOWER] = {
    CLR____, CLR____, CLR_GRN, CLR_BLU, CLR_GRN, CLR____, CLR____, CLR_BLU, CLR_BLU, CLR_PUR, CLR_PUR, CLR____,
    CLR_ORG, CLR____, CLR_BLU, CLR_BLU, CLR_BLU, CLR____, CLR____, CLR_GRN, CLR_GRN, CLR_PUR, CLR_ORG, CLR_ORG,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR_BLU, CLR_BLU, CLR_PUR, CLR_ORG, CLR_ORG,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____         , CLR____, CLR____, CLR____, CLR____, CLR____
  },
  [_RAISE] = {
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR_BLU, CLR_BLU, CLR_BLU, CLR_BLU,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR_PUR, CLR_BLU, CLR_BLU, CLR_BLU, CLR_PUR,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR_PUR, CLR_BLU, CLR_BLU, CLR_BLU, CLR_PUR,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____         , CLR____, CLR_GRN, CLR_BLU, CLR_GRN, CLR____
  },
  [_ADJUST] = {
    CLR_GRN, CLR_GRN, CLR_YLW, CLR_YLW, CLR_RED, CLR_RED, CLR____, CLR____, CLR____, CLR____, CLR____, CLR_RED,
    CLR_BLU, CLR_BLU, CLR_BLU, CLR_BLU, CLR_BLU, CLR_BLU, CLR____, CLR_PUR, CLR_PUR, CLR_PUR, CLR_PUR, CLR_RED,
    CLR_BLU, CLR_BLU, CLR_BLU, CLR_BLU, CLR_BLU, CLR_BLU, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____         , CLR____, CLR____, CLR____, CLR____, CLR____
  },
  [_GAMING] = {
    CLR____, CLR____, CLR_BLU, CLR_GRN, CLR_BLU, CLR____, CLR____, CLR_BLU, CLR_GRN, CLR_BLU, CLR____, CLR____,
    CLR____, CLR____, CLR_GRN, CLR_GRN, CLR_GRN, CLR____, CLR____, CLR_GRN, CLR_GRN, CLR_GRN, CLR____, CLR____,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,
    CLR_ORG, CLR____, CLR____, CLR____, CLR____, CLR____         , CLR____, CLR____, CLR____, CLR____, CLR____,
  },
};

/********** Functions **********/

/* Layers */

layer_state_t layer_state_set_kb(layer_state_t state) {
  planck_ez_left_led_off();
  planck_ez_right_led_off();
  
  state = layer_state_set_user(state);
  uint8_t layer = biton32(state);
  switch (layer) {
    case PLANCK_EZ_LED_LOWER:
      planck_ez_left_led_on();
      break;
    case PLANCK_EZ_LED_RAISE:
      planck_ez_right_led_on();
      break;
    case PLANCK_EZ_LED_ADJUST:
      planck_ez_right_led_on();
      planck_ez_left_led_on();
      break;
    default:
      break;
  }
  return state;
}

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

/* LEDs */

void keyboard_post_init_user(void) {
  rgb_matrix_enable();
}

void set_layer_color(int layer) {
  for (int i = 0; i < DRIVER_LED_TOTAL; i++) {
    HSV hsv = {
      .h = pgm_read_byte(&ledmap[layer][i][0]),
      .s = pgm_read_byte(&ledmap[layer][i][1]),
      .v = pgm_read_byte(&ledmap[layer][i][2]),
    };
    if (!hsv.h && !hsv.s && !hsv.v) {
        rgb_matrix_set_color( i, 0, 0, 0 );
    } else {
        RGB rgb = hsv_to_rgb( hsv );
        float f = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
        rgb_matrix_set_color( i, f * rgb.r, f * rgb.g, f * rgb.b );
    }
  }
}

void rgb_matrix_indicators_user(void) {
  if (keyboard_config.disable_layer_led) { return; }
  switch (biton32(layer_state)) {
    case _LOWER:
      set_layer_color(_LOWER);
      break;
    case _RAISE:
      set_layer_color(_RAISE);
      break;
    case _ADJUST:
      set_layer_color(_ADJUST);
      break;
    case _GAMING:
      set_layer_color(_GAMING);
      break;
   default:
    if (rgb_matrix_get_flags() == LED_FLAG_NONE)
      rgb_matrix_set_color_all(0, 0, 0);
    break;
  }
}

/* Music */

bool encoder_update_user(uint8_t index, bool clockwise) {
  if (muse_mode) {
    if (IS_LAYER_ON(_RAISE)) {
      if (clockwise) {
        muse_offset++;
      } else {
        muse_offset--;
      }
    } else {
      if (clockwise) {
        muse_tempo+=1;
      } else {
        muse_tempo-=1;
      }
    }
  } else {
    if (clockwise) {
#ifdef MOUSEKEY_ENABLE
      tap_code(KC_MS_WH_DOWN);
#else
      tap_code(KC_PGDN);
#endif
    } else {
#ifdef MOUSEKEY_ENABLE
      tap_code(KC_MS_WH_UP);
#else
      tap_code(KC_PGUP);
#endif
    }
  }
    return true;
}

void matrix_scan_user(void) {
#ifdef AUDIO_ENABLE
  if (muse_mode) {
    if (muse_counter == 0) {
      uint8_t muse_note = muse_offset + SCALE[muse_clock_pulse()];
      if (muse_note != last_muse_note) {
        stop_note(compute_freq_for_midi_note(last_muse_note));
        play_note(compute_freq_for_midi_note(muse_note), 0xF);
        last_muse_note = muse_note;
      }
    }
    muse_counter = (muse_counter + 1) % muse_tempo;
  } else {
    if (muse_counter) {
      stop_all_notes();
      muse_counter = 0;
    }
  }
#endif
}

bool music_mask_user(uint16_t keycode) {
  switch (keycode) {
    case RAISE:
    case LOWER:
      return false;
    default:
      return true;
  }
}
