cmake_minimum_required(VERSION 3.0 )
project(matlab)

#---ROOT libraries/modules needed by our classes
set(rootLibsToLink Core Hist Spectrum RIO Tree)

#---specify needed libraries so they are added to ${ROOT_LIBRARIES}
find_package(ROOT REQUIRED ${rootLibsToLink})


include(${ROOT_USE_FILE} )



link_directories(${ROOT_LIBRARY_DIRS} )



# needed for v6: sets cxx11 options (perhaps always needed?)
set(CMAKE_CXX_FLAGS  ${ROOT_CXX_FLAGS} )

message( ${CMAKE_CXX_FLAGS} )
#ROOT_STANDARD_LIBRARY_PACKAGE(${libName} DEPENDENCIES ${ROOT_LIBRARIES})



set(TARGET_H tsegment.h  )

set(TARGET_SRC tsegment.cpp )


add_executable(matlab main.cpp ${TARGET_H} ${TARGET_SRC}  )
target_link_libraries(matlab ${rootLibsToLink})


#install(TARGETS matlab RUNTIME DESTINATION bin)
