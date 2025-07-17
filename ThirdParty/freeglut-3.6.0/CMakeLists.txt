cmake_minimum_required(VERSION 3.10...3.29)
cmake_policy(VERSION 3.10...3.29)

project(freeglut LANGUAGES C)

# Set policies
if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif()
if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

# Define version
set(FREEGLUT_VERSION_MAJOR 3)
set(FREEGLUT_VERSION_MINOR 6)
set(FREEGLUT_VERSION_PATCH 0)
set(FREEGLUT_VERSION "${FREEGLUT_VERSION_MAJOR}.${FREEGLUT_VERSION_MINOR}.${FREEGLUT_VERSION_PATCH}")

# Output directories
include(GNUInstallDirs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Define options
option(FREEGLUT_BUILD_SHARED_LIBS "Build shared FreeGLUT library" ON)
option(FREEGLUT_BUILD_STATIC_LIBS "Build static FreeGLUT library" ON)
option(FREEGLUT_PRINT_ERRORS "Enable error messages" ON)
option(FREEGLUT_PRINT_WARNINGS "Enable warning messages" ON)

# Platform specifics
if(WIN32)
    option(FREEGLUT_REPLACE_GLUT "Build as GLUT replacement" OFF)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    if(MSVC)
        set(CMAKE_DEBUG_POSTFIX "d")
    endif()
else()
    option(FREEGLUT_REPLACE_GLUT "Build as GLUT replacement" ON)
endif()

# Source files (example subset)
set(FREEGLUT_SRCS
    src/fg_callbacks.c
    src/fg_cursor.c
    src/fg_display.c
    src/fg_ext.c
    src/fg_font_data.c
    src/fg_geometry.c
    src/fg_gl2.c
    src/fg_init.c
    src/fg_input_devices.c
    src/fg_main.c
    src/fg_misc.c
    src/fg_overlay.c
    src/fg_spaceball.c
    src/fg_state.c
    src/fg_structure.c
    src/fg_teapot.c
    src/fg_videoresize.c
    src/fg_window.c
)

# Include directory for freeglut.h
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Library creation
if(FREEGLUT_BUILD_SHARED_LIBS)
    add_library(freeglut SHARED ${FREEGLUT_SRCS})
    target_compile_definitions(freeglut PRIVATE FREEGLUT_EXPORTS)
endif()

if(FREEGLUT_BUILD_STATIC_LIBS)
    add_library(freeglut_static STATIC ${FREEGLUT_SRCS})
    target_compile_definitions(freeglut_static PUBLIC FREEGLUT_STATIC)
endif()

# Link OpenGL
find_package(OpenGL REQUIRED)
if(FREEGLUT_BUILD_SHARED_LIBS)
    target_link_libraries(freeglut PRIVATE OpenGL::GL)
endif()
if(FREEGLUT_BUILD_STATIC_LIBS)
    target_link_libraries(freeglut_static PRIVATE OpenGL::GL)
endif()

# Install headers
install(DIRECTORY include/GL DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Install libraries
install(TARGETS freeglut freeglut_static
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
