# 1. Project Name

project(doc)

# 2. Env (Include/Lib Path, C/CXX/LD FLAGS)

# 4. Sub Projects

# 5. Project Install

file(GLOB SOURCES *.ppt* *.doc*)
install(FILES ${SOURCES} DESTINATION ${CMAKE_INSTALL_DOCDIR})
