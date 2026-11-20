cmake_minimum_required(VERSION 3.16)

# 组件根：components/ 下每个子目录是一个 component（一模块一 component，见 docs/project-skeleton.md §2）
set(EXTRA_COMPONENT_DIRS "${CMAKE_CURRENT_LIST_DIR}/components")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(yuj-core VERSION 0.1.0)   # 模块版本独立演进（law §4）
