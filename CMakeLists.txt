cmake_minimum_required (VERSION 3.5)
project (Trader)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(SOURCE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/src)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

MESSAGE(STATUS "Project name is " ${PROJECT})
MESSAGE(STATUS "This is EXECUTE dir " ${EXECUTABLE_OUTPUT_PATH})
MESSAGE(STATUS "This is SOURCE dir " ${SOURCE_OUTPUT_PATH})
MESSAGE(STATUS "This is LIBRARY dir " ${LIBRARY_OUTPUT_PATH})

# include h files
include_directories(inc)

add_subdirectory(src)








