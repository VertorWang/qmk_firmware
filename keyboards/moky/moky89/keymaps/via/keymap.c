// Copyright 2023 JoyLee (@itarze)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum layers{
    WIN_BASE = 0,
    WIN_FN,
    MAC_BASE,
    MAC_FN,
    CUSTOM
};

enum user_keys {
    KC_TS = EX_USER,
    KC_DESK,
    KC_EE_CLR,
    LEO_MOD,
    LEO_HUI,
    LEO_BOOT
};

#define LEO_MOD_MAX 6
#define LEO_HUI_MAX 8

#define DISCOLORATION                     35    /* 七彩变色 */
#define STEADYRED                         1     /* 红色常亮 */
#define MONOCHROMATICRESPIRATION          5     /* 单色呼吸 */
#define NEONLAMP                          8     /* 霓虹灯效 */
#define WATERFALLLIGHT                    13    /* 流水灯 */
#define MARQUEECONTROL                    21    /* 跑马灯 */
#define MONOCHROMERUNNINGLANTERN          15    /* 单色跑马灯 */

static uint8_t leo_modes[] = {WATERFALLLIGHT, DISCOLORATION, STEADYRED, MONOCHROMATICRESPIRATION, NEONLAMP, 0};
static uint8_t leo_hsvs[][3] = {
    {HSV_RED},
    {HSV_ORANGE},
    {HSV_YELLOW},
    {HSV_GREEN},
    {HSV_CYAN},
    {HSV_BLUE},
    {HSV_PURPLE},
    {HSV_WHITE}
};

extern bool im_bat_req_flag;
uint8_t bat_state = 0;
bool bat_blink = false;

/* enum alias_key {
    AL_MSUP = KC_MS_UP,
    AL_MSDN = KC_MS_DOWN,
    AL_MSLF = KC_MS_LEFT,
    AL_MSRG = KC_MS_RIGHT,
}; */

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [WIN_BASE] = LAYOUT( /* Base */
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,  KC_F4,  KC_F5,  KC_F6,   KC_F7,  KC_F8,  KC_F9,    KC_F10,   KC_F11,     KC_F12,   KC_MUTE,  KC_PSCR,  KC_SCRL,  KC_PAUSE,
        KC_GRV,   KC_1,     KC_2,     KC_3,   KC_4,   KC_5,   KC_6,    KC_7,   KC_8,   KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,  KC_INS,   KC_HOME,  KC_PGUP,
        KC_TAB,   KC_Q,     KC_W,     KC_E,   KC_R,   KC_T,   KC_Y,    KC_U,   KC_I,   KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,  KC_DEL,   KC_END,   KC_PGDN,
        KC_CAPS,  KC_A,     KC_S,     KC_D,   KC_F,   KC_G,   KC_H,    KC_J,   KC_K,   KC_L,     KC_SCLN,  KC_QUOT,    KC_NUHS,  KC_ENT,
        KC_LSFT,  KC_NUBS,  KC_Z,     KC_X,   KC_C,   KC_V,   KC_B,    KC_N,   KC_M,   KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,            KC_UP,
        KC_LCTL,  KC_LGUI,  KC_LALT,                          KC_SPC,                            KC_RALT,  MO(WIN_FN), KC_APP,   KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [WIN_FN] = LAYOUT( /* Base */
        _______,  KC_MSEL,       KC_VOLD,       KC_VOLU,  KC_MUTE,  KC_MSTP,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MAIL,  KC_WHOM,  KC_CALC,  KC_WSCH,  _______,  _______, _______,  _______,
        _______,  _______,       _______,       _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  RGB_SPD,  RGB_SPI,  RGB_TOG,   _______,  _______,  LEO_MOD,
        _______,  IM_BT1,        IM_BT2,        IM_BT3,   IM_2G4,   _______,  _______,  _______,  _______,  _______,  _______,  RGB_SAD,  RGB_SAI,  RGB_MOD,   _______,  _______,  LEO_HUI,
        _______,  TO(WIN_BASE),  TO(MAC_BASE),  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,       _______,       _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            MO(CUSTOM),          RGB_VAI,
        _______,  GU_TOGG,       _______,                                     KC_EE_CLR,                              _______,  _______,  _______,  _______,   RGB_HUD,  RGB_VAD,  RGB_HUI),

    [MAC_BASE] = LAYOUT(  /* Base */
        KC_ESC,   KC_BRMD,  KC_BRMU,  KC_TS,    KC_DESK,  _______,  _______,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,     KC_VOLU,  KC_MUTE,  KC_PSCR,  KC_SCRL,  KC_PAUSE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,     KC_EQL,   KC_BSPC,  KC_INS,   KC_HOME,  KC_PGUP,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,     KC_RBRC,  KC_BSLS,  KC_DEL,   KC_END,   KC_PGDN,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,     KC_NUHS,  KC_ENT,
        KC_LSFT,  KC_NUBS,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,               KC_RSFT,            KC_UP,
        KC_LCTL,  KC_LALT,  KC_LGUI,                                KC_SPC,                                 KC_RGUI,  MO(MAC_FN),  KC_APP,   KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [MAC_FN] = LAYOUT(  /* FN */
        _______,  KC_F1,         KC_F2,         KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_MUTE,   KC_PSCR,  KC_SCRL,  KC_PAUSE,
        _______,  _______,       _______,       _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  RGB_SPD,  RGB_SPI,  RGB_TOG,   _______,  _______,  LEO_MOD,
        _______,  IM_BT1,        IM_BT2,        IM_BT3,   IM_2G4,   _______,  _______,  _______,  _______,  _______,  _______,  RGB_SAD,  RGB_SAI,  RGB_MOD,   _______,  _______,  LEO_HUI,
        _______,  TO(WIN_BASE),  TO(MAC_BASE),  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,       _______,       _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            MO(CUSTOM),          RGB_VAI,
        _______,  _______,       _______,                                     KC_EE_CLR,                              _______,  _______,  _______,  _______,   RGB_HUD,  RGB_VAD,  RGB_HUI),
    [CUSTOM] = LAYOUT(  /* FN */
        LEO_BOOT, _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,            _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______,  _______),
};

// clang-format on

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {

    [0] = { ENCODER_CCW_CW(KC_VOLU, KC_VOLD)},
    [1] = { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD)},
    [2] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
    [3] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
    [4] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS)}
};
#endif

