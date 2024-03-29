# CMake 最低版本要求
cmake_minimum_required(VERSION 2.8)

# 项目信息
project(myTinyWebserver)

# 查找包
find_package(Threads)

# 添加头路径
include_directories({CMAKE_CURRENT_LIST_DIR}/inc)

# 添加源文件路径，并将其保存在变量source_path中(递归查找)
file(GLOB_RECURSE SRC_DIR_LIST "src/*.cpp" "src/*.c")
# aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src source_path)

# 指定生成目标，引用source_path变量
add_executable(${PROJECT_NAME} ${SRC_DIR_LIST})

# 将资源拷贝到可执行文件目录下
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resource DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# 显式指定编译器
set(CMAKE_CXX_COMPILER "g++")

# 开启调试信息
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAG_DEBUG "$ENV{CXXFLAGS} -00 -Wall -g2 --ggdb")
set(CMAKE_CXX_FLAG_RELEASE "$ENV{CXXFLAGS} -03 -Wall")

# 开启所有警告
set(CMAKE_CXX_FLAG "-Wall")

# 目标链接库， 表示当前目标文件需要链接库
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})