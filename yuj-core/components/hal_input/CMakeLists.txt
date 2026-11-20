idf_component_register(
    SRCS "src/input_hal.c"
    INCLUDE_DIRS "include"
    REQUIRES board
    PRIV_REQUIRES esp_driver_i2c)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wall -Wextra -Werror)
