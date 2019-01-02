##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2019 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc targets.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

if(EXISTS "${CMAKE_SOURCE_DIR}/src/ACC")
   add_subdirectory(ACC)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/src/AR")
   add_subdirectory(AR)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/src/AS")
   add_subdirectory(AS)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/src/BC")
   add_subdirectory(BC)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/src/CC")
   add_subdirectory(CC)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/src/CPP")
   add_subdirectory(CPP)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/src/Core")
   add_subdirectory(Core)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/src/IR")
   add_subdirectory(IR)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/src/IRDump")
   add_subdirectory(IRDump)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/src/LD")
   add_subdirectory(LD)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/src/MakeLib")
   add_subdirectory(MakeLib)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/src/NTSC")
   add_subdirectory(NTSC)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/src/Option")
   add_subdirectory(Option)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/src/SR")
   add_subdirectory(SR)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/src/Target")
   add_subdirectory(Target)
endif()

## EOF

