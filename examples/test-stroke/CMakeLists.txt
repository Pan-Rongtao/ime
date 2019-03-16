
####################################################################################################

# Company       : Huizhou Desay SV Automotive Co., Ltd.

# Division      : Automotive Electronics, Desay Corporation

# Business Unit : Central Technology

# Department    : Advanced Development (Huizhou)

####################################################################################################

# 1. Project Name

project(test-stroke)

# 2. Project Env (Include/Lib Path, C/CXX/LD FLAGS)

# 3. Project Build

set(BIN_NAME "test-stroke")

file(GLOB SRC_FILES "*.h" "*.cpp" "../test-share/*.h" "../test-share/*.cpp")
add_executable(${BIN_NAME} ${SRC_FILES})
target_link_libraries(${BIN_NAME} stroke)

# 4. Project Install
install(TARGETS ${BIN_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})