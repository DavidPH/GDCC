##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2024 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## Root CMake file.
##
##-----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.0...3.31)

project(gdcc)

include(CheckCXXCompilerFlag)
include(CheckTypeSize)


##----------------------------------------------------------------------------|
## CMake Configuration                                                        |
##

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "release")
endif()

set(CMAKE_CXX_STANDARD 17)


##----------------------------------------------------------------------------|
## Functions                                                                  |
##

##
## GDCC_INSTALL_PART
##
function(GDCC_INSTALL_PART name nameAPI pathAPI wantExe wantLib)
   if(wantLib AND GDCC_INSTALL_API)
      install(FILES ${GDCC_${nameAPI}_H}
         DESTINATION include/GDCC/${pathAPI})
   endif()

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

set(GDCC_SHARED_DEFAULT ON)

if(NOT GDCC_NOFLAGS)
   if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
      GDCC_TRY_CXX_FLAG(-Wall    Wall)
      GDCC_TRY_CXX_FLAG(-Wextra  Wextra)
      GDCC_TRY_CXX_FLAG(-Wshadow Wshadow)

      GDCC_TRY_CXX_FLAG(-Wno-misleading-indentation Wno_misleading_indentation)
      GDCC_TRY_CXX_FLAG(-Wno-implicit-fallthrough   Wno_implicit_fallthrough)
   endif()

   if(MSVC)
      add_definitions(-D_SCL_SECURE_NO_WARNINGS)
      add_definitions(/wd4146) # Unary minus applied to unsigned.
      add_definitions(/wd4244) # Possible loss of data by conversion.
      add_definitions(/wd4800) # Forcing value to boolean. (All my hate.)
      add_definitions(/wd4996) # Deprecated declaration.
   endif()
endif()

if(MSVC)
   # Disable shared by default, as the source does not contain the needed
   # declaration annotations to make that work under MSVC.
   set(GDCC_SHARED_DEFAULT OFF)
endif()

find_package(GMP)

CHECK_TYPE_SIZE("long" GDCC_Core_SizeLong)
CHECK_TYPE_SIZE("long long" GDCC_Core_SizeLongLong)


##----------------------------------------------------------------------------|
## Variables                                                                  |
##

##
## GDCC_Option
##
if(NOT DEFINED GDCC_Option)
   if(EXISTS "${CMAKE_SOURCE_DIR}/Option")
      set(GDCC_Option ON CACHE BOOL "Enable Option module.")
   else()
      set(GDCC_Option OFF CACHE BOOL "Enable Option module.")
   endif()
endif()

##
## GDCC_Core
##
if(NOT DEFINED GDCC_Core)
   if(GDCC_Option AND EXISTS "${CMAKE_SOURCE_DIR}/Core")
      set(GDCC_Core ON CACHE BOOL "Enable core module.")
   else()
      set(GDCC_Core OFF CACHE BOOL "Enable core module.")
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
## GDCC_Target
##
if(NOT DEFINED GDCC_Target)
   if(GDCC_Core AND EXISTS "${CMAKE_SOURCE_DIR}/Target")
      set(GDCC_Target ON CACHE BOOL "Enable Target module.")
   else()
      set(GDCC_Target OFF CACHE BOOL "Enable Target module.")
   endif()
endif()

##
## GDCC_IR
##
if(NOT DEFINED GDCC_IR)
   if(GDCC_Target AND GDCC_Core_BigNum AND EXISTS "${CMAKE_SOURCE_DIR}/IR")
      set(GDCC_IR ON CACHE BOOL "Enable IR module.")
   else()
      set(GDCC_IR OFF CACHE BOOL "Enable IR module.")
   endif()
endif()

##
## GDCC_BC_DGE
##
if(NOT DEFINED GDCC_BC_DGE)
   if(GDCC_IR AND EXISTS "${CMAKE_SOURCE_DIR}/BC/DGE")
      set(GDCC_BC_DGE ON CACHE BOOL "Enable BC::DGE module.")
   else()
      set(GDCC_BC_DGE OFF CACHE BOOL "Enable BC::DGE module.")
   endif()
