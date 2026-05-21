cmake_minimum_required(VERSION 3.16)

project(pomodoro_timer C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

add_executable(pomodoro
    src/main.c
    src/timer.c
    src/ui.c
)

target_include_directories(pomodoro PRIVATE
    ${GTK4_INCLUDE_DIRS}
)

target_link_directories(pomodoro PRIVATE
    ${GTK4_LIBRARY_DIRS}
)

target_link_libraries(pomodoro PRIVATE
    ${GTK4_LIBRARIES}
)

target_compile_options(pomodoro PRIVATE
    ${GTK4_CFLAGS_OTHER}
)

target_link_options(pomodoro PRIVATE
    ${GTK4_LDFLAGS_OTHER}
)
