# Set minimum version of CMake.
cmake_minimum_required(VERSION 3.20)

# Set project name and version
project(CLIc VERSION 0.5.4)

# Set environment variables
include(${PROJECT_SOURCE_DIR}/cmake/SetEnv.cmake)

# find required package
if(NOT OpenCL_FOUND)
    find_package(OpenCL REQUIRED)
endif()

if(NOT Python3_FOUND)
    message(STATUS "Python_EXECUTABLE ${PYTHON_EXECUTABLE} Python3_EXECUTABLE ${Python3_EXECUTABLE} PYTHON3_ROOT_DIR ${PYTHON3_ROOT_DIR} PYTHON_INCLUDE_DIR:PATH ${PYTHON_INCLUDE_DIR}")
    find_package(Python3 COMPONENTS Interpreter Development)
  if(NOT Python3_EXECUTABLE AND _Python3_EXECUTABLE) # workaround for cases where FindPython3 fails to set correctly
    set(Python3_EXECUTABLE ${_Python3_EXECUTABLE} CACHE INTERNAL
      "Path to the Python interpreter" FORCE)
  endif()
endif()

# Library sources
add_subdirectory(${THIRDPARTY_DIR})
add_subdirectory(${LIBRARY_DIR})

# Build and run tests
if(BUILD_TESTING)
    include(CTest)
    add_subdirectory(${LIBRARY_DIR}/tests)
    add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif(BUILD_TESTING)

# Build and run benchmark
if(BUILD_BENCHMARK)
    add_subdirectory(benchmark)
endif(BUILD_BENCHMARK)

if(BUILD_DOCUMENTATION)
    # find_package(Doxygen)
    # if(DOXYGEN_FOUND)
    #     # set input and output files
    #     set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    #     set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    #     # request to configure the file
    #     configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    #     message("Doxygen build started")
    #     # note the option ALL which allows to build the docs together with the application
    #     add_custom_target( doc_doxygen ALL
    #         COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    #         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    #         COMMENT "Generating API documentation with Doxygen"
    #         VERBATIM )
    # else(DOXYGEN_FOUND)
    #     message("Doxygen need to be installed to generate the documentation")
    # endif(DOXYGEN_FOUND)
endif(BUILD_DOCUMENTATION)
