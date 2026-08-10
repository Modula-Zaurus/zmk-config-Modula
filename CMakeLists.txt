zephyr_library()

zephyr_library_sources_ifdef(CONFIG_MODULA_BEHAVIOR_ALT_GUI_MORPH
    src/behaviors/behavior_alt_gui_morph.c)

# The behavior invokes ZMK application symbols, which are provided by app/libapp.a.
zephyr_library_link_libraries(app)
