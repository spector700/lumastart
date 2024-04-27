cmake_minimum_required(VERSION 3.23)

set(VERSION 0.0.1)

project(lumastart
        DESCRIPTION "launcher"
        VERSION ${VERSION}
)

if(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES DEBUG)
    message(STATUS "Configuring lumastart in Debug with CMake")
else()
    message(STATUS "Configuring lumastart in Release with CMake")
endif()

# configure
set(CMAKE_CXX_STANDARD 23)
add_compile_options(-Wall -Wextra -Wno-unused-parameter -Wno-unused-value
	-Wno-missing-field-initializers -Wno-narrowing)

file(GLOB_RECURSE SRCFILES CONFIGURE_DEPENDS "src/*.cpp")

# dependencies
message(STATUS "Checking deps...")

find_package(PkgConfig REQUIRED)
pkg_check_modules(deps REQUIRED IMPORTED_TARGET gtkmm-4.0 gtk4 glibmm-2.68 cairomm-1.16 pangomm-2.48)

add_executable(lumastart ${SRCFILES})
target_link_libraries(lumastart PUBLIC PkgConfig::deps)

# Installation
install(TARGETS lumastart)
