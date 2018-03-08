cmake_minimum_required(VERSION 3.0)

# Name our project
project(blinking)

# Create a variable that holds the path to our libwiringPi.so file
set(WPI_PATH /home/student/rpi/wiringPi/wiringPi)

# Add the local 'include' directory and the wiringPi directory to grab headers
include_directories(include ${WPI_PATH})

# Actually find the wiringPi library object
find_library(WPI_LIB wiringPi HINTS ${WPI_PATH} NO_CMAKE_FIND_ROOT_PATH)

# Alert the user if we do not find it
if(NOT WPI_LIB)
	message(FATAL_ERROR "wiringPi library not found")
endif()

# Add all the *.c files in our source directory to our executable output
FILE(GLOB SRC_FILES src/*.c)
add_executable(blinking ${SRC_FILES})

# Link the pre-compiled wiringPi library to the executable we just declared
target_link_libraries(blinking ${WPI_LIB})