endif()

##
## GDCC_BC_ZDACS
##
if(NOT DEFINED GDCC_BC_ZDACS)
   if(GDCC_IR AND EXISTS "${CMAKE_SOURCE_DIR}/BC/ZDACS")
      set(GDCC_BC_ZDACS ON CACHE BOOL "Enable BC::ZDACS module.")
   else()
      set(GDCC_BC_ZDACS OFF CACHE BOOL "Enable BC::ZDACS module.")
   endif()
endif()

##
## GDCC_INSTALL_API
##
if(NOT DEFINED GDCC_INSTALL_API)
   set(GDCC_INSTALL_API ON CACHE BOOL "Install headers.")
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
## If true (or equivalent), libraries will be built as SHARED. Otherwise,
## they are built as STATIC.
##
if(NOT DEFINED GDCC_SHARED)
   set(GDCC_SHARED ${GDCC_SHARED_DEFAULT} CACHE BOOL
      "Build libraries as shared objects.")
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

configure_file(Config.hpp inc/GDCC/Config.hpp)

include_directories(${CMAKE_BINARY_DIR}/inc .)

if(GMP_FOUND)
   include_directories(${GMP_INCLUDE_DIRS})
endif()


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

if(GDCC_IR AND EXISTS "${CMAKE_SOURCE_DIR}/ACC")
   add_subdirectory(ACC)
endif()

if(GDCC_IR AND EXISTS "${CMAKE_SOURCE_DIR}/AR")
   add_subdirectory(AR)
endif()

if(GDCC_IR AND EXISTS "${CMAKE_SOURCE_DIR}/AS")
   add_subdirectory(AS)
endif()

if(GDCC_IR AND EXISTS "${CMAKE_SOURCE_DIR}/BC")
   add_subdirectory(BC)
endif()

if(GDCC_IR AND EXISTS "${CMAKE_SOURCE_DIR}/CC")
   add_subdirectory(CC)
endif()

if(GDCC_IR AND EXISTS "${CMAKE_SOURCE_DIR}/CPP")
   add_subdirectory(CPP)
endif()

if(GDCC_Core)
   add_subdirectory(Core)
endif()

if(GDCC_IR)
   add_subdirectory(IR)
endif()

if(GDCC_IR AND EXISTS "${CMAKE_SOURCE_DIR}/IRDump")
   add_subdirectory(IRDump)
endif()

if(GDCC_IR AND EXISTS "${CMAKE_SOURCE_DIR}/LD")
   add_subdirectory(LD)
endif()

if(GDCC_IR AND EXISTS "${CMAKE_SOURCE_DIR}/MakeLib")
   add_subdirectory(MakeLib)
endif()

if(GDCC_IR AND EXISTS "${CMAKE_SOURCE_DIR}/NTSC")
   add_subdirectory(NTSC)
endif()

if(GDCC_Option)
   add_subdirectory(Option)
endif()

if(GDCC_IR AND EXISTS "${CMAKE_SOURCE_DIR}/SR")
   add_subdirectory(SR)
endif()

if(GDCC_Target)
   add_subdirectory(Target)
endif()

if(GDCC_INSTALL_API)
   install(FILES ${CMAKE_BINARY_DIR}/inc/GDCC/Config.hpp DESTINATION include/GDCC)
endif()

if(WIN32)
   if(GDCC_INSTALL_DOC)
      install(DIRECTORY doc/ DESTINATION doc)
      install(FILES COPYING DESTINATION doc)
      install(FILES COPYLIB DESTINATION doc)
   endif()

   install(DIRECTORY lib/ DESTINATION bin/lib)
else()
   if(GDCC_INSTALL_DOC)
      install(DIRECTORY doc/ DESTINATION share/gdcc/doc)
      install(FILES COPYING DESTINATION share/gdcc/doc)
      install(FILES COPYLIB DESTINATION share/gdcc/doc)
   endif()

   install(DIRECTORY lib/ DESTINATION share/gdcc/lib)
endif()

## EOF

