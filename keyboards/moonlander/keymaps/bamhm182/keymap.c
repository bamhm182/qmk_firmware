/********** SETUP **********/

/* Base */

#include QMK_KEYBOARD_H
#include "version.h"
#include "muse.h"
#include "eeprom.h"
#include "print.h"

/* Colors */

#define CLR____ {  0,  0,  0}
#define CLR_RED {  0,255,255}
#define CLR_ORG { 28,255,255}
#define CLR_YLW { 40,255,255}
#define CLR_GRN { 83,255,214}
#define CLR_BLU {134,255,213}
#define CLR_PUR {198,255,255}
#define CLR_PNK {243,222,234}
#define CLR_TAN { 10,225,255}

/* Music */

bool     muse_mode      = false;
uint8_t  last_muse_note = 0;
uint16_t muse_counter   = 0;
uint8_t  muse_offset    = 70;
uint16_t muse_tempo     = 50;

float upgrade_song[][2] = SONG(UPGRADE_SOUND);
float change_song[][2] = SONG(CHANGE_SOUND);

/* Tap Dance */

enum tap_dance {
  TD_SPC_TAB,
  TD_LSFT_CAPS,
  TD_LRCTL,
  TD_LRALT,
  TD_LRGUI,
  TD_MS_B1_B2,
  TD_WHD_WHU,
  TD_WHR_WHL,
  TD_LR_PRNS,
  TD_LR_CBRS,
  TD_LR_BRCS
};

tap_dance_action_t tap_dance_actions[] = {
  [TD_SPC_TAB]   = ACTION_TAP_DANCE_DOUBLE(KC_SPC , KC_TAB ),
  [TD_LSFT_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_LSFT, KC_CAPS),
  [TD_LRCTL]     = ACTION_TAP_DANCE_DOUBLE(KC_LCTL, KC_RCTL),
  [TD_LRALT]     = ACTION_TAP_DANCE_DOUBLE(KC_LALT, KC_RALT),
  [TD_LRGUI]     = ACTION_TAP_DANCE_DOUBLE(KC_LGUI, KC_RGUI),
  [TD_MS_B1_B2]  = ACTION_TAP_DANCE_DOUBLE(KC_BTN1, KC_BTN2),
  [TD_WHD_WHU]   = ACTION_TAP_DANCE_DOUBLE(KC_WH_D, KC_WH_U),
  [TD_WHR_WHL]   = ACTION_TAP_DANCE_DOUBLE(KC_WH_R, KC_WH_L),
  [TD_LR_PRNS]   = ACTION_TAP_DANCE_DOUBLE(KC_LPRN, KC_RPRN),
  [TD_LR_CBRS]   = ACTION_TAP_DANCE_DOUBLE(KC_LCBR, KC_RCBR),
  [TD_LR_BRCS]   = ACTION_TAP_DANCE_DOUBLE(KC_LBRC, KC_RBRC),
};

#define TD_SPC  TD(TD_SPC_TAB)
#define TD_SHFT TD(TD_LSFT_CAPS)
#define TD_CTL  TD(TD_LRCTL)
#define TD_ALT  TD(TD_LRALT)
#define TD_GUI  TD(TD_LRGUI)
#define TD_MSPC TD(TD_MS_B1_B2)
#define TD_MWDU TD(TD_WHD_WHU)
#define TD_MWRL TD(TD_WHR_WHL)
#define TD_PRNS TD(TD_LR_PRNS)
#define TD_CBRS TD(TD_LR_CBRS)
#define TD_BRCS TD(TD_LR_BRCS)

/* Custom Keycodes */

enum my_keycodes {
      MUS_RST
};

/* Layers */

#define LOWER  MO(_LOWER)
#define RAISE  MO(_RAISE)

enum layers {
    _COLEMAK,
    _LOWER,
    _RAISE,
    _ADJUST,
    _QWERTY,
    _CDL
};

enum custom_keycodes {
    VRSN = SAFE_RANGE,
};

