cmake_minimum_required(VERSION 2.8)
project(DLoopDetector)
include(ExternalProject)
set(CMAKE_CXX_STANDARD 11)

option(BUILD_DemoBRIEF  "Build demo application with BRIEF features" ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

if(MSVC)
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
endif()

set(HDRS
  include/DLoopDetector/DLoopDetector.h         include/DLoopDetector/TemplatedLoopDetector.h)

set(DEPENDENCY_DIR ${CMAKE_CURRENT_BINARY_DIR}/dependencies)
set(DEPENDENCY_INSTALL_DIR ${DEPENDENCY_DIR}/install)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

macro(GetDependency name other_dependency)
  find_package(${name} QUIET
    PATHS ${DEPENDENCY_INSTALL_DIR})
  if(${${name}_FOUND})
    message("${name} library found, using it from the system")
    include_directories(${${name}_INCLUDE_DIRS})
    add_custom_target(${name}_dep)
  else(${${name}_FOUND})
    message("${name} library not found in the system, it will be downloaded on build")
    option(DOWNLOAD_${name}_dependency "Download ${name} dependency" ON)
    if(${DOWNLOAD_${name}_dependency})
      if(NOT ${other_dependency})
        set(dependency ${other_dependency}_dep)
      endif()
      ExternalProject_Add(${name}
        PREFIX ${DEPENDENCY_DIR}
        #GIT_REPOSITORY http://github.com/dorian3d/${name}
        #GIT_TAG master
        INSTALL_DIR ${DEPENDENCY_INSTALL_DIR}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
        DEPENDS ${dependency})
      add_custom_target(${name}_dep ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR} DEPENDS ${name})
    else()
      message(SEND_ERROR "Please, activate DOWNLOAD_${name}_dependency option or download manually")
    endif(${DOWNLOAD_${name}_dependency})
  endif(${${name}_FOUND})
endmacro(GetDependency)

GetDependency(DLib "")
GetDependency(DBoW2 DLib)
add_custom_target(Dependencies ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR} DEPENDS DBoW2_dep DLib_dep)

include_directories(include/DLoopDetector/)

if(BUILD_DemoBRIEF)
  add_executable(demo_brief demo/demo_brief.cpp)
  add_dependencies(demo_brief DLib_dep DBoW2_dep)
  target_link_libraries(demo_brief ${OpenCV_LIBS} ${DLib_LIBS} ${DBoW2_LIBS})
endif(BUILD_DemoBRIEF)


  add_executable(VocCreator demo/VocCreator.cpp)
  add_dependencies(VocCreator DLib_dep DBoW2_dep)
  target_link_libraries(VocCreator ${OpenCV_LIBS} ${DLib_LIBS} ${DBoW2_LIBS})
#if(BUILD_DemoSURF)
#  add_executable(demo_surf demo/demo_surf.cpp)
#  target_link_libraries(demo_surf ${OpenCV_LIBS} ${DLib_LIBS} ${DBoW2_LIBS})
#endif(BUILD_DemoSURF)

if(BUILD_DemoBRIEF)
  set(RESOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/resources/)
  set(RESOURCE_FILE ${CMAKE_CURRENT_BINARY_DIR}/resources.tar.gz)
  if(NOT EXISTS ${RESOURCE_DIR})
    if(EXISTS ${RESOURCE_FILE})
      execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${RESOURCE_FILE})
    else()
      message("To run the demo application, please download \
https://drive.google.com/uc?export=download&id=1MpZwPjXDAUxKfSTpeCjG0PAUpaeWuo7D \
into ${RESOURCE_FILE} and run cmake again.")
    endif()
  endif(NOT EXISTS ${RESOURCE_DIR})
endif(BUILD_DemoBRIEF)

configure_file(src/DLoopDetector.cmake.in
  "${PROJECT_BINARY_DIR}/DLoopDetectorConfig.cmake" @ONLY)

install(DIRECTORY include/DLoopDetector DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/DLoopDetectorConfig.cmake"
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME})
install(FILES "${PROJECT_BINARY_DIR}/DLoopDetectorConfig.cmake" 
  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/DLoopDetector/)
install(DIRECTORY ${DEPENDENCY_INSTALL_DIR}/ DESTINATION ${CMAKE_INSTALL_PREFIX} OPTIONAL)
