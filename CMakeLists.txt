cmake_minimum_required(VERSION 3.0)
project(matlab)





add_executable(matlab main.cpp tsegment.cpp)



install(TARGETS matlab RUNTIME DESTINATION bin)
