cmake_minimum_required(VERSION 3.24)
project(WindowsServiceTemplate)

# https://stackoverflow.com/a/40947954/13680015
string(LENGTH "${CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)
add_definitions("-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}")

# Print relative paths to source code in logs
# instead of absolute paths (which are better
# for debugging but not for Release builds).
add_definitions("-DDEBUGGING_RELATIVE_PATH")

##########################
##### STATIC LINKING #####
#### so that it could ####
## run on older systems ##
## without VC redist... ##
##########################

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
if (MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
endif()

##########################



###############################
##### INCREASE STACK SIZE #####
###############################

set(STACK_SIZE 16777216) # 16 MB
if (MSVC)
    add_link_options(/STACK:${STACK_SIZE})
elseif (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    if (WIN32)
        add_link_options("-Wl,--stack,${STACK_SIZE}")
    else()
        # Linux, BSD, macOS: raise stack at runtime via setrlimit()
        # add_compile_definitions(SET_STACK_AT_RUNTIME)
		# But this program only runs on Windows so...
		message(FATAL_ERROR "This program can only be built on Windows.")
    endif()
endif()

###############################



add_subdirectory(main)
