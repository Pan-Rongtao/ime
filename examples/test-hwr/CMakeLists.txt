
####################################################################################################

# Company       : Huizhou Desay SV Automotive Co., Ltd.

# Division      : Automotive Electronics, Desay Corporation

# Business Unit : Central Technology

# Department    : Advanced Development (Huizhou)

####################################################################################################

# 1. Project Name

project(test-hwr)

# 2. Project Env (Include/Lib Path, C/CXX/LD FLAGS)

# 3. Project Build

set(BIN_NAME "test-hwr")

file(GLOB SRC_FILES "*.h" "*.cpp" "../test-share/*.h" "../test-share/*.cpp")
add_executable(${BIN_NAME} ${SRC_FILES})
set_target_properties(${BIN_NAME} PROPERTIES OUTPUT_NAME ime.${BIN_NAME})
target_link_libraries(${BIN_NAME} hwr)
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "../../../dist/lib")

# 4. Project Install
install(TARGETS ${BIN_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})