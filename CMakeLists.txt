cmake_minimum_required(VERSION 3.16)
project(LRUCache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(lru_cache STATIC
    src/cache.cpp
)

target_include_directories(lru_cache
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(MSVC)
    target_compile_options(lru_cache PRIVATE /W4 /permissive-)
else()
    target_compile_options(lru_cache PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Исполняемый файл с main()
add_executable(lru_demo
    src/main.cpp
)

# Линкуем кэш-библиотеку к демке
target_link_libraries(lru_demo PRIVATE lru_cache)