/********** Keymaps **********/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Template
       1       2        3        4        5        6        7                7        6        5        4        3        2        1
   ,-------------------------------------------------------------.        ,--------------------------------------------------------------.
 1 |       |        |        |        |        |        |        |        |        |        |        |        |        |        |        | 1
   |-------+--------+--------+--------+--------+--------+--------|        |--------+--------+--------+--------+--------+--------+--------|
 2 |       |        |        |        |        |        |        |        |        |        |        |        |        |        |        | 2
   |-------+--------+--------+--------+========+--------+--------|        |--------+--------+========+--------+--------+--------+--------|
 3 |       |        |        |        |        |        |        |        |        |        |        |        |        |        |        | 3
   |-------+--------+--------+--------+========+--------+--------'        `--------+--------+========+--------+--------+--------+--------|
 4 |       |        |        |        |        |        |                          |        |        |        |        |        |        | 4
   |-------+--------+--------+--------+--------+--------,                          `--------+--------+--------+--------+--------+--------|
 5 |       |        |        |        |        | ,--------.                     ,--------.  |        |        |        |        |        | 5
   `-------------------------------------------' |        |                     |        |  `--------------------------+--------+--------'
                                        ,--------+--------+--------.   ,--------+--------+--------.
                                        |        |        |        |   |        |        |        |
                                        `--------------------------'   `--------------------------'
*/
/*
  [_TEMPLATE] = LAYOUT_moonlander(
    _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______,   _______,                       _______,    _______, _______, _______, _______, _______,
                                          _______, _______, _______,     _______, _______, _______
  ),
*/

/* Colemak
       1       2        3        4        5        6        7                7        6        5        4        3        2        1
   ,-------------------------------------------------------------.       ,--------------------------------------------------------------.
 1 |       |        |        |        |        |        |        |       |        |        |        |        |        |        |        | 1
   |-------+--------+--------+--------+--------+--------+--------|       |--------+--------+--------+--------+--------+--------+--------|
 2 | Esc   |   Q    |   W    |   F    |   P    |   B    |        |       |        |   J    |   L    |   U    |   Y    |  ; :   |        | 2
   |-------+--------+--------+--------+========+--------+--------|       |--------+--------+========+--------+--------+--------+--------|
 3 | Bksp  |   A    |   R    |   S    |   T    |   G    |        |       |        |   M    |   N    |   E    |   I    |   O    | Enter  | 3
   |-------+--------+--------+--------+========+--------+--------'       `--------+--------+========+--------+--------+--------+--------|
 4 | Shift |   Z    |   X    |   C    |   D    |   V    |                         |   K    |   H    |  , <   |  . >   |  / ?   |  ' "   | 4
   |-------+--------+--------+--------+--------+--------,                         `--------+--------+--------+--------+--------+--------|
 5 |       |        |        |        |        | ,--------.                     ,--------. | Left   | Down   | Up     | Right  |        | 5
   `-------------------------------------------' |        |                     | Alt    | `--------------------------+--------+--------'
                                        ,--------+--------+--------.   ,--------+--------+--------.
                                        | GUI    | SpcTab | Lower  |   | Raise  | Enter  | Ctrl   |
                                        `--------------------------'   `--------------------------'
*/
  [_COLEMAK] = LAYOUT_moonlander(
    _______, _______, _______, _______, _______, _______, _______,         _______, _______, _______, _______, _______, _______, _______,
    KC_ESC , KC_Q   , KC_W   , KC_F   , KC_P   , KC_B   , _______,         _______, KC_J   , KC_L   , KC_U   , KC_Y   , KC_SCLN, _______,
    KC_BSPC, KC_A   , KC_R   , KC_S   , KC_T   , KC_G   , _______,         _______, KC_M   , KC_N   , KC_E   , KC_I   , KC_O   , KC_ENT ,
    TD_SHFT, KC_Z   , KC_X   , KC_C   , KC_D   , KC_V   ,                           KC_K   , KC_H   , KC_COMM, KC_DOT , KC_SLSH, KC_QUOT,
    _______, _______, _______, _______, _______,   _______,                       TD_ALT ,   KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, _______,
                                          TD_GUI , TD_SPC , LOWER  ,     RAISE  , KC_ENT , TD_CTL
  ),
};

/********** Functions **********/

/* Key Presses */

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
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
