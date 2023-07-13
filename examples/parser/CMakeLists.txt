cmake_minimum_required(VERSION 3.0)

project(parser)

add_executable(parser ExpressionParser.cpp)

install(TARGETS parser RUNTIME DESTINATION bin)
