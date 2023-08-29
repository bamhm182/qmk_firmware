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

#define CLR____[3] {0,0,0}
#define CLR_RED {  0,255,255}
#define CLR_ORG { 28,255,255}
#define CLR_YLW { 40,255,255}
#define CLR_GRN { 83,255,214}
#define CLR_BLU {134,255,213}
#define CLR_PUR {198,255,255}
#define CLR_PNK {243,222,234}
#define CLR_TAN { 10,225,255}

extern rgb_config_t rgb_matrix_config;

/* Layers */

#define LOWER   MO(_LOWER)
#define RAISE   MO(_RAISE)
#define SHOOTER TT(_SHOOTER)
#define COLEMAK DF(_COLEMAK)
#define QWERTY  DF(_QWERTY)
#define OUTSHIN TT(_OUTSHINE)
#define CDL     MO(_CDL)

enum planck_layers {
  _COLEMAK,
  _LOWER,
  _RAISE,
  _ADJUST,
  _SHOOTER,
  _QWERTY,
  _OUTSHINE,
  _CDL
};

/* Custom Keycodes */

bool android_guac_mode = false;

enum my_keycodes {
  KA_BTN1 = SAFE_RANGE + 10,
  KA_BSPC,
  KA_TOGG,
  MUS_RST
};

/* Music */

bool     muse_mode      = false;
uint8_t  last_muse_note = 0;
uint16_t muse_counter   = 0;
uint8_t  muse_offset    = 70;
uint16_t muse_tempo     = 50;

float upgrade_song[][2] = SONG(UPGRADE_SOUND);
float change_song[][2] = SONG(CHANGE_SOUND);

/* Tap Dance */

#define TD_SPC  TD(TD_SPC_TAB)
#define TD_LSFT TD(TD_LSFT_CAPS)
#define TD_CTL  TD(TD_LRCTL)
#define TD_ALT  TD(TD_LRALT)
#define TD_GUI  TD(TD_LRGUI)
#define TD_MSPC TD(TD_MS_B1_B2)
#define TD_MWDU TD(TD_WHD_WHU)
#define TD_MWRL TD(TD_WHR_WHL)

enum tap_dance {
  TD_SPC_TAB,
  TD_LSFT_CAPS,
  TD_LRCTL,
  TD_LRALT,
  TD_LRGUI,
  TD_MS_B1_B2,
  TD_WHD_WHU,
  TD_WHR_WHL,
};

qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_SPC_TAB]   = ACTION_TAP_DANCE_DOUBLE(KC_SPC , KC_TAB ),
  [TD_LSFT_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_LSFT, KC_CAPS),
  [TD_LRCTL]     = ACTION_TAP_DANCE_DOUBLE(KC_LCTL, KC_RCTL),
  [TD_LRALT]     = ACTION_TAP_DANCE_DOUBLE(KC_LALT, KC_RALT),
  [TD_LRGUI]     = ACTION_TAP_DANCE_DOUBLE(KC_LGUI, KC_RGUI),
  [TD_MS_B1_B2]  = ACTION_TAP_DANCE_DOUBLE(KC_BTN1, KC_BTN2),
  [TD_WHD_WHU]   = ACTION_TAP_DANCE_DOUBLE(KC_WH_D, KC_WH_U),
  [TD_WHR_WHL]   = ACTION_TAP_DANCE_DOUBLE(KC_WH_R, KC_WH_L),
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
 1 | Esc   |   Q    |   W    |   F    |   P    |   B    |   J    |   L    |   U    |   Y    |  ; :   |        |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 2 | Bksp  |   A    |   R    |   S    |   T    |   G    |   M    |   N    |   E    |   I    |   O    |  ' "   |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 3 | Shift |   Z    |   X    |   C    |   D    |   V    |   K    |   H    |  , <   |  . >   |  / ?   | Enter  |
   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 4 | CDL   | Ctrl   | Alt    | GUI    | Lower  |     SPC/TAB     | Raise  | Left   | Down   |  Up    | Right  |
   `----------------------------------------------------------------------------------------------------------'
 */
  [_COLEMAK] = LAYOUT_planck_grid(
    KC_ESC , KC_Q   , KC_W   , KC_F   , KC_P   , KC_B   , KC_J   , KC_L   , KC_U   , KC_Y   , KC_SCLN, _______,
    KA_BSPC, KC_A   , KC_R   , KC_S   , KC_T   , KC_G   , KC_M   , KC_N   , KC_E   , KC_I   , KC_O   , KC_QUOT,
    KC_LSFT, KC_Z   , KC_X   , KC_C   , KC_D   , KC_V   , KC_K   , KC_H   , KC_COMM, KC_DOT , KC_SLSH, KC_ENT ,
    CDL    , TD_CTL , TD_ALT , TD_GUI , LOWER  , TD_SPC , TD_SPC , RAISE  , KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT
  ),

/* Lower
       1       2        3        4        5        6        7        8        9        10       11       12
   ,----------------------------------------------------------------------------------------------------------.
 1 |       |        | MS_WHRL| MS_U   | MS_WHDU|        |        |   (    |   {    |  [ {   |        |        |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 2 | Del   |        | MS_L   | MS_D   | MS_R   |        |        |   )    |   }    |  ] }   |        |        |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 3 |       |        |        |        |        | KA_BTN1|        |  ` ~   |  - _   |  = +   |  \ |   |        |
   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 4 |       |        |        |        |        |    MS_LC_RC     | Home   | PgDwn  | PgUp   | End    |        |
   `----------------------------------------------------------------------------------------------------------'
 */
  [_LOWER] = LAYOUT_planck_grid(
    _______, _______, TD_MWRL, KC_MS_U, TD_MWDU, _______, _______, KC_LPRN, KC_LCBR, KC_LBRC, _______, _______,
    KC_DEL , _______, KC_MS_L, KC_MS_D, KC_MS_R, _______, _______, KC_RPRN, KC_RCBR, KC_RBRC, _______, _______,
    _______, _______, ______ , _______, _______, KA_BTN1, _______, KC_GRV , KC_MINS, KC_EQL , KC_BSLS, _______,
    _______, _______, _______, _______, _______, TD_MSPC, TD_MSPC, KC_HOME, KC_PGDN, KC_PGUP, KC_END , _______
  ),

/* Raise
       1       2        3        4        5        6        7        8        9        10       11       12
   ,----------------------------------------------------------------------------------------------------------.
 1 |       |        |   A    |   B    |   C    |        |        |  7 &   |  8 *   |  9 (   |  0 )   |        |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 2 |       |   :    |   D    |   E    |   F    |        |        |  4 $   |  5 %   |  6 ^   |  . >   |        |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 3 |       |   *    |  / ?   |   +    |  - _   |        |        |  1 !   |  2 @   |  3 #   |  , <   |        |
   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 4 |       |        |        |        |        |                 |        |        |        |        |        |
   `----------------------------------------------------------------------------------------------------------'
 */
  [_RAISE] = LAYOUT_planck_grid(
    _______, _______, KC_A   , KC_B   , KC_C   , _______, _______, KC_7   , KC_8   , KC_9   , KC_0   , _______,
    _______, KC_COLN, KC_D   , KC_E   , KC_F   , _______, _______, KC_4   , KC_5   , KC_6   , KC_DOT , _______,
    _______, KC_ASTR, KC_SLSH, KC_PLUS, KC_MINS, _______, _______, KC_1   , KC_2   , KC_3   , KC_COMM, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
  ),

/* Adjust (Lower + Raise)
       1       2        3        4        5        6        7        8        9        10       11       12
   ,----------------------------------------------------------------------------------------------------------.
 1 | F1    | F2     | F3     | F4     | F5     | F6     | F7     | F8     | F9     | F10    | F11    | F12    |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 2 |       |        |        | MUTog  | RGBTog |        |        | Vol-   | Vol+   | Skip   | Play   | Reset  |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 3 |       |        |        | MUMod  | RGBMod |        |        |        |        |        |        | Debug  | 
   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 4 | ASTog |        |        |        |        |                 |        |        |        |        |        |
   `----------------------------------------------------------------------------------------------------------'
 */
  [_ADJUST] = LAYOUT_planck_grid(
    KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  , KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F11 , KC_F12 ,
    _______, _______, _______, MU_TOG , RGB_TOG, _______, _______, KC_VOLD, KC_VOLU, KC_MNXT, KC_MPLY, MUS_RST,
    _______, _______, _______, MU_MOD , RGB_MOD, _______, _______, _______, _______, _______, _______, DEBUG  ,
    KC_ASTG, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
  ),

