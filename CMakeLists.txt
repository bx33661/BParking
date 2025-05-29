cmake_minimum_required(VERSION 3.10)
project(parking_system C)

set(CMAKE_C_STANDARD 99)

# 添加可执行文件
add_executable(parking_system main.c parking.c)

# 如果有额外的包含目录，可以添加
# include_directories(include)

# 如果需要链接其他库，可以添加
# target_link_libraries(parking_system other_lib)
