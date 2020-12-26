cmake_minimum_required (VERSION 3.0)
set(CMAKE_BUILD_TYPE Debug)

project (Elevators)

file(GLOB_RECURSE Elevators_SRC

	"src/*.h"
	"src/*.cpp"
)

add_executable(Elevators ${Elevators_SRC})