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

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;

void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (state->pressed) {
        if (state->count == 1
#ifndef PERMISSIVE_HOLD
            && !state->interrupted
#endif
        ) {
            register_code16(tap_hold->hold);
            tap_hold->held = tap_hold->hold;
        } else {
            register_code16(tap_hold->tap);
            tap_hold->held = tap_hold->tap;
        }
    }
}

void tap_dance_tap_hold_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (tap_hold->held) {
        unregister_code16(tap_hold->held);
        tap_hold->held = 0;
    }
}


#define ACTION_TAP_DANCE_TAP_HOLD(tap, hold) \
    { .fn = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset}, .user_data = (void *)&((tap_dance_tap_hold_t){tap, hold, 0}), }

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
  TD_LR_BRCS,
  TD_GUI_ALT,
  TD_UP_DOWN,
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
  [TD_GUI_ALT]   = ACTION_TAP_DANCE_DOUBLE(KC_LGUI, KC_LALT),
  [TD_UP_DOWN]   = ACTION_TAP_DANCE_DOUBLE(KC_UP  , KC_DOWN),
  [TD_LR_PRNS]   = ACTION_TAP_DANCE_TAP_HOLD(KC_LPRN, KC_RPRN),
  [TD_LR_CBRS]   = ACTION_TAP_DANCE_TAP_HOLD(KC_LCBR, KC_RCBR),
  [TD_LR_BRCS]   = ACTION_TAP_DANCE_TAP_HOLD(KC_LBRC, KC_RBRC),
};

#define TD_SPC  TD(TD_SPC_TAB)
#define TD_SHFT TD(TD_LSFT_CAPS)
#define TD_CTL  TD(TD_LRCTL)
#define TD_ALT  TD(TD_LRALT)
#define TD_GUI  TD(TD_LRGUI)
#define TD_MSPC TD(TD_MS_B1_B2)
#define TD_MWDU TD(TD_WHD_WHU)
#define TD_MWRL TD(TD_WHR_WHL)
#define TD_GALT TD(TD_GUI_ALT)
#define TD_UPDN TD(TD_UP_DOWN)
#define TD_PRNS TD(TD_LR_PRNS)
#define TD_CBRS TD(TD_LR_CBRS)
#define TD_BRCS TD(TD_LR_BRCS)

/* Custom Keycodes */

enum my_keycodes {
    MK_RST,
};

/* Layers */

#define LOWER   MO(_LOWER)
#define RAISE   MO(_RAISE)
#define COLEMAK DF(_COLEMAK)
#define QWERTY  DF(_QWERTY)
#define CDL     MO(_CDL)

// Base layers MUST be at top
enum layers {
    _COLEMAK,
    _QWERTY,
    _LOWER,
    _RAISE,
    _ADJUST,
    _CDL
};

enum custom_keycodes {
    VRSN = SAFE_RANGE,
};


/********** Keymaps **********/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Template
       1       2        3        4        5        6        7                 7        6        5        4        3        2        1
   ,-------------------------------------------------------------.        ,--------------------------------------------------------------.
 1 |       |        |        |        |        |        |        |        |        |        |        |        |        |        |        | 1
   |-------+--------+--------+--------+--------+--------+--------|        |--------+--------+--------+--------+--------+--------+--------|
 2 |       |        |        |        |        |        |        |        |        |        |        |        |        |        |        | 2
   |-------+--------+--------+--------+========+--------+--------|        |--------+--------+========+--------+--------+--------+--------|
 3 |       |        |        |        |        |        |        |        |        |        |        |        |        |        |        | 3
   |-------+--------+--------+--------+========+--------+--------'        `--------+--------+========+--------+--------+--------+--------|
 4 |       |        |        |        |        |        |                          |        |        |        |        |        |        | 4
   |-------+--------+--------+--------+--------+--------'                          `--------+--------+--------+--------+--------+--------|
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
 1 | Esc   |   Q    |   W    |   F    |   P    |   B    |        |       |        |   J    |   L    |   U    |   Y    |  ; :   |        | 1
   |-------+--------+--------+--------+--------+--------+--------|       |--------+--------+--------+--------+--------+--------+--------|
 2 | Bksp  |   A    |   R    |   S    |   T    |   G    |        |       |        |   M    |   N    |   E    |   I    |   O    | Enter  | 2
   |-------+--------+--------+--------+========+--------+--------|       |--------+--------+========+--------+--------+--------+--------|
 3 | Shift |   Z    |   X    |   C    |   D    |   V    |        |       |        |   K    |   H    |  , <   |  . >   |  / ?   |  ' "   | 3
   |-------+--------+--------+--------+========+--------+--------'       `--------+--------+========+--------+--------+--------+--------|
 4 |       |        |        |        |        |        |                         |        |        |        |        |        |        | 4
   |-------+--------+--------+--------+--------+--------'                         `--------+--------+--------+--------+--------+--------|
 5 |       |        |        |        | GUI    | ,--------.                     ,--------. |        |        |        |        |        | 5
   `-------------------------------------------' | Alt    |                     |        | `--------------------------+--------+--------'
                                         ,-------+--------+--------.   ,--------+--------+--------.
                                         | SpcTab| Ctrl   | Lower  |   | Raise  |        |        |
                                         `-------------------------'   `--------------------------'
