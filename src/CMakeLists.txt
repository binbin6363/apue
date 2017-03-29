cmake_minimum_required(VERSION 2.6)

#PROJECT(file_io)
SET(CMAKE_AR ar)

ADD_DEFINITIONS(
 -Os -g -W -Wall -Wextra -fPIC
 -Wunused-variable -Wunused-parameter -Wunused-function -Wunused
 -Wno-deprecated -Woverloaded-virtual -Wwrite-strings
 -D__WUR= -D_REENTRANT -D_FILE_OFFSET_BITS=64 -DTIXML_USE_STL
 -DAC_HAS_INFO -DAC_HAS_WARNING -DAC_HAS_ERROR -DAC_HAS_CRITICAL 
)


#ADD_DEFINITIONS(-DAC_ERROR_RED)
ADD_SUBDIRECTORY(file ../bin/)

