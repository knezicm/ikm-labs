cmake_minimum_required(VERSION 3.0)

# Name our project
project(hello-world)

# Add all the *.c files in our source directory to our executable output
FILE(GLOB SRC_FILES src/*.c)
add_executable(hello-world ${SRC_FILES})