*/
  [_COLEMAK] = LAYOUT_moonlander(
    KC_ESC , KC_Q   , KC_W   , KC_F   , KC_P   , KC_B   , _______,         _______, KC_J   , KC_L   , KC_U   , KC_Y   , KC_SCLN, _______,
    KC_BSPC, KC_A   , KC_R   , KC_S   , KC_T   , KC_G   , _______,         _______, KC_M   , KC_N   , KC_E   , KC_I   , KC_O   , KC_ENT ,
    TD_SHFT, KC_Z   , KC_X   , KC_C   , KC_D   , KC_V   , _______,         _______, KC_K   , KC_H   , KC_COMM, KC_DOT , KC_SLSH, KC_QUOT,
    _______, _______, _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, KC_LGUI,   KC_LALT,                       _______,  _______, _______, _______, _______, _______,
                                          TD_SPC , KC_LCTL, LOWER  ,     RAISE  , _______, _______
  ),

/* QWERTY
       1       2        3        4        5        6        7                7        6        5        4        3        2        1
   ,-------------------------------------------------------------.       ,--------------------------------------------------------------.
 1 | Esc   |   Q    |   W    |   E    |   R    |   T    |        |       |        |   Y    |   U    |   I    |   O    |   P    |        | 1
   |-------+--------+--------+--------+--------+--------+--------|       |--------+--------+--------+--------+--------+--------+--------|
 2 | Bksp  |   A    |   S    |   D    |   F    |   G    |        |       |        |   H    |   J    |   K    |   L    |   ; :  | Enter  | 2
   |-------+--------+--------+--------+========+--------+--------|       |--------+--------+========+--------+--------+--------+--------|
 3 | Shift |   Z    |   X    |   C    |   V    |   B    |        |       |        |   N    |   M    |  , <   |  . >   |  / ?   |  ' "   | 3
   |-------+--------+--------+--------+========+--------+--------'       `--------+--------+========+--------+--------+--------+--------|
 4 |       |        |        |        |        |        |                         |        |        |        |        |        |        | 4
   |-------+--------+--------+--------+--------+--------'                         `--------+--------+--------+--------+--------+--------|
 5 |       |        |        |        | GUI    | ,--------.                     ,--------. |        |        |        |        |        | 5
   `-------------------------------------------' | Alt    |                     |        | `--------------------------+--------+--------'
                                        ,--------+--------+--------.   ,--------+--------+--------.
                                        | SpcTab | Ctrl   | Lower  |   | Raise  |        |        |
                                        `--------------------------'   `--------------------------'
*/
  [_QWERTY] = LAYOUT_moonlander(
    KC_ESC , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   , _______,         _______, KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , _______,
    KC_BSPC, KC_A   , KC_S   , KC_D   , KC_F   , KC_G   , _______,         _______, KC_H   , KC_J   , KC_K   , KC_L   , KC_SCLN, KC_ENT ,
    TD_SHFT, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , _______,         _______, KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, KC_QUOT,
    _______, _______, _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, KC_LGUI,   KC_LALT,                       _______,   _______, _______, _______, _______, _______,
                                          TD_SPC , KC_LCTL, LOWER  ,     RAISE  , _______, _______ 
  ),

