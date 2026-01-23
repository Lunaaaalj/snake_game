cmake_minimum_required(VERSION 4.0)

project(Snake LANGUAGES C)

find_package(Curses REQUIRED)

include_directories(../include)

add_executable(snake main.c)

target_link_libraries(snake ${CURSES_LIBRARIES})

target_link_options(snake PRIVATE -fsanitize=address)