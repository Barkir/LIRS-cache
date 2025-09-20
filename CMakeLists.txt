cmake_minimum_required(VERSION 3.16)
project(LRUCache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lru_demo
    src/main.cpp
    include/cache.hpp
    include/main.hpp
    include/lirs.hpp
)

# Устанавливаем include директории
target_include_directories(lru_demo
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Настройки компилятора
if(MSVC)
    target_compile_options(lru_demo PRIVATE /W4 /permissive-)
else()
    target_compile_options(lru_demo PRIVATE -Wall -Wextra -Wpedantic)
endif()