#ifdef RGB_MATRIX_BLINK_ENABLE
void bat_indicators_hook(uint8_t index);

blink_rgb_t blink_rgbs[RGB_MATRIX_BLINK_COUNT] = {
    {.index = IM_MM_RGB_BLINK_INDEX_BT1, .interval = 250, .times = 1, .color = {.r = 0x00, .g = 0xFF, .b = 0x00}, .blink_cb = im_mm_rgb_blink_cb},
    {.index = IM_MM_RGB_BLINK_INDEX_BT2, .interval = 250, .times = 1, .color = {.r = 0x00, .g = 0xFF, .b = 0x00}, .blink_cb = im_mm_rgb_blink_cb},
    {.index = IM_MM_RGB_BLINK_INDEX_BT3, .interval = 250, .times = 1, .color = {.r = 0x00, .g = 0xFF, .b = 0x00}, .blink_cb = im_mm_rgb_blink_cb},
    {.index = IM_MM_RGB_BLINK_INDEX_2G4, .interval = 250, .times = 1, .color = {.r = 0x00, .g = 0xFF, .b = 0x00}, .blink_cb = im_mm_rgb_blink_cb},
    {.index = IM_MM_RGB_BLINK_INDEX_USB, .interval = 250, .times = 1, .color = {.r = 0x00, .g = 0xFF, .b = 0x00}, .blink_cb = im_mm_rgb_blink_cb},
    {.index = IM_MM_RGB_BLINK_INDEX_SPI, .interval = 250, .times = 1, .color = {.r = 0xFF, .g = 0xFF, .b = 0xFF}, .blink_cb = NULL},
    {.index = IM_MM_RGB_BLINK_INDEX_SPD, .interval = 250, .times = 1, .color = {.r = 0xFF, .g = 0xFF, .b = 0xFF}, .blink_cb = NULL},
    {.index = IM_MM_RGB_BLINK_INDEX_VAI, .interval = 250, .times = 1, .color = {.r = 0xFF, .g = 0xFF, .b = 0xFF}, .blink_cb = NULL},
    {.index = IM_MM_RGB_BLINK_INDEX_VAD, .interval = 250, .times = 1, .color = {.r = 0xFF, .g = 0xFF, .b = 0xFF}, .blink_cb = NULL},
    {.index = IM_MM_RGB_BLINK_INDEX_WIN, .interval = 250, .times = 1, .color = {.r = 0xFF, .g = 0xFF, .b = 0xFF}, .blink_cb = NULL},
    {.index = IM_MM_RGB_BLINK_INDEX_MAC, .interval = 250, .times = 1, .color = {.r = 0xFF, .g = 0xFF, .b = 0xFF}, .blink_cb = NULL},
    {.index = RGB_MATRIX_BLINK_INDEX_BAT, .interval = 250, .times = 3, .color = {.r = 0x00, .g = 0x00, .b = 0x00}, .blink_cb = bat_indicators_hook},
    {.index = RGB_MATRIX_BLINK_INDEX_ALL, .interval = 250, .times = 3, .color = {.r = 0xFF, .g = 0x00, .b = 0x00}, .blink_cb = NULL},
};
#endif

