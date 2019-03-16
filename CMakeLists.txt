
# 0. CMake Defs

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

cmake_policy(VERSION 2.6)

# 1. Project Name

project(ime)

# 2. Env (Include/Lib Path, C/CXX/LD FLAGS)
set(CMAKE_DEBUG_POSTFIX d)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL QNX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

if ("${CMAKE_INSTALL_PREFIX}" STREQUAL "")
	SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/dist)
endif()

SET(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_PREFIX}/include)
SET(CMAKE_INSTALL_LIBDIR ${CMAKE_INSTALL_PREFIX}/lib)
SET(CMAKE_INSTALL_BINDIR ${CMAKE_INSTALL_PREFIX}/bin)
SET(CMAKE_INSTALL_ETCDIR ${CMAKE_INSTALL_PREFIX}/etc)
SET(CMAKE_INSTALL_DOCDIR ${CMAKE_INSTALL_PREFIX}/doc)


include_directories(include)
if(WIN32)
	include_directories(${CMAKE_SOURCE_DIR}/extern/include)
	link_directories(${CMAKE_SOURCE_DIR}/extern/lib/win32)
endif()

# 4. Sub Projects

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(extern)
add_subdirectory(etc)
add_subdirectory(doc)

# 5. Project Install
