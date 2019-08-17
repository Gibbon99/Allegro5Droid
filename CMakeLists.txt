cmake_minimum_required(VERSION 3.14)
project(Allegro5Droid)

set(CMAKE_CXX_STANDARD 14)

# Set default locations
set(CL_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})

set(CONFIGURATION_NAME "Debug")

if (DEBUG_LEVEL)
    add_definitions(-DDEBUG_LEVEL=${DEBUG_LEVEL})
endif ()

include_directories(
        .
        hdr
)

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L. -L/usr/local/lib ")

# Define the CXX sources
set ( CXX_SRCS
        ${CMAKE_CURRENT_LIST_DIR}/src/system/sys_main.cpp
        src/system/sys_init.cpp
        hdr/system/sys_init.h
        src/system/sys_gameFrameUpdate.cpp
        hdr/system/sys_gameFrameUpdate.h
        src/system/sys_gameFrameRender.cpp
        hdr/system/sys_gameFrameRender.h
        src/system/sys_shutdown.cpp
        hdr/system/sys_shutdown.h
        src/system/sys_gameEvents.cpp
        hdr/system/sys_gameEvents.h src/system/sys_configFile.cpp hdr/system/sys_configFile.h src/system/sys_font.cpp hdr/system/sys_font.h src/system/sys_util.cpp hdr/system/sys_util.h)

set_source_files_properties(
        ${CXX_SRCS} PROPERTIES COMPILE_FLAGS
        " -pg -g -O0")
#        " -pg -g -O0 -fsanitize=address -fno-omit-frame-pointer")	# Address sanitizer - and memleak
#        " -pg -g -O0 -fsanitize=thread -fno-omit-frame-pointer")	# Thread sanitizer

add_executable(Allegro5Droid ${RC_SRCS} ${CXX_SRCS} ${C_SRCS})
target_link_libraries(Allegro5Droid ${LINK_OPTIONS})


target_link_libraries(Allegro5Droid
        #        asan		# Used for address sanitizer
        #		tsan		# Used for thread sanitizer
        allegro
        allegro_dialog
        allegro_font
        allegro_primitives
        )
