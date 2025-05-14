cmake_minimum_required(VERSION 3.12)    # 设置最低版本
project(fxy_project)    # 设置工程名

# 设置编译器,并且清理缓存
set(CMAKE_C_COMPILER "/home/fxy/imx6ull/100ask_imx6ull-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/bin/arm-buildroot-linux-gnueabihf-gcc" CACHE STRING "Updated variable" FORCE)

# 设置链接库
SET(GCC_USER_LINK_FLAGS    "-lm") # 自己用到的库链接

# 指定inlcude目录
include_directories(include)
include_directories(ui)

# 去指定目录下执行该目录下的cmake配置文件
add_subdirectory(ui/lvgl)
add_subdirectory(ui/lv_drivers)

# 添加可执行文件
add_executable(${PROJECT_NAME} "sys/main.c"  "ui/ui.c" )

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE ${GCC_USER_LINK_FLAGS})
target_link_libraries(${PROJECT_NAME} PRIVATE lvgl lvgl::examples lvgl::drivers)