##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2016 David Hill
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

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR})
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
      elseif(GDCC_SHARED)
         install(TARGETS gdcc-${name}-lib
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
         )
      endif()
   endif()
endfunction()

##
## GDCC_TRY_CXX_FLAG
##
function(GDCC_TRY_CXX_FLAG flag name)
   CHECK_CXX_COMPILER_FLAG(${flag} GDCC_FLAG_CXX_${name})

   if(GDCC_FLAG_CXX_${name})
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
   endif()
endfunction()


##----------------------------------------------------------------------------|
## Environment Detection                                                      |
##

if(NOT GDCC_NOFLAGS)
   if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
      GDCC_TRY_CXX_FLAG(-Wall      Wall   )
      GDCC_TRY_CXX_FLAG(-Wextra    Wextra )
      GDCC_TRY_CXX_FLAG(-Wpedantic Wpedantic)
      GDCC_TRY_CXX_FLAG(-Wshadow   Wshadow)

      GDCC_TRY_CXX_FLAG(-Wno-misleading-indentation Wno_misleading_indentation)

      GDCC_TRY_CXX_FLAG(-std=c++11 CXX11)
   endif()
endif()

find_package(GMP)


##----------------------------------------------------------------------------|
## Variables                                                                  |
##

##
## GDCC_BC_MgC
##
if(NOT DEFINED GDCC_BC_MgC)
   if(EXISTS "${CMAKE_SOURCE_DIR}/src/BC/MgC")
      set(GDCC_BC_MgC ON CACHE BOOL "Enable bytecode module: MgC")
   else()
      set(GDCC_BC_MgC OFF CACHE BOOL "Enable bytecode module: MgC")
   endif()
endif()

##
## GDCC_BC_ZDACS
##
if(NOT DEFINED GDCC_BC_ZDACS)
   if(EXISTS "${CMAKE_SOURCE_DIR}/src/BC/ZDACS")
      set(GDCC_BC_ZDACS ON CACHE BOOL "Enable bytecode module: ZDACS")
   else()
      set(GDCC_BC_ZDACS OFF CACHE BOOL "Enable bytecode module: ZDACS")
   endif()
endif()

##
## GDCC_Core_BigNum
##
if(NOT DEFINED GDCC_Core_BigNum)
   set(GDCC_Core_BigNum ${GMP_FOUND} CACHE BOOL
      "Enable extended precision types and functions.")
endif()

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

configure_file(${CMAKE_SOURCE_DIR}/inc/Config.hpp ${CMAKE_BINARY_DIR}/inc/GDCC/Config.hpp)

include_directories(${CMAKE_BINARY_DIR}/inc inc src ${GMP_INCLUDE_DIRS})

if(GDCC_BC_MgC)
   add_definitions(-DGDCC_BC_MgC=1)
endif()

if(GDCC_BC_ZDACS)
   add_definitions(-DGDCC_BC_ZDACS=1)
endif()


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

add_subdirectory(src)

if(GDCC_INSTALL_API)
   install(DIRECTORY inc/ DESTINATION include/GDCC)
endif()

if(GDCC_INSTALL_DOC)
   install(DIRECTORY doc/ DESTINATION share/gdcc/doc)
   install(FILES COPYING DESTINATION share/gdcc/doc)
   install(FILES COPYLIB DESTINATION share/gdcc/doc)
endif()

install(DIRECTORY lib/ DESTINATION share/gdcc/lib)

## EOF