typedef union {
    uint32_t raw;
    struct {
        uint8_t leo_mod_index: 4;
        uint8_t leo_hui_index: 4;
        uint8_t flag: 1;
        uint8_t rgb_enable: 1;
    };
} confinfo_t;
confinfo_t confinfo;

// 此函数不需要改动
bool mm_get_rgb_enable(void) {
    // return rgb_matrix_config.enable;
#ifdef RGBLIGHT_ENABLE
    return confinfo.rgb_enable;
#else
    return rgb_matrix_config.enable;
#endif
}

// 此函数不需要改动
void mm_set_rgb_enable(bool state) {
    // rgb_matrix_config.enable = state;
#ifdef RGBLIGHT_ENABLE
    confinfo.rgb_enable = state;
    if (confinfo.rgb_enable)
        rgblight_enable();
    else
        rgblight_disable();

    eeconfig_update_user(confinfo.raw);
#else
    rgb_matrix_config.enable = state;
#endif
}

void eeconfig_confinfo_update(void)
{
    eeconfig_update_user(confinfo.raw);
}

void eeconfig_confinfo_default(void) {

    confinfo.leo_mod_index = 0;
    confinfo.leo_hui_index = 0;
    confinfo.flag = true;
    confinfo.rgb_enable = true;

    rgblight_mode(leo_modes[confinfo.leo_mod_index]);

    eeconfig_update_user(confinfo.raw);
}

#ifdef RGB_MATRIX_ENABLE

#ifdef RGB_MATRIX_BLINK_INDEX_BAT

static bool battery_process_flag = false;
static uint32_t battery_process_time = 0;

void bat_indicators_hook(uint8_t index) {
    /* 充电状态 */
    if ((mm_eeconfig.charging) && (bat_state != BAT_FULL_STATE)) {
        rgb_matrix_blink_set_color(index, 0xFF, 0x00, 0x00);
        rgb_matrix_blink_set_interval_times(index, 200, 0xFF);

        bat_blink = true;
    } else if ((bts_info.bt_info.pvol <= BATTERY_CAPACITY_LOW) && (bts_info.bt_info.pvol > BATTERY_CAPACITY_STOP)) {
        /* 低电压状态 */

        rgb_matrix_blink_set_color(index, 0xFF, 0x00, 0x00);
        rgb_matrix_blink_set_interval_times(index, 3000, 0x1);

        bat_blink = true;
    } else if (bts_info.bt_info.pvol <= BATTERY_CAPACITY_STOP) {
        /* 亏电压状态 */

        if (!battery_process_flag) {
            battery_process_time = timer_read32();
            battery_process_flag = true;
        }
        bat_blink = false;
    } else if (bat_state == BAT_FULL_STATE) {
        bat_blink = false;
    } else {
        bat_blink = true;
    }

    if (battery_process_flag) {
        if (timer_elapsed32(battery_process_time) > 10000) {
            battery_process_flag = false;

            if (bts_info.bt_info.pvol <= BATTERY_CAPACITY_STOP) {
                im_set_power_off();
            }
        }
    }

    rgb_matrix_blink_set(index);
}

bool rgb_matrix_blink_user(blink_rgb_t *blink_rgb) {
    if (blink_rgb->index == RGB_MATRIX_BLINK_INDEX_BAT) {
        if (bat_blink != true) {
            return false;
        }
    }

    return true;
}
#endif

#endif

void suspend_power_down_user(void) {

}

