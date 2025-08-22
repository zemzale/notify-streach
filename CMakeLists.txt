cmake_minimum_required(VERSION 3.10)
project(stretch-reminder)

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Ensure we're on Linux
if(NOT UNIX OR APPLE)
    message(FATAL_ERROR "This application only supports Linux platforms")
endif()

# Find libnotify package
find_package(PkgConfig REQUIRED)
pkg_check_modules(NOTIFY REQUIRED libnotify)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set optimization flags
set(CMAKE_CXX_FLAGS_RELEASE "-Os -flto -s")
set(CMAKE_BUILD_TYPE Release)

# Add executable
add_executable(stretch-reminder src/main.cpp)

# Link libraries
target_include_directories(stretch-reminder PRIVATE ${NOTIFY_INCLUDE_DIRS})
target_link_libraries(stretch-reminder ${NOTIFY_LIBRARIES})

# Install target
install(TARGETS stretch-reminder DESTINATION bin)
