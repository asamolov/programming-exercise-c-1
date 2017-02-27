cmake_minimum_required(VERSION 3.6)
project(normalize_path)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES src/normalize.h src/normalize.c)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

add_subdirectory(ext)

add_executable(normalize_path src/main.c ${SOURCE_FILES})

add_executable(bench src/bench.cpp ${SOURCE_FILES})
target_link_libraries(bench benchmark)

add_executable(bench_prefetch src/bench.cpp ${SOURCE_FILES})
target_compile_definitions(bench_prefetch PRIVATE PREFETCH)
target_link_libraries(bench_prefetch benchmark)

add_executable(mytests src/test.cpp ${SOURCE_FILES})
target_link_libraries(mytests gtest_main)