cmake_minimum_required (VERSION 3.0)
set(CMAKE_BUILD_TYPE Debug)

project (Blackjack)

file(GLOB_RECURSE Blackjack_SRC

	"src/*.h"
	"src/*.cpp"
)

add_executable(Blackjack ${Blackjack_SRC})



