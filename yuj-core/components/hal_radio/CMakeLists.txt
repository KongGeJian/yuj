idf_component_register(
    SRCS "src/radio_hal.c"
    INCLUDE_DIRS "include"
    REQUIRES board
    PRIV_REQUIRES esp_driver_spi esp_driver_gpio)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wall -Wextra -Werror)
