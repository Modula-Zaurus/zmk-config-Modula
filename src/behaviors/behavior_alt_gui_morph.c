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
    struct zmk_behavior_binding normal_binding;
    struct zmk_behavior_binding alt_binding;
    struct zmk_behavior_binding gui_binding;
};

struct behavior_alt_gui_morph_data {
    struct zmk_behavior_binding *pressed_binding;
};

static int on_alt_gui_morph_pressed(struct zmk_behavior_binding *binding,
                                    struct zmk_behavior_binding_event event) {
    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    const struct behavior_alt_gui_morph_config *cfg = dev->config;
    struct behavior_alt_gui_morph_data *data = dev->data;

    if (data->pressed_binding != NULL) {
        LOG_ERR("Can't press the same Alt/GUI morph twice");
        return -ENOTSUP;
    }

    zmk_mod_flags_t explicit_mods = zmk_hid_get_explicit_mods();
    if (explicit_mods & (MOD_LGUI | MOD_RGUI)) {
        /* GUI wins when Alt and GUI are both held. */
        data->pressed_binding = (struct zmk_behavior_binding *)&cfg->gui_binding;
        zmk_hid_masked_modifiers_set(MOD_LALT | MOD_RALT | MOD_LGUI | MOD_RGUI);
    } else if (explicit_mods & (MOD_LALT | MOD_RALT)) {
        data->pressed_binding = (struct zmk_behavior_binding *)&cfg->alt_binding;
        zmk_hid_masked_modifiers_set(MOD_LALT | MOD_RALT | MOD_LGUI | MOD_RGUI);
    } else {
        data->pressed_binding = (struct zmk_behavior_binding *)&cfg->normal_binding;
    }

    return zmk_behavior_invoke_binding(data->pressed_binding, event, true);
}

static int on_alt_gui_morph_released(struct zmk_behavior_binding *binding,
                                     struct zmk_behavior_binding_event event) {
    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    struct behavior_alt_gui_morph_data *data = dev->data;

    if (data->pressed_binding == NULL) {
        LOG_ERR("Alt/GUI morph already released");
        return -ENOTSUP;
    }

    struct zmk_behavior_binding *pressed_binding = data->pressed_binding;
    data->pressed_binding = NULL;
    zmk_hid_masked_modifiers_clear();
    return zmk_behavior_invoke_binding(pressed_binding, event, false);
}

static const struct behavior_driver_api behavior_alt_gui_morph_driver_api = {
    .binding_pressed = on_alt_gui_morph_pressed,
    .binding_released = on_alt_gui_morph_released,
#if IS_ENABLED(CONFIG_ZMK_BEHAVIOR_METADATA)
    .get_parameter_metadata = zmk_behavior_get_empty_param_metadata,
#endif
};

#define TRANSFORM_ENTRY(idx, node)                                                               \
    {                                                                                            \
        .behavior_dev = DEVICE_DT_NAME(DT_INST_PHANDLE_BY_IDX(node, bindings, idx)),           \
        .param1 = COND_CODE_0(DT_INST_PHA_HAS_CELL_AT_IDX(node, bindings, idx, param1), (0),   \
                              (DT_INST_PHA_BY_IDX(node, bindings, idx, param1))),               \
        .param2 = COND_CODE_0(DT_INST_PHA_HAS_CELL_AT_IDX(node, bindings, idx, param2), (0),   \
                              (DT_INST_PHA_BY_IDX(node, bindings, idx, param2))),               \
    }

#define ALT_GUI_MORPH_INST(n)                                                                    \
    static struct behavior_alt_gui_morph_config behavior_alt_gui_morph_config_##n = {          \
        .normal_binding = TRANSFORM_ENTRY(0, n),                                                \
        .alt_binding = TRANSFORM_ENTRY(1, n),                                                   \
        .gui_binding = TRANSFORM_ENTRY(2, n),                                                   \
    };                                                                                           \
    static struct behavior_alt_gui_morph_data behavior_alt_gui_morph_data_##n = {};            \
    BEHAVIOR_DT_INST_DEFINE(n, NULL, NULL, &behavior_alt_gui_morph_data_##n,                    \
                            &behavior_alt_gui_morph_config_##n, POST_KERNEL,                   \
                            CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                               \
                            &behavior_alt_gui_morph_driver_api);

DT_INST_FOREACH_STATUS_OKAY(ALT_GUI_MORPH_INST)

#endif