bool im_led_deinit_user(void) {

    if (mm_eeconfig.charging) {
        setPinOutput(POWER_DCDC_EN_PIN);
        writePin(POWER_DCDC_EN_PIN, 0);
    }

    return true;
}

bool im_led_init_user() {
    if (mm_eeconfig.charging) {
        setPinOutput(POWER_DCDC_EN_PIN);
        writePin(POWER_DCDC_EN_PIN, 1);
    }

    return true;
}

bool im_init_user(void) {

    confinfo.raw = eeconfig_read_user();
    if (!confinfo.raw) {
        eeconfig_confinfo_default();
    }

#ifdef RGB_MATRIX_BLINK_INDEX_BAT
    rgb_matrix_blink_set_interval_times(RGB_MATRIX_BLINK_INDEX_BAT, 200, 0xFF);
    rgb_matrix_blink_set(RGB_MATRIX_BLINK_INDEX_BAT);
#endif

    return true;
}

bool im_pre_init_user(void) {

    bool usb_cable = false;

#ifdef MM_BAT_CABLE_PIN
    setPinInput(MM_BAT_CABLE_PIN);
    usb_cable = readPin(MM_BAT_CABLE_PIN);
#endif

#ifdef POWER_DCDC_EN_PIN
    if (!usb_cable) {
        setPinOutput(POWER_DCDC_EN_PIN);
        writePin(POWER_DCDC_EN_PIN, 1);
    }
#endif

#ifdef BAT_FULL_PIN
    setPinInputHigh(BAT_FULL_PIN);
    bat_state = readPin(BAT_FULL_PIN);
#endif

    return true;
}

bool im_loop_user(void) {

    bat_state = readPin(BAT_FULL_PIN);

    return true;
}

bool im_reset_settings_user(void) {

#ifdef MULTIMODE_ENABLE
    eeconfig_update_multimode_default();
#endif
// #ifdef RGB_MATRIX_BLINK_ENABLE
//     rgb_matrix_blink_set(RGB_MATRIX_BLINK_INDEX_ALL);
// #endif

    return true;
}

im_lkey_t lkey_define_user[IM_LKEY_COUNT] = {
    {.keycode = KC_EE_CLR, .hole_time = 3000},
    {.keycode = LEO_BOOT, .hole_time = 3000},
};

bool im_lkey_process_user(uint16_t keycode, bool pressed) {
    switch (keycode) {
    case KC_EE_CLR:
        if (pressed) {
            im_reset_settings();
        }
        return false;
        break;
    case LEO_BOOT:
        if (pressed) {
            reset_keyboard();
        }
        return false;
        break;

    default:
        break;
    }

    return true;
}

