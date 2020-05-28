cmake_minimum_required (VERSION 3.0)
set(CMAKE_BUILD_TYPE Debug)

project (SocialNet)

file(GLOB_RECURSE SocialNet_SRC

	"src/*.h"
	"src/*.cpp"
)

add_executable(SocialNet ${SocialNet_SRC})


