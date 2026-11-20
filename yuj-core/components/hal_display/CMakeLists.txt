idf_component_register(
    SRCS "src/display_hal.c"
    INCLUDE_DIRS "include"
    REQUIRES board
    PRIV_REQUIRES esp_lcd esp_driver_spi)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wall -Wextra -Werror)
