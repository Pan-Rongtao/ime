# 1. Project Name

project(resource)

# 2. Env (Include/Lib Path, C/CXX/LD FLAGS)


# 4. Sub Projects

# 5. Project Install
file(GLOB SOURCES *.db)
install(FILES ${SOURCES} DESTINATION ${CMAKE_INSTALL_ETCDIR})
install(DIRECTORY hwr DESTINATION ${CMAKE_INSTALL_ETCDIR})

