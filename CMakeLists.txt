cmake_minimum_required(VERSION 3.16)
project(LRUCache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER 		"/usr/bin/clang++-20")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
# set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")

set(COMMON_HEADERS
    include/cache.hpp
    include/lirs.hpp
    include/stats.hpp
    include/main.hpp
    include/ideal.hpp
)

add_executable(lru
    src/main.cpp
    include/cache.hpp
    include/stats.hpp
    include/main.hpp
)

add_executable(ideal
    src/main_ideal.cpp
    include/ideal.hpp
    include/stats.hpp
    include/main.hpp
)


add_executable(lirs
    src/main_lirs.cpp
    include/lirs.hpp
    include/stats.hpp
    include/main.hpp
)

target_include_directories(lru PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(lirs PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(ideal PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Настройки компилятора
if(MSVC)
    target_compile_options(lru PRIVATE /W4 /permissive-)
    target_compile_options(lirs PRIVATE /W4 /permissive-)
    target_compile_options(ideal PRIVATE /W4 /permissive-)
else()
    target_compile_options(lru PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(lirs PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(ideal PRIVATE -Wall -Wextra -Wpedantic)
endif()
