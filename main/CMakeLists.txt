cmake_minimum_required(VERSION 3.24)

set(TARGET_NAME service)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${TARGET_NAME}
    include/stdafx.h
        src/stdafx.cpp

    include/CharacterEncoding.h
        src/CharacterEncoding.cpp
    
    include/ServiceBase.h
        src/ServiceBase.cpp

    include/Logging.h

    include/ServiceInstaller.h
        src/ServiceInstaller.cpp
    
    include/WindowsServiceImpl.h
        src/WindowsServiceImpl.cpp

    include/WindowsService.h
        src/WindowsService.cpp
)

target_include_directories(${TARGET_NAME} PRIVATE include)

set(LIBRARIES
    bcrypt
)

target_link_libraries(${TARGET_NAME} PRIVATE ${LIBRARIES})

# Enable Unicode support
target_compile_definitions(${TARGET_NAME} PRIVATE UNICODE _UNICODE)

if(MSVC)
    target_link_options(${TARGET_NAME} PRIVATE /SUBSYSTEM:CONSOLE)
endif()
