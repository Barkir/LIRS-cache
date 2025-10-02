cmake_minimum_required(VERSION 3.16)
project(LRUCache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMMON_HEADERS
    inlcude/cache.hpp
    include/lirs.hpp
    include/main.hpp
)

add_executable(lru
    src/main.cpp
    include/cache.hpp
    include/main.hpp
)


add_executable(lirs
    src/main_lirs.cpp
    include/lirs.hpp
    include/main.hpp
)

target_include_directories(lru PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(lirs PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Настройки компилятора
if(MSVC)
    target_compile_options(lru PRIVATE /W4 /permissive-)
    target_compile_options(lirs PRIVATE /W4 /permissive-)
else()
    target_compile_options(lru PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(lirs PRIVATE -Wall -Wextra -Wpedantic)
endif()
