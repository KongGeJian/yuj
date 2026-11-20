idf_component_register(
    SRCS "src/board.c"
    INCLUDE_DIRS "include"
    PRIV_REQUIRES esp_driver_gpio)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wall -Wextra -Werror)
