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
   TRY_CXX_COMPILER_FLAG(-Wall      FLAG_CXX_Wall   )
   TRY_CXX_COMPILER_FLAG(-Wextra    FLAG_CXX_Wextra )
   TRY_CXX_COMPILER_FLAG(-Wpedantic FLAG_CXX_Wpedantic)
   TRY_CXX_COMPILER_FLAG(-Wshadow   FLAG_CXX_Wshadow)

   TRY_CXX_COMPILER_FLAG(-std=c++11 FLAG_CXX_CXX11)
endif()


##----------------------------------------------------------------------------|
## Environment Configuration                                                  |
##

##
## GDCC_SHARED
##
## If true (or equiavalent), libraries will be built as SHARED. Otherwise,
## they are built as STATIC.
##
if(NOT DEFINED GDCC_SHARED)
   set(GDCC_SHARED TRUE CACHE BOOL "Build libraries as shared objects.")
endif()

##
## GDCC_SHARED_DECL
##
## Used internally for convenience in add_library commands.
##
if(GDCC_SHARED)
   set(GDCC_SHARED_DECL SHARED)
else()
   set(GDCC_SHARED_DECL STATIC)
endif()

include_directories(inc src)


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_subdirectory(src)

## EOF

