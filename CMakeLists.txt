cmake_minimum_required (VERSION 2.8.11)
project (TinCan)
set(CMAKE_CXX_FLAGS "-std=c++11 -fno-exceptions")

link_directories(/usr/local/lib)

find_package(PkgConfig REQUIRED)

include_directories (/usr/local/include ${CMAKE_CURRENT_SOURCE_DIR})
set (TINCANDEPS Stick)

set (TINCANINC 
TinCan/SerialPort.hpp
TinCan/SerialPortInfo.hpp
TinCan/Posix/PosixSerialPortImpl.hpp
)

set (TINCANSRC 
TinCan/SerialPort.cpp
TinCan/SerialPortInfo.cpp
TinCan/Posix/PosixSerialPortImpl.cpp
)

add_library (TinCan SHARED ${TINCANSRC})
add_library (TinCanStatic STATIC ${TINCANSRC})

target_link_libraries(TinCan ${TINCANDEPS})
target_link_libraries(TinCanStatic ${TINCANDEPS})

install (TARGETS TinCan TinCanStatic DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
foreach ( file ${TINCANINC} )
    get_filename_component( dir ${file} DIRECTORY )
    install( FILES ${file} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${dir} )
endforeach()
