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
## GDCC_INSTALL_PART
##
function(GDCC_INSTALL_PART name wantExe wantLib)
   if(wantExe AND GDCC_INSTALL_EXE)
      install(TARGETS gdcc-${name}
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib
      )
   endif()

   if(wantLib AND GDCC_INSTALL_LIB)
      if(GDCC_INSTALL_API)
         install(TARGETS gdcc-${name}-lib
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
         )
      else()
         install(TARGETS gdcc-${name}-lib
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
         )
      endif()
   endif()
endfunction()

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
## Variables                                                                  |
##

##
## GDCC_INSTALL_API
##
if(NOT DEFINED GDCC_INSTALL_API)
   set(GDCC_INSTALL_API OFF CACHE BOOL "Install headers.")
endif()

##
## GDCC_INSTALL_DOC
##
if(NOT DEFINED GDCC_INSTALL_DOC)
   set(GDCC_INSTALL_DOC ON CACHE BOOL "Install documentation.")
endif()

##
## GDCC_INSTALL_EXE
##
if(NOT DEFINED GDCC_INSTALL_EXE)
   set(GDCC_INSTALL_EXE ON CACHE BOOL "Install executables.")
endif()

##
## GDCC_INSTALL_LIB
##
if(NOT DEFINED GDCC_INSTALL_LIB)
   set(GDCC_INSTALL_LIB ON CACHE BOOL "Install libraries.")
endif()

##
## GDCC_SHARED
##
## If true (or equiavalent), libraries will be built as SHARED. Otherwise,
## they are built as STATIC.
##
if(NOT DEFINED GDCC_SHARED)
   set(GDCC_SHARED ON CACHE BOOL "Build libraries as shared objects.")
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


##----------------------------------------------------------------------------|
## Environment Configuration                                                  |
##

include_directories(inc src)


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_subdirectory(src)

if(GDCC_INSTALL_API)
   install(DIRECTORY inc/ DESTINATION include/GDCC)
endif()

if(GDCC_INSTALL_DOC)
   install(DIRECTORY doc/ DESTINATION share/gdcc/doc)
endif()

## EOF

