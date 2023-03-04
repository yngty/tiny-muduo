cmake_minimum_required(VERSION 3.14)
project(mydumuo)

file(GLOB HEAD_FILES
        ${LIBMUDUO_PROJECT_DIR}/include/*.h)
aux_source_directory(${LIBMUDUO_PROJECT_DIR}/src SRC_FILES)

add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEAD_FILES})
add_library(DUMUO::library ALIAS ${PROJECT_NAME})

# set output directory
set_target_properties(${PROJECT_NAME}
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
# set version
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    VERSION 0.0.1
    SOVERSION 3)

target_include_directories(${PROJECT_NAME}
        PUBLIC  ${LIBMUDUO_PROJECT_DIR}/include)
install(
  TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)