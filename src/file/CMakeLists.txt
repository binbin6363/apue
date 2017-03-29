cmake_minimum_required(VERSION 2.6)

PROJECT(file_io)

SET (EXE_OUTPUT_PATH ../../bin)

ADD_DEFINITIONS(-g -W -Wall)

#ADD_LIBRARY(file STATIC
#	file_io.c
#)

add_executable(file_io
    file_io.c
    )

#INSTALL(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION ${EXE_OUTPUT_PATH})
