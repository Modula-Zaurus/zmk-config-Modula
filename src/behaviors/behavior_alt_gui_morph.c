/*
 * Copyright (c) 2026
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT modula_behavior_alt_gui_morph

#include <zephyr/device.h>
#include <zephyr/logging/log.h>

#include <drivers/behavior.h>
#include <zmk/behavior.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/hid.h>
#include <zmk/matrix.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

struct behavior_alt_gui_morph_config {
    zmk_mod_flags_t mods;
    zmk_mod_flags_t masked_mods;
};

struct behavior_alt_gui_morph_data {
    uint32_t pressed_keycodes[ZMK_KEYMAP_LEN];
    bool active[ZMK_KEYMAP_LEN];
};

static int on_alt_gui_morph_pressed(struct zmk_behavior_binding *binding,
                                    struct zmk_behavior_binding_event event) {
    if (event.position >= ZMK_KEYMAP_LEN) {
        return -EINVAL;
    }

    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    const struct behavior_alt_gui_morph_config *cfg = dev->config;
    struct behavior_alt_gui_morph_data *data = dev->data;

    if (data->active[event.position]) {
        LOG_ERR("Key position %d is already active", event.position);
        return -EALREADY;
    }

    uint32_t keycode = binding->param1;
    if (zmk_hid_get_explicit_mods() & cfg->mods) {
        keycode = binding->param2;
        zmk_hid_masked_modifiers_set(cfg->masked_mods);
    }

    data->pressed_keycodes[event.position] = keycode;
    data->active[event.position] = true;
    return raise_zmk_keycode_state_changed_from_encoded(keycode, true, event.timestamp);
}

static int on_alt_gui_morph_released(struct zmk_behavior_binding *binding,
                                     struct zmk_behavior_binding_event event) {
    if (event.position >= ZMK_KEYMAP_LEN) {
        return -EINVAL;
    }

    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    struct behavior_alt_gui_morph_data *data = dev->data;

    if (!data->active[event.position]) {
        LOG_ERR("Key position %d is not active", event.position);
        return -EINVAL;
    }

    uint32_t keycode = data->pressed_keycodes[event.position];
    data->active[event.position] = false;
    zmk_hid_masked_modifiers_clear();
    return raise_zmk_keycode_state_changed_from_encoded(keycode, false, event.timestamp);
}

static const struct behavior_driver_api behavior_alt_gui_morph_driver_api = {
    .binding_pressed = on_alt_gui_morph_pressed,
    .binding_released = on_alt_gui_morph_released,
#if IS_ENABLED(CONFIG_ZMK_BEHAVIOR_METADATA)
    .get_parameter_metadata = zmk_behavior_get_empty_param_metadata,
#endif
};

#define ALT_GUI_MORPH_INST(n)                                                                    \
    static struct behavior_alt_gui_morph_config behavior_alt_gui_morph_config_##n = {          \
        .mods = DT_INST_PROP(n, mods),                                                          \
        .masked_mods =                                                                           \
            COND_CODE_0(DT_INST_NODE_HAS_PROP(n, keep_mods), (DT_INST_PROP(n, mods)),          \
                        (DT_INST_PROP(n, mods) & ~DT_INST_PROP(n, keep_mods))),                 \
    };                                                                                           \
    static struct behavior_alt_gui_morph_data behavior_alt_gui_morph_data_##n = {};            \
    BEHAVIOR_DT_INST_DEFINE(n, NULL, NULL, &behavior_alt_gui_morph_data_##n,                    \
                            &behavior_alt_gui_morph_config_##n, POST_KERNEL,                   \
                            CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                               \
                            &behavior_alt_gui_morph_driver_api);

DT_INST_FOREACH_STATUS_OKAY(ALT_GUI_MORPH_INST)

#endif
