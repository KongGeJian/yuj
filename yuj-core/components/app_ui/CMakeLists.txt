idf_component_register(
    SRCS "src/app_ui.c"
    INCLUDE_DIRS "include"
    REQUIRES svc_link svc_storage)

# LVGL / esp_lvgl_port 到 F 段接入时再加 PRIV_REQUIRES
target_compile_options(${COMPONENT_LIB} PRIVATE -Wall -Wextra -Werror)
