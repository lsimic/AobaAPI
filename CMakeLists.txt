# CMakeList.txt : CMake project for AobaAPI, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.11)
set (CMAKE_CXX_STANDARD 11)

project(
	AobaAPI
	VERSION 0.1
	DESCRIPTION "3D mesh modeling add_library"
	LANGUAGES CXX
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(CMAKE_CXX_EXTENSIONS OFF)
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	find_package(Doxygen)
	if(Doxygen_FOUND)
		add_subdirectory(docs)
	else()
		message(STATUS "Doxygen not found, not building docs.")
	endif()
endif()

add_subdirectory(src)