/* Lower
       1       2        3        4        5        6        7                 7        6        5        4        3        2        1
   ,-------------------------------------------------------------.        ,--------------------------------------------------------------.
 1 |       |        | MW_LR  | MOUS_U | MW_UP  |        |        |        |        | PgUp   | Home   | End    |        |        |        | 1
   |-------+--------+--------+--------+--------+--------+--------|        |--------+--------+--------+--------+--------+--------+--------|
 2 | Del   | MS_LRC | MOUS_L | MOUS_D | MOUS_R |        |        |        |        | PgDwn  | Left   | Down   | Up     | Right  |        | 2
   |-------+--------+--------+--------+========+--------+--------|        |--------+--------+========+--------+--------+--------+--------|
 3 |       |        |        |        |        |        |        |        |        |        |        |        |        |        |        | 3
   |-------+--------+--------+--------+========+--------+--------'        `--------+--------+========+--------+--------+--------+--------|
 4 |       |        |        |        |        |        |                          |        |        |        |        |        |        | 4
   |-------+--------+--------+--------+--------+--------'                          `--------+--------+--------+--------+--------+--------|
 5 |       |        |        |        |        | ,--------.                     ,--------.  |        |        |        |        |        | 5
   `-------------------------------------------' |        |                     |        |  `--------------------------+--------+--------'
                                        ,--------+--------+--------.   ,--------+--------+--------.
                                        |        |        |        |   |        |        |        |
                                        `--------------------------'   `--------------------------'
*/
  [_LOWER] = LAYOUT_moonlander(
    _______, _______, TD_MWRL, KC_MS_U, TD_MWDU, _______, _______,          _______, KC_PGUP, KC_HOME, KC_END , _______, _______, _______,
    KC_DEL , TD_MSPC, KC_MS_L, KC_MS_D, KC_MS_R, _______, _______,          _______, KC_PGDN, KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, _______,
    _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______,   _______,                       _______,    _______, _______, _______, _______, _______,
                                          _______, _______, _______,     _______, _______, _______
  ),

/* Raise
       1       2        3        4        5        6        7                 7        6        5        4        3        2        1
   ,-------------------------------------------------------------.        ,--------------------------------------------------------------.
 2 |       |  ` ~   |  - _   |  = +   |  \ |   |        |        |        |        |        |  7 &   |  8 *   |  9 (   |  , <   |        | 2
   |-------+--------+--------+--------+========+--------+--------|        |--------+--------+========+--------+--------+--------+--------|
 3 |       |        |  [ ]   |  { }   |  ( )   |        |        |        |        |        |  4 $   |  5 %   |  6 ^   |  0 )   |        | 3
   |-------+--------+--------+--------+========+--------+--------|        |--------+--------+========+--------+--------+--------+--------|
 4 |       |        |        |        |        |        |        |        |        |        |  1 !   |  2 @   |  3 #   |  . >   |        | 4
   |-------+--------+--------+--------+--------+--------+--------'        `--------+--------+--------+--------+--------+--------+--------|
 1 |       |        |        |        |        |        |                          |        |        |        |        |        |        | 1
   |-------+--------+--------+--------+--------+--------'                          `--------+--------+--------+--------+--------+--------|
 5 |       |        |        |        |        | ,--------.                     ,--------.  |        |        |        |        |        | 5
   `-------------------------------------------' |        |                     |        |  `--------------------------+--------+--------'
                                        ,--------+--------+--------.   ,--------+--------+--------.
                                        |        |        |        |   |        |        |        |
                                        `--------------------------'   `--------------------------'
*/
  [_RAISE] = LAYOUT_moonlander(
    _______, KC_GRV , KC_MINS, KC_EQL , KC_BSLS, _______, _______,          _______, _______, KC_7   , KC_8   , KC_9   , KC_COMM, _______,
    _______, _______, TD_BRCS, TD_CBRS, TD_PRNS, _______, _______,          _______, _______, KC_4   , KC_5   , KC_6   , KC_0   , _______,
    _______, _______, _______, _______, _______, _______, _______,          _______, _______, KC_1   , KC_2   , KC_3   , KC_DOT , _______,
    _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______,   _______,                       _______,    _______, _______, _______, _______, _______,
                                          _______, _______, _______,     _______, _______, _______
  ),

