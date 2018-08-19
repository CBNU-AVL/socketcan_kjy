cmake_minimum_required(VERSION 2.6)
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#macro(use_cxx11)
  if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
    endif ()
  else ()
    set (CMAKE_CXX_STANDARD 11)
  endif ()
#endmacro(use_cxx11)

project(JY)

include_directories(/home/kimjy/qt_cmake_test)	# 헤더파일 PATH 넣기

link_directories()	# 라이브러리파일 path
link_libraries()	# 라이브러리파일 이름 (lib이름.so)ss

add_executable(JY1 test.cpp)
add_executable(JY2 test_2.cpp)
add_library(joystick joystick.cc)
target_link_libraries(JY2 joystick)
