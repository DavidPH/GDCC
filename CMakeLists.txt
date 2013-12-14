##-----------------------------------------------------------------------------
##
## Copyright (C) 2013 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## Root CMake file.
##
##-----------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.6)

project(gdcc)

include(CheckCXXCompilerFlag)


##----------------------------------------------------------------------------|
## Functions                                                                  |
##

##
## TRY_CXX_COMPILER_FLAG
##
function(TRY_CXX_COMPILER_FLAG flag name)
   CHECK_CXX_COMPILER_FLAG(${flag} ${name})

   if(${name})
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
   endif()
endfunction()


##----------------------------------------------------------------------------|
## Environment Detection                                                      |
##

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
   TRY_CXX_COMPILER_FLAG(-Wall    FLAG_CXX_Wall   )
   TRY_CXX_COMPILER_FLAG(-Wextra  FLAG_CXX_Wextra )
   TRY_CXX_COMPILER_FLAG(-Wshadow FLAG_CXX_Wshadow)

   TRY_CXX_COMPILER_FLAG(-std=c++11 FLAG_CXX_CXX11)
endif()


##----------------------------------------------------------------------------|
## Environment Configuration                                                  |
##

include_directories(inc src)


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_subdirectory(src)

## EOF

