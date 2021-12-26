cmake_minimum_required(VERSION 3.20)
project(debugbreak VERSION 1.0.0)

add_library(debugbreak INTERFACE)
target_include_directories(debugbreak PUBLIC ./)
