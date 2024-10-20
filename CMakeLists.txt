cmake_minimum_required(VERSION 3.29)

project(HoyoGameUnlocker)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 指定头文件的搜索路径
include_directories(${CMAKE_SOURCE_DIR}/include)

# 查找src目录下的所有.cpp文件
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/HoyoGameUnlocker/src/*.cpp")

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})
