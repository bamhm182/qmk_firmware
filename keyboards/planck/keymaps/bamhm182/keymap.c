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

/* Setup */

#include QMK_KEYBOARD_H
#include "muse.h"
#include "eeprom.h"
#include "print.h"

extern rgb_config_t rgb_matrix_config;

enum planck_layers {
  _COLEMAK,
  _LOWER,
  _RAISE,
  _ADJUST
};

enum planck_keycodes {
  COLEMAK = SAFE_RANGE,
  BACKLIT
};

#define LOWER TT(_LOWER)
#define RAISE TT(_RAISE)

#define CLR_NON {  0,  0,  0}
#define CLR_RED {243,222,234}
#define CLR_PUR {169,120,255}
#define CLR_GRN { 85,203,158}
#define CLR_BLU {134,255,213}
#define CLR_ORG { 10,225,255}
#define CLR_YLW { 32,176,255}


/* Keymaps */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Template 
 *       1       2        3        4        5        6        7        8        9        10       11       12
 *   ,----------------------------------------------------------------------------------------------------------.
 * 1 |       |        |        |        |        |        |        |        |        |        |        |        |
 *   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * 2 |       |        |        |        |        |        |        |        |        |        |        |        |
 *   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * 3 |       |        |        |        |        |        |        |        |        |        |        |        |
 *   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * 4 |       |        |        |        |        |                 |        |        |        |        |        |
 *   `----------------------------------------------------------------------------------------------------------'
 */
/*
[_TEMPLATE = LAYOUT_planck_grid(
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
),
*/

/* Colemak
 *       1       2        3        4        5        6        7        8        9        10       11       12
 *   ,----------------------------------------------------------------------------------------------------------.
 * 1 | Tab   |   Q    |   W    |   F    |   P    |   G    |   J    |   L    |   U    |   Y    |   ;    | Esc    |
 *   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * 2 | Bksp  |   A    |   R    |   S    |   T    |   D    |   H    |   N    |   E    |   I    |   O    |  "     |
 *   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * 3 | Shift |   Z    |   X    |   C    |   V    |   B    |   K    |   M    |   ,    |   .    |   /    |Enter   |
 *   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * 4 | Brite | Ctrl   | Alt    | GUI    |Lower   |    Space        |Raise   | Left   | Down   |  Up    |Right   |
 *   `----------------------------------------------------------------------------------------------------------'
 */
[_COLEMAK] = LAYOUT_planck_grid(
      KC_TAB , KC_Q   , KC_W   , KC_F   , KC_P   , KC_G   , KC_J   , KC_L   , KC_U   , KC_Y   , KC_SCLN, KC_ESC ,
      KC_BSPC, KC_A   , KC_R   , KC_S   , KC_T   , KC_D   , KC_H   , KC_N   , KC_E   , KC_I   , KC_O   , KC_QUOT,
      KC_LSFT, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , KC_K   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, KC_ENT ,
      BACKLIT, KC_LCTL, KC_LALT, KC_LGUI, LOWER  , KC_SPC , KC_SPC , RAISE  , KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT
),

/* Lower
 *       1       2        3        4        5        6        7        8        9        10       11       12
 *   ,----------------------------------------------------------------------------------------------------------.
 * 1 |   ~   |        | MS_LC  | MS_U   |MS_RC   |        |        |        |        |   (    |   )    |        |
 *   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * 2 | Del   |        | MS_L   | MS_D   | MS_R   |        |        | MS_LC  | MS_RC  |   {    |   }    |  |     |
 *   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * 3 |       |        |        |        |        |        |        | PageDn | PageUp | Home   | End    |        |
 *   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * 4 |       |        |        |        |        |                 |        |        |        |        |        |
 *   `----------------------------------------------------------------------------------------------------------'
 */
[_LOWER] = LAYOUT_planck_grid(
      KC_TILD, _______, KC_BTN1, KC_MS_U, KC_BTN2, _______, _______, _______, _______, KC_LPRN, KC_RPRN, _______,
      KC_DEL , _______, KC_MS_L, KC_MS_D, KC_MS_R, _______, _______, KC_BTN1, KC_BTN2, KC_LCBR, KC_RCBR, KC_PIPE,
      _______, _______, _______, _______, _______, _______, _______, KC_PGDN, KC_PGUP, KC_HOME, KC_END,  _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),

/* Raise
 *       1       2        3        4        5        6        7        8        9        10       11       12
 *   ,----------------------------------------------------------------------------------------------------------.
 * 1 |   `   |        |        |        |        |        |        |   +    |   7    |   8    |   9    |  *     |
 *   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * 2 | Del   |        |        |        |        |        |        |   -    |   4    |   5    |   6    |  \     |
 *   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * 3 |       |        |        |        |        |        |        |   0    |   1    |   2    |   3    |        |
 *   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * 4 |       |        |        |        |        |                 |        |        |        |        |        |
 *   `----------------------------------------------------------------------------------------------------------'
 */