bool im_process_record_user(uint16_t keycode, keyrecord_t *record) {

    if ((keycode != MO(WIN_FN)) && (keycode != MO(MAC_FN))) {
        im_bat_req_flag = false;
    }

    switch (keycode) {
    case KC_TS:
        if (record->event.pressed) {
            register_code(KC_LGUI);
            register_code(KC_TAB);
        }
        else {
            unregister_code(KC_LGUI);
            unregister_code(KC_TAB);
        }
        return false;
        break;
    case KC_DESK:
        if (record->event.pressed) {
            register_code(KC_LGUI);
            register_code(KC_LSFT);
            register_code(KC_D);
        }
        else {
            unregister_code(KC_LGUI);
            unregister_code(KC_LSFT);
            unregister_code(KC_D);
        }
        return false;
        break;
    case RGB_SPI:
        if (record->event.pressed) {
            if (rgb_matrix_get_speed() >= 0xFF) {
                rgb_matrix_blink_set_interval_times(IM_MM_RGB_BLINK_INDEX_SPI, 250, 0x3);
                rgb_matrix_blink_set(IM_MM_RGB_BLINK_INDEX_SPI);
            }
        }
        return true;
        break;
    case RGB_SPD:
        if (record->event.pressed) {
            if (rgb_matrix_get_speed() <= 0x0) {
                rgb_matrix_blink_set_interval_times(IM_MM_RGB_BLINK_INDEX_SPD, 250, 3);
                rgb_matrix_blink_set(IM_MM_RGB_BLINK_INDEX_SPD);
            }
        }
        return true;
        break;
    case RGB_VAI:
        if (record->event.pressed) {
            if (rgb_matrix_get_val() >= RGB_MATRIX_MAXIMUM_BRIGHTNESS) {
                rgb_matrix_blink_set_interval_times(IM_MM_RGB_BLINK_INDEX_VAI, 250, 0x3);
                rgb_matrix_blink_set(IM_MM_RGB_BLINK_INDEX_VAI);
            }
        }
        return true;
        break;
    case RGB_VAD:
        if (record->event.pressed) {
            if (rgb_matrix_get_val() <= 0x0) {
                rgb_matrix_blink_set_interval_times(IM_MM_RGB_BLINK_INDEX_VAD, 250, 3);
                rgb_matrix_blink_set(IM_MM_RGB_BLINK_INDEX_VAD);
            }
        }
        return true;
        break;
    case TO(WIN_BASE):
        if (record->event.pressed) {
            rgb_matrix_blink_set_interval_times(IM_MM_RGB_BLINK_INDEX_WIN, 250, 3);
            rgb_matrix_blink_set(IM_MM_RGB_BLINK_INDEX_WIN);
        }
        return true;
        break;
    case TO(MAC_BASE):
        if (record->event.pressed) {
            rgb_matrix_blink_set_interval_times(IM_MM_RGB_BLINK_INDEX_MAC, 250, 4);
            rgb_matrix_blink_set(IM_MM_RGB_BLINK_INDEX_MAC);
        }
        return true;
        break;
    case KC_EE_CLR:
        if (!record->event.pressed) {
            im_bat_req_flag = true;
        }
        return false;
    case LEO_BOOT:
        return false;
    case LEO_MOD:
        if (!record->event.pressed) {
            confinfo.leo_mod_index = (confinfo.leo_mod_index + 1) % LEO_MOD_MAX;
            uint8_t mode = leo_modes[confinfo.leo_mod_index];

            if (mode == 0) {
                rgblight_disable();
            }
            else {
                rgblight_enable();
            }

            rgblight_mode(mode);
            eeconfig_confinfo_update();
        }
        return false;
    case LEO_HUI:
        if (!record->event.pressed) {
            confinfo.leo_hui_index = (confinfo.leo_hui_index + 1) % LEO_HUI_MAX;
            rgblight_sethsv(leo_hsvs[confinfo.leo_hui_index][0],
                            leo_hsvs[confinfo.leo_hui_index][1],
                            leo_hsvs[confinfo.leo_hui_index][2]);
            eeconfig_confinfo_update();
        }
        return false;
    default:
        break;
    }
    return true;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {

    // FN 按下时显示当前设备状态
    if (get_highest_layer(default_layer_state | layer_state) == 1) {
        rgb_matrix_set_color(IM_MM_RGB_BLINK_INDEX_WIN, RGB_WHITE);
    } else if (get_highest_layer(default_layer_state | layer_state) == 3) {
        rgb_matrix_set_color(IM_MM_RGB_BLINK_INDEX_MAC, RGB_WHITE);
    }

    if (host_keyboard_led_state().caps_lock) {
        RGB_MATRIX_INDICATOR_SET_COLOR(51, RGB_MATRIX_MAXIMUM_BRIGHTNESS, RGB_MATRIX_MAXIMUM_BRIGHTNESS, RGB_MATRIX_MAXIMUM_BRIGHTNESS);
    }
    if (host_keyboard_led_state().scroll_lock) {
        RGB_MATRIX_INDICATOR_SET_COLOR(15, RGB_MATRIX_MAXIMUM_BRIGHTNESS, RGB_MATRIX_MAXIMUM_BRIGHTNESS, RGB_MATRIX_MAXIMUM_BRIGHTNESS);
    }
    if (keymap_config.no_gui) {
        if (get_highest_layer(default_layer_state) == 0) {
            RGB_MATRIX_INDICATOR_SET_COLOR(80, RGB_MATRIX_MAXIMUM_BRIGHTNESS, RGB_MATRIX_MAXIMUM_BRIGHTNESS, RGB_MATRIX_MAXIMUM_BRIGHTNESS);
        } else if (get_highest_layer(default_layer_state) == 2) {
            RGB_MATRIX_INDICATOR_SET_COLOR(81, RGB_MATRIX_MAXIMUM_BRIGHTNESS, RGB_MATRIX_MAXIMUM_BRIGHTNESS, RGB_MATRIX_MAXIMUM_BRIGHTNESS);
        }
    }

    return true;
}
