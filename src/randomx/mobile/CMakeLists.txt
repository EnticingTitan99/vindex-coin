cmake_minimum_required(VERSION 3.10)
project(RandomXPOC CXX)

add_executable(randomx_poc randomx_poc.cpp)

# Link only pthread for iOS, as log and android are Android-specific
target_link_libraries(randomx_poc pthread)
