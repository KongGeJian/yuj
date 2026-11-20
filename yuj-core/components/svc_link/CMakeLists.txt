idf_component_register(
    SRCS "src/link_sched.c" "src/protocol_assert.c"
    INCLUDE_DIRS "include"
    REQUIRES board hal_radio yuj-common)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wall -Wextra -Werror)
