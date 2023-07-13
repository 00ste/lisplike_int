cmake_minimum_required(VERSION 3.0)

project(lisplike_manager)

add_executable(lisplike_manager main.cpp)

install(TARGETS lisplike_manager RUNTIME DESTINATION bin)
