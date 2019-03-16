
####################################################################################################

# Company       : Huizhou Desay SV Automotive Co., Ltd.

# Division      : Automotive Electronics, Desay Corporation

# Business Unit : Central Technology

# Department    : Advanced Development (Huizhou)

####################################################################################################

# 1. Project Name

project(edic)

# 2. Project Env (Include/Lib Path, C/CXX/LD FLAGS)

# 3. Project Build

set(LIB_NAME "edic")

file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/*.h" "*.h" "*.cpp")

add_library(${LIB_NAME} SHARED ${SRC_FILES})
target_link_libraries(${LIB_NAME} sqlite3)

# 4. Project Install
install(TARGETS ${LIB_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})