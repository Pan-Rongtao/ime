
####################################################################################################

# Company       : Huizhou Desay SV Automotive Co., Ltd.

# Division      : Automotive Electronics, Desay Corporation

# Business Unit : Central Technology

# Department    : Advanced Development (Huizhou)

####################################################################################################

# 1. Project Name

project(hwr)

# 2. Project Env (Include/Lib Path, C/CXX/LD FLAGS)

# 3. Project Build

set(LIB_NAME "hwr")

file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/*.h" "*.h" "*.cpp")

add_library(${LIB_NAME} SHARED ${SRC_FILES})
target_link_libraries(${LIB_NAME} hci_sys hci_hwr)

# 4. Project Install
if(WIN32)
	install(TARGETS ${LIB_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR}/debug CONFIGURATIONS debug)
	install(TARGETS ${LIB_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR}/release CONFIGURATIONS release)
elseif(UNIX AND NOT ANDROID)
	install(TARGETS ${LIB_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()