/* QWERTY
       1       2        3        4        5        6        7        8        9        10       11       12
   ,----------------------------------------------------------------------------------------------------------.
 1 | Esc   |   Q    |   W    |   E    |   R    |   T    |   Y    |   U    |   I    |   O    |   P    |        |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 2 | Bksp  |   A    |   S    |   D    |   F    |   G    |   H    |   J    |   K    |   L    |  ; :   |  ' "   |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 3 | Shift |   Z    |   X    |   C    |   V    |   B    |   N    |   M    |  , <   |  . >   |  / ?   | Enter  |
   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 4 | CDL   | Ctrl   | Alt    | GUI    | Lower  |      Space      | Raise  | Left   | Down   |  Up    | Right  |
   `----------------------------------------------------------------------------------------------------------'
 */
  [_QWERTY] = LAYOUT_planck_grid(
    KC_ESC , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   , KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , _______,
    KA_BSPC, KC_A   , KC_S   , KC_D   , KC_F   , KC_G   , KC_H   , KC_J   , KC_K   , KC_L   , KC_SCLN, KC_QUOT,
    TD_LSFT, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, KC_ENT ,
    CDL    , TD_CTL , TD_ALT , TD_GUI , LOWER  , TD_SPC , TD_SPC , RAISE  , KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT
  ),

/* Shooter Games 
       1       2        3        4        5        6        7        8        9        10       11       12
   ,----------------------------------------------------------------------------------------------------------.
 1 |       |        | MS_LC  | MS_U   | MS_RC  |        |        |   Q    |   W    |   E    |        |        |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 2 |       |        | MS_L   | MS_D   | MS_R   |        |        |   A    |   S    |   D    |        |        |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 3 |       |        |        |        |        |        |        |        |        |        |        |        |
   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 4 | CDL   |        |        |        |        |                 |        |        |        |        |        |
   `----------------------------------------------------------------------------------------------------------'
 */
  [_SHOOTER] = LAYOUT_planck_grid(
    _______, _______, KC_BTN1, KC_MS_U, KC_BTN2, _______, _______, KC_Q   , KC_W   , KC_E   , _______, _______,
    _______, _______, KC_MS_L, KC_MS_D, KC_MS_R, _______, _______, KC_A   , KC_S   , KC_D   , _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    CDL    , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
  ),

/* Outshine Game Mods
       1       2        3        4        5        6        7        8        9        10       11       12
   ,----------------------------------------------------------------------------------------------------------.
 1 |       |        |        |        |        |        |        |        |        |        |        |        |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 2 |       |        |        |        |        |        |        |        |        |        |        |        |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 3 |       |        |        |        |        |        |        |        |        |        |        |        |
   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 4 | CDL   |        |        | L_SHFT |        |                 |        | R_SHFT |        |        |        |
   `----------------------------------------------------------------------------------------------------------'
 */

  [_OUTSHINE] = LAYOUT_planck_grid(
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    CDL    , _______, _______, KC_LSFT, _______, _______, _______, _______, KC_RSFT, _______, _______, _______
  ),

