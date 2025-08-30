cmake_minimum_required(VERSION 3.26.4)

project(debugbreak VERSION 2.0)

add_library(debugbreak INTERFACE)
target_sources(debugbreak PRIVATE
	"debugbreak.h"
)
target_include_directories(debugbreak INTERFACE .)