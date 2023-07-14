cmake_minimum_required(VERSION 3.0)

project(yet_another_interpreter)

add_executable(yet_another_interpreter YetAnotherInterpreter.cpp)

install(TARGETS yet_another_interpreter RUNTIME DESTINATION bin)