/* Change Default Layout (CDL)
       1       2        3        4        5        6        7        8        9        10       11       12
   ,----------------------------------------------------------------------------------------------------------.
 1 |       | QWERTY |        |        |        |        |        |        |        |        |        |        |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 2 |       | KATog  |        | SHOOTER|        |        |        |        |        |        |OUTSHINE|        |
   |-------+--------+--------+--------*========*--------+--------*========*--------+--------+--------+--------|
 3 |       |        |        | COLEMAK|        |        |        |        |        |        |        |        |
   |-------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 4 |       |        |        |        |        |                 |        |        |        |        |        |
   `----------------------------------------------------------------------------------------------------------'
 */
  [_CDL] = LAYOUT_planck_grid(
    _______, QWERTY , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, KA_TOGG, _______, SHOOTER, _______, _______, _______, _______, _______, _______, OUTSHIN, _______,
    _______, _______, _______, COLEMAK, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
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
    CLR____, CLR____, CLR_ORG, CLR_BLU, CLR_ORG, CLR____, CLR____, CLR_BLU, CLR_BLU, CLR_BLU, CLR____, CLR____,
    CLR_ORG, CLR____, CLR_BLU, CLR_BLU, CLR_BLU, CLR____, CLR____, CLR_BLU, CLR_BLU, CLR_BLU, CLR____, CLR____,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR_GRN, CLR____, CLR_PUR, CLR_PUR, CLR_PUR, CLR_PUR, CLR____,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR_GRN         , CLR_ORG, CLR_ORG, CLR_ORG, CLR_ORG, CLR____
  },
  [_RAISE] = {
    CLR____, CLR____, CLR_BLU, CLR_BLU, CLR_BLU, CLR____, CLR____, CLR_BLU, CLR_BLU, CLR_BLU, CLR_BLU, CLR____,
    CLR____, CLR_PUR, CLR_BLU, CLR_BLU, CLR_BLU, CLR____, CLR____, CLR_BLU, CLR_BLU, CLR_BLU, CLR_PUR, CLR____,
    CLR____, CLR_PUR, CLR_PUR, CLR_PUR, CLR_PUR, CLR____, CLR____, CLR_BLU, CLR_BLU, CLR_BLU, CLR_PUR, CLR____,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____         , CLR____, CLR____, CLR____, CLR____, CLR____
  },
  [_ADJUST] = {
    CLR_ORG, CLR_ORG, CLR_ORG, CLR_ORG, CLR_ORG, CLR_ORG, CLR_ORG, CLR_ORG, CLR_ORG, CLR_ORG, CLR_ORG, CLR_ORG,
    CLR____, CLR____, CLR____, CLR_BLU, CLR_RED, CLR____, CLR____, CLR_PUR, CLR_PUR, CLR_PUR, CLR_PUR, CLR_GRN,
    CLR____, CLR____, CLR____, CLR_BLU, CLR_RED, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR_GRN,
    CLR_RED, CLR____, CLR____, CLR____, CLR____, CLR____         , CLR____, CLR____, CLR____, CLR____, CLR____
  },
  [_SHOOTER] = {
    CLR____, CLR____, CLR_BLU, CLR_GRN, CLR_BLU, CLR____, CLR____, CLR_BLU, CLR_GRN, CLR_BLU, CLR____, CLR____,
    CLR____, CLR____, CLR_GRN, CLR_GRN, CLR_GRN, CLR____, CLR____, CLR_GRN, CLR_GRN, CLR_GRN, CLR____, CLR____,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,
    CLR_ORG, CLR____, CLR____, CLR____, CLR____, CLR____         , CLR____, CLR____, CLR____, CLR____, CLR____,
  },
  [_CDL] = {
    CLR____, CLR_PUR, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,
    CLR____, CLR_PUR, CLR____, CLR_PUR, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR_PUR, CLR____,
    CLR____, CLR____, CLR____, CLR_PUR, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____         , CLR____, CLR____, CLR____, CLR____, CLR____,
  },
};

/********** Functions **********/

/* Key Presses */

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
        case KA_BTN1:
            if (record->event.pressed) {
                if (android_guac_mode) {
                    SEND_STRING(SS_TAP(X_MS_L));
                    SEND_STRING(SS_TAP(X_MS_R));
                    unregister_code(KC_BTN1);
                }
                register_code(KC_BTN1);
            } else {
                if (android_guac_mode) {
                    SEND_STRING(SS_DELAY(100));
                }
                unregister_code(KC_BTN1);
            }
            break;
        case KA_BSPC:
            if (record->event.pressed) {
                if (android_guac_mode) {
                    SEND_STRING(SS_TAP(X_LEFT));
                    SEND_STRING(SS_TAP(X_DEL));
                } else {
                    register_code(KC_BSPC);
                }
            } else if(!android_guac_mode) {
                unregister_code(KC_BSPC);
            }
            break;
        case KA_TOGG:
            if (record->event.pressed) {
                android_guac_mode = !android_guac_mode;
                PLAY_SONG(change_song);
            }
            break;
        case MUS_RST:
            if (record->event.pressed) {
                PLAY_SONG(upgrade_song);
                SEND_STRING(SS_DELAY(800));
                reset_keyboard();
            }
            break;
    }
    return true;
};

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
    case _SHOOTER:
      set_layer_color(_SHOOTER);
      break;
    case _CDL:
      set_layer_color(_CDL);
      break;
    case _COLEMAK:
    case _QWERTY:
      if (rgb_matrix_get_flags() == LED_FLAG_NONE) {
        rgb_matrix_set_color_all(0, 0, 0);
      }
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
