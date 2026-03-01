cmake_minimum_required(VERSION 3.15)

project(Snake LANGUAGES C)

include(GNUInstallDirs)

find_package(Curses REQUIRED)

include_directories(../include)

add_executable(snake main.c)

target_link_libraries(snake ${CURSES_LIBRARIES} cvector)

target_link_options(snake PRIVATE -fsanitize=address)

# Pass the installed data directory path as a compile-time definition
target_compile_definitions(snake PRIVATE
    SNAKE_DATA_DIR="${CMAKE_INSTALL_FULL_DATADIR}/snake"
)

# Install the snake executable
install(TARGETS snake
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install the high score file with owner and group write permissions
install(FILES ../files/high_score.txt
    DESTINATION ${CMAKE_INSTALL_DATADIR}/snake
    PERMISSIONS
        OWNER_READ OWNER_WRITE
        GROUP_READ GROUP_WRITE
        WORLD_READ
)