/* Adjust (Lower + Raise)
       1       2        3        4        5        6        7                 7        6        5        4        3        2        1
   ,-------------------------------------------------------------.        ,--------------------------------------------------------------.
 1 |  F1   |  F2    |  F3    |  F4    |  F5    |  F6    |  F7    |        |  F8    |  F9    |  F10   |  F11   |  F12   |  F13   |  F14   | 1
   |-------+--------+--------+--------+--------+--------+--------|        |--------+--------+--------+--------+--------+--------+--------|
 2 |       |        | ASTog  | MuTogg | RGBTog |        |        |        |        |        |        |        |        |        | Reset  | 2
   |-------+--------+--------+--------+========+--------+--------|        |--------+--------+========+--------+--------+--------+--------|
 3 |       |        |        | MuNext | RGBMod |        |        |        |        |        |        |        |        |        |        | 3
   |-------+--------+--------+--------+========+--------+--------'        `--------+--------+========+--------+--------+--------+--------|
 4 |       |        |        |        |        |        |                          |        |        |        |        |        |        | 4
   |-------+--------+--------+--------+--------+--------'                          `--------+--------+--------+--------+--------+--------|
 5 | CDL   |        |        |        |        | ,--------.                     ,--------.  |        |        |        |        |        | 5
   `-------------------------------------------' |        |                     |        |  `--------------------------+--------+--------'
                                        ,--------+--------+--------.   ,--------+--------+--------.
                                        |        |        |        |   |        |        |        |
                                        `--------------------------'   `--------------------------'
*/
  [_ADJUST] = LAYOUT_moonlander(
    KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  , KC_F6  , KC_F7  ,          KC_F8  , KC_F9  , KC_F10 , KC_F11 , KC_F12 , KC_F13 , KC_F14 ,
    _______, _______, AS_TOGG, MU_TOGG, RGB_TOG, _______, _______,          _______, _______, _______, _______, _______, _______, MK_RST ,
    _______, _______, _______, MU_NEXT, RGB_MOD, _______, _______,          _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
    CDL    , _______, _______, _______, _______,   _______,                       _______,    _______, _______, _______, _______, _______,
                                          _______, _______, _______,     _______, _______, _______
  ),

/* Change Default Layer (CDL)
       1       2        3        4        5        6        7                7         6        5        4        3        2        1
   ,-------------------------------------------------------------.        ,--------------------------------------------------------------.
 1 |       | QWERTY |        |        |        |        |        |        |        |        |        |        |        |        |        | 1
   |-------+--------+--------+--------+--------+--------+--------|        |--------+--------+--------+--------+--------+--------+--------|
 2 |       |        |        |        |        |        |        |        |        |        |        |        |        |        |        | 2
   |-------+--------+--------+--------+========+--------+--------|        |--------+--------+========+--------+--------+--------+--------|
 3 |       |        |        | COLEMAK|        |        |        |        |        |        |        |        |        |        |        | 3
   |-------+--------+--------+--------+========+--------+--------'        `--------+--------+========+--------+--------+--------+--------|
 4 |       |        |        |        |        |        |                          |        |        |        |        |        |        | 4
   |-------+--------+--------+--------+--------+--------'                          `--------+--------+--------+--------+--------+--------|
 5 |       |        |        |        |        | ,--------.                     ,--------.  |        |        |        |        |        | 5
   `-------------------------------------------' |        |                     |        |  `--------------------------+--------+--------'
                                        ,--------+--------+--------.   ,--------+--------+--------.
                                        |        |        |        |   |        |        |        |
                                        `--------------------------'   `--------------------------'
*/
  [_CDL] = LAYOUT_moonlander(
    _______, QWERTY , _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, COLEMAK, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______,   _______,                       _______,    _______, _______, _______, _______, _______,
                                          _______, _______, _______,     _______, _______, _______
  ),
};

/* LED Colormaps */

