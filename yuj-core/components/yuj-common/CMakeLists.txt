# yuj-common 接入 shim：源码与头文件直接指向仓库根的 yuj-common，不复制（铁律⑤）
set(YUJ_COMMON_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../yuj-common")
file(GLOB YUJ_COMMON_SRCS "${YUJ_COMMON_DIR}/src/*.c")

idf_component_register(
    SRCS ${YUJ_COMMON_SRCS}
    INCLUDE_DIRS "${YUJ_COMMON_DIR}/include")
