CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Context
SET(CMAKE_BUILD_TYPE Release)
#SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -funroll-loops -Wall -Wextra")
PROJECT(RobotcupVision CXX)

# Locate the sources
FIND_PACKAGE(OpenCV REQUIRED)
FILE(GLOB_RECURSE source_files src/*.cpp)

# Libraries headers
#INCLUDE_DIRECTORIES(/usr/include/opencv2) # (manually)
#INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIR})

# Libraries to link
#LINK_DIRECTORIES(/usr/lib) # (manually)
#LINK_DIRECTORIES(${OpenCV_LIBRARIES})

# Define the executable
ADD_EXECUTABLE( RobotcupVision ${source_files} )
SET_PROPERTY( TARGET RobotcupVision PROPERTY RUNTIME_OUTPUT_DIRECTORY bin )

# Compile options
TARGET_LINK_LIBRARIES( RobotcupVision ${OpenCV_LIBS} )
#TARGET_LINK_LIBRARIES(Cypher ${OpenCV_LINK_DIRECTORIES})


#******************DEBUG******************
#SET(CMAKE_VERBOSE_MAKEFILE on)

#GET_CMAKE_PROPERTY(_variableNames VARIABLES)
#FOREACH (_variableName ${_variableNames})
#    MESSAGE(STATUS "${_variableName}=${${_variableName}}")
#ENDFOREACH()
