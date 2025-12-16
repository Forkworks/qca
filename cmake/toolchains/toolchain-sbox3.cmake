#----------------------------------------------------------------------------------
# Plantium custom configuration
#----------------------------------------------------------------------------------
# This is for compiling for ecuVision2
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR aarch64)
SET(CMAKE_HOST_SYSTEM_PROCESSOR x86_64)

SET(CMAKE_SYSROOT     "/mnt/sbox3-mainApp" CACHE INTERNAL "" FORCE)

SET(CMAKE_LIBRARY_ARCHITECTURE aarch64-linux-gnu)
SET(CROSSCOMPILE TRUE)

SET(COMPILER_ROOT /opt/sbox-sdk/toolchains/gcc-linaro-7.5.0-aarch64-linux-gnu/bin)
SET(CMAKE_C_COMPILER ${COMPILER_ROOT}/aarch64-linux-gnu-gcc)
SET(CMAKE_CXX_COMPILER ${COMPILER_ROOT}/aarch64-linux-gnu-g++)

SET(CMAKE_SYSROOT_COMPILE   "${CMAKE_SYSROOT}" CACHE INTERNAL "" FORCE)
SET(CMAKE_SYSROOT_LINK      "${CMAKE_SYSROOT}" CACHE INTERNAL "" FORCE)

set(CMAKE_EXPORT_COMPILE_COMMANDS=ON)
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

#----------------------------------------------------
# Linker Configuration 
#----------------------------------------------------
set(QT_PATH ${CMAKE_SYSROOT}/opt/sbox-sdk/QT5_10_1/)

SET(CMAKE_PREFIX_PATH  ${QT_PATH})

SET(CMAKE_C_FLAGS        "--sysroot=${CMAKE_SYSROOT}" CACHE INTERNAL "" FORCE)
SET(CMAKE_C_LINK_FLAGS   "--sysroot=${CMAKE_SYSROOT}" CACHE INTERNAL "" FORCE)
SET(CMAKE_CXX_FLAGS      "--sysroot=${CMAKE_SYSROOT}" CACHE INTERNAL "" FORCE)
SET(CMAKE_CXX_LINK_FLAGS "--sysroot=${CMAKE_SYSROOT}" CACHE INTERNAL "" FORCE)

# Flags for libraries
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC -D_FORCE_INLINES ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-rpath-link,${CMAKE_SYSROOT}/usr/local/lib/ ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-rpath-link,${CMAKE_SYSROOT}/lib/${CMAKE_LIBRARY_ARCHITECTURE}")	
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-rpath-link,${CMAKE_SYSROOT}/usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}")	
