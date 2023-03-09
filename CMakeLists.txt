# CMake 最低版本要求
cmake_minimum_required(VERSION 2.8)

# 项目信息
project(myTinyWebserver)

# 查找包
# find_package()

# 添加头路径
include_directories({CMAKE_CURRENT_LIST_DIR}/inc)

# 添加源文件路径，并将其保存在变量source_path中
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src source_path)

# 指定生成目标，引用source_path变量
add_executable(${PROJECT_NAME} ${source_path})

# 显式指定编译器
set(CMAKE_CXX_COMPILER "g++")

# 开启调试信息
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAG_DEBUG "$ENV{CXXFLAGS} -00 -Wall -g2 --ggdb")
set(CMAKE_CXX_FLAG_RELEASE "$ENV{CXXFLAGS} -03 -Wall")

# 开启所有警告
set(CMAKE_CXX_FLAG "-Wall")

# 目标链接库， 表示当前目标文件需要链接库
target_link_libraries(${PROJECT_NAME})