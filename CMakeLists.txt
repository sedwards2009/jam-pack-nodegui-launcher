cmake_minimum_required(VERSION 3.0)
project (jam-pack-nodegui-launcher)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

if (MSVC)
  # Sets compile options for this directory and all subdirectories.
  add_compile_options(/W4)
else()
  add_compile_options(-W -Wall)
endif()

include_directories(${jam-pack-nodegui-launcher_SOURCE_DIR}/src)

if(CMAKE_HOST_WIN32)
  set(CMAKE_GENERATOR_PLATFORM "x64")
  add_executable(jam-pack-nodegui-launcher src/main.c)
	target_compile_definitions(jam-pack-nodegui-launcher PRIVATE -D_CRT_SECURE_NO_WARNINGS)

elseif(CMAKE_HOST_APPLE)
  add_executable(jam-pack-nodegui-launcher src/main.c)

elseif(CMAKE_HOST_UNIX)
  add_executable(jam-pack-nodegui-launcher src/main.c)

  SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -fstack-protector")
  SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS}  -Wall -fstack-protector")
endif()
