cmake_minimum_required(VERSION 3.31)
project(lutok-examples)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find installed Lutok
find_package(Lutok REQUIRED)

# Build example programs
add_executable(bindings bindings.cpp)
target_link_libraries(bindings PRIVATE Lutok::lutok)

add_executable(hello hello.cpp)
target_link_libraries(hello PRIVATE Lutok::lutok)

add_executable(interpreter interpreter.cpp)
target_link_libraries(interpreter PRIVATE Lutok::lutok)

add_executable(raii raii.cpp)
target_link_libraries(raii PRIVATE Lutok::lutok)
