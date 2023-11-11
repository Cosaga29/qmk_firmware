#include QMK_KEYBOARD_H


/* THIS FILE WAS GENERATED!
 *
 * This file was generated by qmk json2c. You may or may not want to
 * edit it directly.
 */


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT_split_3x6_3(KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_MINS, KC_BSPC, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_LSFT, MT(MOD_LCTL, KC_Z), KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_ESC, MO(3), MO(1), KC_SPC, KC_ENT, MO(2), MO(4)),
	[1] = LAYOUT_split_3x6_3(KC_TILD, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_7, KC_8, KC_9, KC_DEL, KC_LBRC, KC_RBRC, KC_LPRN, KC_RPRN, KC_GRV, KC_MINS, KC_PIPE, KC_EQL, KC_4, KC_5, KC_6, C(KC_DEL), _______, _______, KC_LCBR, KC_RCBR, _______, KC_PLUS, KC_UNDS, KC_BSLS, KC_1, KC_2, KC_3, _______, _______, _______, _______, MO(5), KC_0),
	[2] = LAYOUT_split_3x6_3(KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, C(KC_BSPC), C(KC_BSPC), KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, _______, _______, QK_BOOT, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_HOME, KC_PGDN, KC_PGUP, KC_END, _______, _______, _______, MO(5), _______, _______, _______, _______),
	[3] = LAYOUT_split_3x6_3(_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, LM(0, MOD_LCTL), LM(0, MOD_LALT), LM(0, MOD_LCTL | MOD_LALT), _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),
	[4] = LAYOUT_split_3x6_3(_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, LM(0, MOD_LGUI), _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, LM(0, MOD_LSFT | MOD_LCTL), LM(0, MOD_LCTL), LM(0, MOD_LALT), LM(0, MOD_LCTL | MOD_LALT), _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),
	[5] = LAYOUT_split_3x6_3(_______, _______, _______, _______, _______, _______, RGB_VAI, RGB_HUI, RGB_SAI, RGB_MOD, RGB_TOG, _______, EE_CLR, _______, _______, _______, _______, _______, RGB_VAD, RGB_HUD, RGB_SAD, RGB_RMOD, CK_TOGG, _______, QK_BOOT, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______)
};

#if defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {

};
#endif // defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)