[_RAISE] = LAYOUT_planck_grid(
      KC_GRV , _______, _______, _______, _______, _______, _______, KC_PLUS, KC_7   , KC_8   , KC_9   , KC_ASTR,
      KC_DEL , _______, _______, _______, _______, _______, _______, KC_MINS, KC_4   , KC_5   , KC_6   , KC_SLSH,
      _______, _______, _______, _______, _______, _______, _______, KC_0   , KC_1   , KC_2   , KC_3   , _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),

/* Adjust (Lower + Raise)
 *       1       2        3        4        5        6        7        8        9        10       11       12
 *   ,----------------------------------------------------------------------------------------------------------.
 * 1 | AUtog | MIDtog | MUStog | MUSmod | RGBtog | RGBmod |        |        |        |        |        | Debug  |
 *   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * 2 | F1    | F2     | F3     | F4     | F5     | F6     |        | Vol-   | Vol+   | Skip   | Play   | Reset  |
 *   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * 3 | F7    | F8     | F9     | F10    | F11    | F12    |        |        |        |        |        |        |
 *   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * 4 |       |        |        |        |        |                 |        |        |        |        |        |
 *   `----------------------------------------------------------------------------------------------------------'
 */
[_ADJUST] = LAYOUT_planck_grid(
      AU_TOG , MI_TOG , MU_TOG , MU_MOD , RGB_TOG, RGB_MOD, _______, _______, _______, _______, _______, DEBUG,
      KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  , KC_F6  , _______, KC_VOLD, KC_VOLU, KC_MNXT, KC_MPLY, RESET  ,
      KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F11 , KC_F12 , _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
)

};

/* LED Colors */

const uint8_t PROGMEM ledmap[][DRIVER_LED_TOTAL][3] = {
    /* Lower */
    [1] = {
      CLR_RED, CLR_NON, CLR_GRN, CLR_BLU, CLR_GRN, CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_BLU, CLR_BLU, CLR_NON,
      CLR_RED, CLR_NON, CLR_BLU, CLR_BLU, CLR_BLU, CLR_NON, CLR_NON, CLR_GRN, CLR_GRN, CLR_BLU, CLR_BLU, CLR_BLU,
      CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_ORG, CLR_ORG, CLR_GRN, CLR_GRN, CLR_NON,
      CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_NON         , CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_NON
    },
    /* Raise */
    [2] = {
      CLR_RED, CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_PUR, CLR_BLU, CLR_BLU, CLR_BLU, CLR_PUR,
      CLR_RED, CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_PUR, CLR_BLU, CLR_BLU, CLR_BLU, CLR_PUR,
      CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_BLU, CLR_BLU, CLR_BLU, CLR_BLU, CLR_NON,
      CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_NON         , CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_NON
    },
    /* Adjust */
    [3] = {
      CLR_GRN, CLR_GRN, CLR_YLW, CLR_YLW, CLR_RED, CLR_RED, CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_ORG,
      CLR_BLU, CLR_BLU, CLR_BLU, CLR_BLU, CLR_BLU, CLR_BLU, CLR_NON, CLR_PUR, CLR_PUR, CLR_PUR, CLR_PUR, CLR_ORG,
      CLR_BLU, CLR_BLU, CLR_BLU, CLR_BLU, CLR_BLU, CLR_BLU, CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_NON,
      CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_NON         , CLR_NON, CLR_NON, CLR_NON, CLR_NON, CLR_NON
    }
};

/* Functions */

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
    case 1:
      set_layer_color(1);
      break;
    case 2:
      set_layer_color(2);
      break;
    case 3:
      set_layer_color(3);
      break;
   default:
    if (rgb_matrix_get_flags() == LED_FLAG_NONE)
      rgb_matrix_set_color_all(0, 0, 0);
    break;
  }
}

#ifdef AUDIO_ENABLE
  float plover_song[][2]     = SONG(PLOVER_SOUND);
  float plover_gb_song[][2]  = SONG(PLOVER_GOODBYE_SOUND);
#endif

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case BACKLIT:
      if (record->event.pressed) {
        register_code(KC_RSFT);
        #ifdef BACKLIGHT_ENABLE
          backlight_step();
        #endif
        #ifdef KEYBOARD_planck_rev5
          writePinLow(E6);
        #endif
      } else {
        unregister_code(KC_RSFT);
        #ifdef KEYBOARD_planck_rev5
          writePinHigh(E6);
        #endif
      }
      return false;
      break;
  }
  return true;
}

bool muse_mode = false;
uint8_t last_muse_note = 0;
uint16_t muse_counter = 0;
uint8_t muse_offset = 70;
uint16_t muse_tempo = 50;

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

bool dip_switch_update_user(uint8_t index, bool active) {
    switch (index) {
        case 0: {
#ifdef AUDIO_ENABLE
            static bool play_sound = false;
#endif
            if (active) {
#ifdef AUDIO_ENABLE
                if (play_sound) { PLAY_SONG(plover_song); }
#endif
                layer_on(_ADJUST);
            } else {
#ifdef AUDIO_ENABLE
                if (play_sound) { PLAY_SONG(plover_gb_song); }
#endif
                layer_off(_ADJUST);
            }
#ifdef AUDIO_ENABLE
            play_sound = true;
#endif
            break;
        }
        case 1:
            if (active) {
                muse_mode = true;
            } else {
                muse_mode = false;
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
