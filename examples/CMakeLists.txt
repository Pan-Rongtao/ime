
# 0. CMake Defs

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

cmake_policy(VERSION 2.6)

# 1. Project Name

project(examples)

# 2. Env (Include/Lib Path, C/CXX/LD FLAGS)
#NA

# 4. Sub Projects

add_subdirectory(test-pinyin)
add_subdirectory(test-associate)
add_subdirectory(test-edic)
add_subdirectory(test-stroke)
add_subdirectory(test-wubi)

# 5. Project Install
