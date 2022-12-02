cmake_minimum_required(VERSION 3.0)
project(wuchat)

# 可执行文件路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 设置调试选项，设置 C++ 11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

set(LIBS luwu pthread dl ssl crypto)

# 编译生成可执行文件
add_executable(wuchat main.cpp chat_servlet.cpp)
target_link_libraries(wuchat ${LIBS})