const uint8_t PROGMEM ledmap[][RGB_MATRIX_LED_COUNT][3] = {
  /*
  [_TEMPLATE] = {
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,          CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,          CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,          CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,                            CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,
    CLR____, CLR____, CLR____, CLR____, CLR____,   CLR____,                       CLR____,    CLR____, CLR____, CLR____, CLR____, CLR____,
                                          CLR____, CLR____, CLR____,     CLR____, CLR____, CLR____
  },
  */
  [_LOWER] = {
    CLR____, CLR____, CLR_ORG, CLR_BLU, CLR_ORG, CLR____, CLR____,          CLR____, CLR_YLW, CLR_YLW, CLR_YLW, CLR____, CLR____, CLR____,
    CLR_YLW, CLR_ORG, CLR_BLU, CLR_BLU, CLR_BLU, CLR____, CLR____,          CLR____, CLR_YLW, CLR_GRN, CLR_GRN, CLR_GRN, CLR_GRN, CLR____,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,          CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,                            CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,
    CLR____, CLR____, CLR____, CLR____, CLR____,   CLR____,                       CLR____,    CLR____, CLR____, CLR____, CLR____, CLR____,
                                          CLR____, CLR____, CLR____,     CLR____, CLR____, CLR____
  },
  [_RAISE] = {
    CLR____, CLR_PUR, CLR_PUR, CLR_PUR, CLR_PUR, CLR____, CLR____,          CLR____, CLR____, CLR_BLU, CLR_BLU, CLR_BLU, CLR_PUR, CLR____,
    CLR____, CLR____, CLR_PUR, CLR_PUR, CLR_PUR, CLR____, CLR____,          CLR____, CLR____, CLR_BLU, CLR_BLU, CLR_BLU, CLR_BLU, CLR____,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,          CLR____, CLR____, CLR_BLU, CLR_BLU, CLR_BLU, CLR_PUR, CLR____,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,                            CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,
    CLR____, CLR____, CLR____, CLR____, CLR____,   CLR____,                       CLR____,    CLR____, CLR____, CLR____, CLR____, CLR____,
                                          CLR____, CLR____, CLR____,     CLR____, CLR____, CLR____
  },
  [_ADJUST] = {
    CLR_ORG, CLR_ORG, CLR_ORG, CLR_ORG, CLR_ORG, CLR_ORG, CLR_ORG,          CLR_ORG, CLR_ORG, CLR_ORG, CLR_ORG, CLR_ORG, CLR_ORG, CLR_ORG,
    CLR____, CLR____, CLR_RED, CLR_GRN, CLR_BLU, CLR____, CLR____,          CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR_PNK,
    CLR____, CLR____, CLR____, CLR_GRN, CLR_BLU, CLR____, CLR____,          CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,                            CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,
    CLR_PNK, CLR____, CLR____, CLR____, CLR____,   CLR____,                       CLR____,    CLR____, CLR____, CLR____, CLR____, CLR____,
                                          CLR____, CLR____, CLR____,     CLR____, CLR____, CLR____
  },
  [_CDL] = {
    CLR____, CLR_PUR, CLR____, CLR____, CLR____, CLR____, CLR____,          CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,          CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,
    CLR____, CLR____, CLR____, CLR_PUR, CLR____, CLR____, CLR____,          CLR____, CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,
    CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,                            CLR____, CLR____, CLR____, CLR____, CLR____, CLR____,
    CLR____, CLR____, CLR____, CLR____, CLR____,   CLR____,                       CLR____,    CLR____, CLR____, CLR____, CLR____, CLR____,
                                          CLR____, CLR____, CLR____,     CLR____, CLR____, CLR____
  },
};

/********** Functions **********/

/* Key Presses */

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    tap_dance_action_t *action;
    switch(keycode) {
        case MK_RST:
            if (record->event.pressed) {
                PLAY_SONG(upgrade_song);
                SEND_STRING(SS_DELAY(800));
                reset_keyboard();
            }
            break;
        case TD_PRNS:  // list all tap dance keycodes with tap-hold configurations
        case TD_BRCS:
        case TD_CBRS:
            action = &tap_dance_actions[TD_INDEX(keycode)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
                tap_code16(tap_hold->tap);
            }
   	        break;
    }
    return true;
};

/* Layers */

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

/* LEDs */
void keyboard_post_init_user(void) {
  rgb_matrix_enable();
}

int convert_led_number(int led) {
  static const int led_mapping[] = {
     0, 14, 28, 42, 54,
     1, 15, 29, 43, 55,
     2, 16, 30, 44, 56,
     3, 17, 31, 45, 57,
     4, 18, 32, 46, 58,
     5, 19, 33, 47,
     6, 20, 34,
    66, 67, 68, 59,

    13, 27, 41, 53, 65,
    12, 26, 40, 52, 64,
    11, 25, 39, 51, 63,
    10, 24, 38, 50, 62,
     9, 23, 37, 49, 61,
     8, 22, 36, 48,
     7, 21, 35,
    71, 70, 69, 60
  };
  return led_mapping[led];
}

void set_layer_color(int layer) {
  for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
    int actual_led = convert_led_number(i);
    HSV hsv = {
      .h = pgm_read_byte(&ledmap[layer][actual_led][0]),
      .s = pgm_read_byte(&ledmap[layer][actual_led][1]),
      .v = pgm_read_byte(&ledmap[layer][actual_led][2]),
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

bool rgb_matrix_indicators_user(void) {
  if (keyboard_config.disable_layer_led) { return false; }
  int layer = biton32(layer_state);
  if (layer == _COLEMAK || layer == _QWERTY) {
      if (rgb_matrix_get_flags() == LED_FLAG_NONE) {
        rgb_matrix_set_color_all(0, 0, 0);
      }
  } else {
      set_layer_color(layer);
  }
  return false;
}
