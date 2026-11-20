# yuj-common：双端共用协议源码库，零平台依赖（铁律①）。
# - yuj-core：经 components/yuj-common shim 走 idf_component_register()
# - yuj-rx / Host 单测：作为普通 CMake 库 add_subdirectory() 引入
cmake_minimum_required(VERSION 3.16)

set(YUJ_COMMON_SRCS
    # 协议实现随第 4 步逐步加入；首版仅有版本头（header-only）
)

if(ESP_PLATFORM)
    idf_component_register(
        SRCS ${YUJ_COMMON_SRCS}
        INCLUDE_DIRS include)
elseif(YUJ_COMMON_SRCS)
    add_library(yuj-common STATIC ${YUJ_COMMON_SRCS})
    target_include_directories(yuj-common PUBLIC include)
    target_compile_features(yuj-common PUBLIC c_std_11)
else()
    add_library(yuj-common INTERFACE)
    target_include_directories(yuj-common INTERFACE include)
    target_compile_features(yuj-common INTERFACE c_std_11)
endif()
